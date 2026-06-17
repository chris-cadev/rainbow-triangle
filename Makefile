RAYLIB_DIR   := raylib
RAYLIB_INC   := raylib/install_native/raylib/include

RAYLIB_BUILD_DEV := raylib/install_native
RAYLIB_BUILD_REL := raylib/install_release

RAYLIB_LIB_DEV := $(RAYLIB_BUILD_DEV)/raylib/libraylib.a
RAYLIB_LIB_REL := $(RAYLIB_BUILD_REL)/raylib/libraylib.a

SRC    := src/main.cpp src/render.cpp src/collider.cpp src/input.cpp src/state.cpp
OBJ    := $(SRC:.cpp=.o)

# ── Platform detection ──────────────────────────────────────────────
ifeq ($(OS),Windows_NT)
  CXX           := g++
  LDLIBS        := -lraylib -lopengl32 -lgdi32 -lwinmm
  TARGET        := rainbow-triangle.exe
  RM            := del /f
  RMDIR         = cmd.exe /c if exist $(subst /,\,$(1)) rmdir /s /q $(subst /,\,$(1)) 2>nul
  CMAKE_G       := -G "MinGW Makefiles"
  EXTRA_LDFLAGS := -static -Wl,--gc-sections
  OBJ_PATHS     := $(subst /,\,$(OBJ))
else
  UNAME_S       := $(shell uname -s)
  ifeq ($(UNAME_S),Linux)
    CXX           := g++
    LDLIBS        := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    EXTRA_LDFLAGS := -Wl,--gc-sections
    OBJ_PATHS     := $(OBJ)
  endif
  ifeq ($(UNAME_S),Darwin)
    CXX           := clang++
    LDLIBS        := -lraylib -framework Cocoa -framework OpenGL -framework IOKit
    EXTRA_LDFLAGS := -Wl,-dead_strip
    OBJ_PATHS     := $(OBJ)
  endif
  TARGET        := rainbow-triangle
  RM            := rm -f
  RMDIR         = rm -rf $(1)
  CMAKE_G       :=
endif

.PHONY: all release clean wasm wasm-release clean-wasm

all: CXXFLAGS = -std=c++11 -Wall -O2 -g -Isrc -I$(RAYLIB_INC)
all: LDFLAGS  = -L$(RAYLIB_BUILD_DEV)/raylib
all: $(RAYLIB_LIB_DEV) $(TARGET)

# Disable unused raylib features for a smaller binary
RAYLIB_REL_CFLAGS = \
    -DSUPPORT_CAMERA_SYSTEM=0 \
    -DSUPPORT_GESTURES_SYSTEM=0 \
    -DSUPPORT_MOUSE_GESTURES=0 \
    -DSUPPORT_SCREEN_CAPTURE=0 \
    -DSUPPORT_COMPRESSION_API=0 \
    -DSUPPORT_AUTOMATION_EVENTS=0 \
    -DSUPPORT_CLIPBOARD_IMAGE=0 \
    -DSUPPORT_IMAGE_EXPORT=0 \
    -DSUPPORT_IMAGE_GENERATION=0 \
    -DSUPPORT_FILEFORMAT_PNG=0 \
    -DSUPPORT_FILEFORMAT_BMP=0 \
    -DSUPPORT_FILEFORMAT_JPG=0 \
    -DSUPPORT_FILEFORMAT_GIF=0 \
    -DSUPPORT_FILEFORMAT_QOI=0 \
    -DSUPPORT_FILEFORMAT_DDS=0 \
    -DSUPPORT_FILEFORMAT_TTF=0 \
    -DSUPPORT_FILEFORMAT_FNT=0 \
    -DSUPPORT_FILEFORMAT_OBJ=0 \
    -DSUPPORT_FILEFORMAT_MTL=0 \
    -DSUPPORT_FILEFORMAT_IQM=0 \
    -DSUPPORT_FILEFORMAT_GLTF=0 \
    -DSUPPORT_FILEFORMAT_VOX=0 \
    -DSUPPORT_FILEFORMAT_M3D=0 \
    -DSUPPORT_MESH_GENERATION=0 \
    -DSUPPORT_FILEFORMAT_WAV=0 \
    -DSUPPORT_FILEFORMAT_QOA=0 \
    -DSUPPORT_FILEFORMAT_XM=0 \
    -DSUPPORT_FILEFORMAT_MOD=0

release: CXXFLAGS = -std=c++11 -Wall -Os -flto -ffunction-sections -fdata-sections -s -Isrc -I$(RAYLIB_DIR)/src
release: LDFLAGS  = -s -L$(RAYLIB_BUILD_REL)/raylib $(EXTRA_LDFLAGS)
release: clean $(RAYLIB_LIB_REL) $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $(filter %.o,$^) $(LDFLAGS) $(LDLIBS)

src/%.o: src/%.cpp src/sounds.h src/colors.h src/constants.h src/render.h src/collider.h src/input.h src/state.h src/config.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(RAYLIB_LIB_DEV):
	cmake -S $(RAYLIB_DIR) -B $(RAYLIB_BUILD_DEV) $(CMAKE_G)
	cmake --build $(RAYLIB_BUILD_DEV)

$(RAYLIB_LIB_REL):
	cmake -S $(RAYLIB_DIR) -B $(RAYLIB_BUILD_REL) $(CMAKE_G) \
		-DCMAKE_BUILD_TYPE=MinSizeRel \
		-DBUILD_EXAMPLES=OFF \
		-DSUPPORT_MODULE_RMODELS=OFF \
		-DCMAKE_C_FLAGS="$(RAYLIB_REL_CFLAGS)"
	cmake --build $(RAYLIB_BUILD_REL)

# ── WebAssembly (Emscripten) ──────────────────────────────────────────────
RAYLIB_WASM     := raylib/install_wasm
RAYLIB_INC_WASM := $(RAYLIB_WASM)/raylib/include
RAYLIB_LIB_WASM := $(RAYLIB_WASM)/raylib/libraylib.a
EMXX            := em++
EMCMAKE         := emcmake

WEB_OUT        := web/index.html
WEB_SRC         = $(SRC)
WEB_OBJ         = $(WEB_SRC:src/%.cpp=web/%.o)
WEB_ASSETS     := src/assets

EM_LDFLAGS = \
	-lraylib \
	--use-port=contrib.glfw3:disableJoystick=true \
	-s WASM=1 \
	-s ASYNCIFY \
	-s USE_PTHREADS=1 \
	-s PTHREAD_POOL_SIZE=4 \
	-s GL_ENABLE_GET_PROC_ADDRESS=1 \
	-s MIN_WEBGL_VERSION=2 \
	-s MAX_WEBGL_VERSION=2 \
	-s FORCE_FILESYSTEM=1 \
	-s TOTAL_MEMORY=67108864 \
	--preload-file $(WEB_ASSETS)@src/assets \
	--shell-file web/shell.html

.PHONY: wasm wasm-release

wasm: CXXFLAGS = -std=c++11 -Wall -O2 -g -Isrc -I$(RAYLIB_INC_WASM) -DMA_ENABLE_AUDIO_WORKLETS
wasm: LDFLAGS  = -L$(RAYLIB_WASM)/raylib
wasm: $(RAYLIB_LIB_WASM) $(WEB_OUT)

wasm-release: CXXFLAGS = -std=c++11 -Wall -Os -flto -DNDEBUG -Isrc -I$(RAYLIB_INC_WASM) -DMA_ENABLE_AUDIO_WORKLETS
wasm-release: LDFLAGS  = -L$(RAYLIB_WASM)/raylib
wasm-release: $(RAYLIB_LIB_WASM) $(WEB_OUT)

$(WEB_OUT): $(WEB_OBJ)
	$(EMXX) -o $@ $(filter %.o,$^) $(LDFLAGS) $(EM_LDFLAGS)

web/%.o: web/%.cpp src/sounds.h src/colors.h src/constants.h src/render.h src/collider.h src/input.h src/state.h src/config.h
	$(EMXX) $(CXXFLAGS) -c $< -o $@

$(RAYLIB_LIB_WASM):
	$(EMCMAKE) cmake -S $(RAYLIB_DIR) -B $(RAYLIB_WASM) -DPLATFORM=Web -DBUILD_EXAMPLES=OFF -DCMAKE_C_FLAGS="-DMA_ENABLE_AUDIO_WORKLETS"
	cmake --build $(RAYLIB_WASM)

clean-wasm:
	-$(RM) web/*.o web/index.html web/index.js web/index.wasm web/*.data web/*.worker.js
	$(call RMDIR,$(RAYLIB_WASM))

clean:
	-$(RM) $(OBJ_PATHS)
	-$(RM) $(TARGET)
	-$(RM) web/*.o web/index.html web/index.js web/index.wasm web/*.data web/*.worker.js
	$(call RMDIR,$(RAYLIB_BUILD_REL))
	$(call RMDIR,$(RAYLIB_WASM))

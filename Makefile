RAYLIB_DIR   := raylib
RAYLIB_BUILD := raylib/install_native
RAYLIB_LIB   := $(RAYLIB_BUILD)/raylib/libraylib.a
RAYLIB_INC   := $(RAYLIB_BUILD)/raylib/include

CXX      := g++
CXXFLAGS := -std=c++11 -Wall -O3 -flto -Isrc -I$(RAYLIB_INC)
LDFLAGS  := -static -L$(RAYLIB_BUILD)/raylib
LDLIBS   := -lraylib -lopengl32 -lgdi32 -lwinmm

SRC      := src/main.cpp src/render.cpp src/collider.cpp src/input.cpp src/state.cpp
OBJ      := $(SRC:.cpp=.o)
TARGET   := rainbow-triangle.exe

.PHONY: all wasm clean

all: $(TARGET)

$(TARGET): $(OBJ) $(RAYLIB_LIB)
	$(CXX) -o $@ $(filter %.o,$^) $(LDFLAGS) $(LDLIBS)

src/%.o: src/%.cpp src/sounds.h src/colors.h src/render.h src/collider.h src/input.h src/state.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(RAYLIB_LIB):
	cmake -S $(RAYLIB_DIR) -B $(RAYLIB_BUILD) -G "MinGW Makefiles"
	cmake --build $(RAYLIB_BUILD)

wasm:
	emcc -o index.html src/main.cpp \
		-O2 \
		-Iraylib/install_wasm/include \
		-Lraylib/install_wasm/lib \
		-lraylib \
		-s TOTAL_MEMORY=67108864 \
		-s FORCE_FILESYSTEM=1 \
		--preload-file assets

run:
	./$(TARGET)

clean:
	cmd.exe /c "del /f $(subst /,\,$(OBJ)) $(TARGET) index.html index.js index.wasm 2>nul"

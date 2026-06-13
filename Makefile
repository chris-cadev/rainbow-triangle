CXX      := g++
CXXFLAGS := -std=c++11 -Wall -O2 -Isrc -Iraylib/install_native/include
LDFLAGS  := -static -Lraylib/install_native/lib
LDLIBS   := -lraylib -lopengl32 -lgdi32 -lwinmm

SRC      := src/main.cpp
OBJ      := $(SRC:.cpp=.o)
TARGET   := rainbow-triangle.exe

.PHONY: all wasm clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

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

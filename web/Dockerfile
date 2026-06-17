# Stage 1: Build WebAssembly
FROM emscripten/emsdk:latest AS wasm-builder
RUN apt-get update && apt-get install -y cmake && rm -rf /var/lib/apt/lists/*

WORKDIR /game
COPY . .

RUN emcmake cmake -S raylib -B raylib/install_wasm -DPLATFORM=Web -DBUILD_EXAMPLES=OFF -DCMAKE_C_FLAGS="-s USE_PTHREADS=1 -DMA_ENABLE_AUDIO_WORKLETS"

RUN cmake --build raylib/install_wasm

RUN mkdir -p /output && \
    em++ -std=c++11 -Os -Isrc -Iraylib/install_wasm/raylib/include \
        -Lraylib/install_wasm/raylib \
        -s USE_PTHREADS=1 -DMA_ENABLE_AUDIO_WORKLETS \
        src/main.cpp src/render.cpp src/collider.cpp src/input.cpp src/state.cpp \
        -lraylib \
        --use-port=contrib.glfw3:disableJoystick=true -s WASM=1 -s ASYNCIFY \
        -s AUDIO_WORKLET=1 -s WASM_WORKERS=1 \
        -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=4 \
        -s GL_ENABLE_GET_PROC_ADDRESS=1 \
        -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 \
        -s FORCE_FILESYSTEM=1 -s TOTAL_MEMORY=67108864 \
        --preload-file src/assets@src/assets \
        --shell-file web/shell.html \
        -o /output/index.html

# Stage 2: Serve with Caddy
FROM caddy:alpine
COPY --from=wasm-builder /output/ /usr/share/caddy/
COPY web/Caddyfile /etc/caddy/Caddyfile
COPY web/index.css /usr/share/caddy/index.css

# Stage 1: Build WebAssembly
FROM emscripten/emsdk:3.1.39 AS wasm-builder
ARG UMAMI_WEBSITE_ID
RUN apt-get update && apt-get install -y cmake && rm -rf /var/lib/apt/lists/*

WORKDIR /game
COPY . .

RUN emcmake cmake -S raylib -B raylib/install_wasm -DPLATFORM=Web -DBUILD_EXAMPLES=OFF

RUN cmake --build raylib/install_wasm

RUN mkdir -p /output && \
    em++ -std=c++11 -Os -Isrc -Iraylib/install_wasm/raylib/include \
        -Lraylib/install_wasm/raylib \
        src/main.cpp src/render.cpp src/collider.cpp src/input.cpp src/state.cpp \
        -lraylib \
        -s USE_GLFW=3 -s WASM=1 -s ASYNCIFY \
        -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 \
        -s FORCE_FILESYSTEM=1 -s TOTAL_MEMORY=67108864 \
        -s ALLOW_MEMORY_GROWTH=1 \
        --preload-file src/assets@src/assets \
        --shell-file web/shell.html \
        -o /output/index.html && \
    sed -i "s/__UMAMI_WEBSITE_ID__/${UMAMI_WEBSITE_ID}/g" /output/index.html

# Stage 2: Serve with Caddy
FROM caddy:alpine
RUN apk upgrade --no-cache
COPY --from=wasm-builder /output/ /usr/share/caddy/
COPY web/Caddyfile /etc/caddy/Caddyfile
COPY web/index.css /usr/share/caddy/index.css

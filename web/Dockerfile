# Stage 1: Build WebAssembly
FROM emscripten/emsdk:6.0.0 AS wasm-builder

ARG UMAMI_WEBSITE_ID

RUN apt-get update && \
    apt-get install -y cmake && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /game
COPY . .

RUN emcmake cmake \
    -S raylib \
    -B raylib/install_wasm \
    -DPLATFORM=Web \
    -DBUILD_EXAMPLES=OFF \
    -DCMAKE_C_FLAGS="-sASYNCIFY" \
    -DCMAKE_CXX_FLAGS="-sASYNCIFY"

RUN cmake --build raylib/install_wasm

RUN mkdir -p /output && \
    em++ \
        -std=c++11 \
        -O0 \
        -g \
        -gsource-map \
        --profiling-funcs \
        -sASSERTIONS=2 \
        -sSTACK_OVERFLOW_CHECK=2 \
        -sSTACK_SIZE=1048576 \
        -sASYNCIFY \
        -Isrc \
        -Iraylib/install_wasm/raylib/include \
        -Lraylib/install_wasm/raylib \
        src/main.cpp \
        src/render.cpp \
        src/collider.cpp \
        src/input.cpp \
        src/state.cpp \
        -lraylib \
        -sUSE_GLFW=3 \
        -sWASM=1 \
        -sMIN_WEBGL_VERSION=2 \
        -sMAX_WEBGL_VERSION=2 \
        -sFORCE_FILESYSTEM=1 \
        -sINITIAL_MEMORY=67108864 \
        -sALLOW_MEMORY_GROWTH=1 \
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
COPY web/thumbnail.png /usr/share/caddy/thumbnail.png
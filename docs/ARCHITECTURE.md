# Rainbow Triangle - Engineering Documentation

## Project Overview

Rainbow Triangle is a fast-paced arcade game built with [raylib](https://www.raylib.com/), a simple and easy-to-use library for learning game programming. The game is designed to run both natively (Windows/Linux/macOS) and on the web via WebAssembly (Emscripten).

---

## Architecture

### Project Structure

```
rainbow-triangle/
├── src/                    # Game source code
│   ├── main.cpp           # Entry point, window/input/audio lifecycle
│   ├── render.cpp         # All rendering/drawing logic
│   ├── state.cpp          # Game state management and game loop logic
│   ├── collider.cpp       # Triangle collision detection utilities
│   ├── input.cpp          # Input handling (keyboard, mouse, touch)
│   ├── constants.h        # Game tuning constants
│   ├── colors.h           # Color definitions and safe-mode variants
│   ├── state.h            # GameState struct and function declarations
│   └── sounds.h           # Sound asset declarations
├── raylib/                # git submodule - raylib library source
├── web/                   # WebAssembly/web deployment
│   ├── compose.yml        # Docker Compose configuration
│   ├── Dockerfile         # Multi-stage Docker build
│   ├── shell.html         # HTML shell template for Emscripten
│   └── index.css          # Styling for web version
├── Makefile               # Build orchestration for all targets
└── .github/               # GitHub workflows
```

---

### Game Architecture

The game follows a simple state machine pattern with 5 phases:

| Phase            | Description                                     |
| ---------------- | ----------------------------------------------- |
| `PHASE_MENU`     | Main menu with PLAY and OPTIONS buttons         |
| `PHASE_OPTIONS`  | Settings screen (difficulty, volume, safe mode) |
| `PHASE_WARNING`  | Photosensitivity warning before gameplay        |
| `PHASE_PLAYING`  | Active gameplay                                 |
| `PHASE_GAMEOVER` | Game over animation and retry/menu options      |

See `state.h` for the `GamePhase` enum and `GameState` struct.

---

## Build Systems

### Native Builds (Makefile)

The project uses a single Makefile with platform detection for cross-platform compilation:

```makefile
# Native targets
make all        # Debug build with raylib
make release    # Optimized release build
make clean      # Clean build artifacts
```

Platform detection handles:
- **Windows**: Uses MinGW g++, links opengl32/gdi32/winmm
- **Linux**: Links GL, pthread, dl, rt, X11
- **macOS**: Links Cocoa, OpenGL, IOKit frameworks

---

### WebAssembly Build (Emscripten)

The WebAssembly build uses Emscripten toolchain:

```makefile
# Web targets
make wasm        # Development-like build
make wasm-release # Production-optimized build
```

Key Emscripten settings used:
- `-s USE_GLFW=3` - Uses GLFW for window management on web
- `-s WASM=1` - Enable WebAssembly output
- `-s ASYNCIFY` - Asyncify for better async support
- `-s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2` - WebGL 2.0
- `-s FORCE_FILESYSTEM=1` - Enable virtual filesystem
- `-s TOTAL_MEMORY=67108864` - 64MB initial memory
- `-s ALLOW_MEMORY_GROWTH=1` - Allow memory to grow if needed

---

### Docker Build (compose.yml)

The Docker build is a multi-stage process:

**Stage 1: `wasm-builder`**
- Base: `emscripten/emsdk:3.1.39`
- Installs cmake
- Builds raylib from source for Web platform
- Compiles game to WebAssembly

**Stage 2: `stage-1` (final)**
- Base: `caddy:alpine`
- Serves the compiled WASM on port 80

---

## Build Results

### 2026-06-17 Docker Build: ✅ SUCCESS

After removing deprecated settings:
- `-s GL_ENABLE_GET_PROC_ADDRESS=1` - removed (deprecated in emsdk 3.1.39)
- `-s EXPORTED_RUNTIME_METHODS='["ccall","HEAPF32"]'` - removed (unused, code uses EM_ASM inline macros)

Build completes successfully with no warnings.

---

## Quick Reference

### Build Commands

```bash
# Native builds
make all          # Build for current platform (debug)
make release      # Optimized native build

# WebAssembly
make wasm         # Web build
make wasm-release # Production web build

# Docker deployment
cd web
cp .env.example .env  # Set UMAMI_WEBSITE_ID
docker compose up --build -d
```

### Game Controls

| Input              | Action                    |
| ------------------ | ------------------------- |
| Left/A/H           | Move triangle left        |
| Right/D/L          | Move triangle right       |
| Space/Enter        | Select/action             |
| Mouse Click        | Select (pointer position) |
| Touch (left half)  | Move left                 |
| Touch (right half) | Move right                |

---

## Dependencies

| Dependency | Version                | Purpose                        |
| ---------- | ---------------------- | ------------------------------ |
| raylib     | 6.0.0 (from submodule) | Game library                   |
| Emscripten | 3.1.39                 | WebAssembly compilation        |
| CMake      | Latest                 | Build system                   |
| Caddy      | alpine                 | Web server for WASM deployment |
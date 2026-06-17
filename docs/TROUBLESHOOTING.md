# Troubleshooting: Emscripten Build Error

## Error Summary

```
em++: error: Attempt to set a non-existent setting: 'GL_ENABLE_GET_PROC_ADDRESS'
```

The Docker build fails during the final `em++` compilation step because `GL_ENABLE_GET_PROC_ADDRESS` is not a recognized Emscripten setting in version 3.1.39.

---

## ✅ Resolution Applied

The `GL_ENABLE_GET_PROC_ADDRESS=1` setting has been **removed** from both:
- `web/Dockerfile` (line 19)
- `Makefile` (line 61)

This setting was deprecated in modern Emscripten versions and is no longer necessary for raylib Web builds.

---

## Root Cause

The setting `GL_ENABLE_GET_PROC_ADDRESS=1` in `web/Dockerfile` (line 19) and `Makefile` (line 61) is **deprecated or removed** in Emscripten 3.1.39.

This setting was used in older Emscripten versions to enable `glGetProcAddress()` for OpenGL function loading. Modern Emscripten handles this differently.

---

## Affected Files (Before Fix)

| File             | Line | Content                             |
| ---------------- | ---- | ----------------------------------- |
| `web/Dockerfile` | 19   | `-s GL_ENABLE_GET_PROC_ADDRESS=1 \` |
| `Makefile`       | 61   | `-s GL_ENABLE_GET_PROC_ADDRESS=1 \` |

---

## Investigation Steps

1. **Check valid Emscripten settings** in the container:
   ```bash
   docker run --rm emscripten/emsdk:3.1.39 emcc --help | grep -i gl
   ```

2. **Check raylib's CMakeLists.txt** - already handles WebGL settings for the library itself (lines 117-119 in `raylib/src/CMakeLists.txt`)

3. **Verify if the setting is actually needed** - raylib's Web platform support may have changed

---

## Solutions Tried

### ✅ Option A: Remove the Setting (Applied)

The setting was unnecessary. Removed from both files:

**Before:**
```bash
-s USE_GLFW=3 -s WASM=1 -s ASYNCIFY \
-s GL_ENABLE_GET_PROC_ADDRESS=1 \
-s EXPORTED_RUNTIME_METHODS='["ccall","HEAPF32"]' \
-s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 \
```

**After:**
```bash
-s USE_GLFW=3 -s WASM=1 -s ASYNCIFY \
-s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 \
```

Also removed `EXPORTED_RUNTIME_METHODS` because the code uses `EM_ASM` inline macros (not `ccall`), and `HEAPF32` was never used.

---

## Verification Steps

Build with:
```bash
docker compose build --no-cache
```

Expected output: Successful build with stage-1 and wasm-builder completing.
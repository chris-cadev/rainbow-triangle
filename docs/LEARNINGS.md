# Learnings & Knowledge Base

## Current Understanding

### Project Tech Stack

* **Game Library**: raylib 6.0.0 (vendored as git submodule)
* **Build System**: CMake + Makefile (hybrid approach)
* **Web Target**: Emscripten 3.1.39 → WebAssembly
* **Web Server**: Caddy (Alpine variant)
* **Deployment**: Docker Compose with Cloudflare tunnel option

### Game Design

* Simple arcade game: move a triangle left/right through color-matching columns
* State machine with 5 phases: Menu, Options, Warning, Playing, GameOver
* Photosensitive epilepsy safe mode available
* Touch-friendly controls for mobile

---

## Resolution Summary

The Emscripten build error was caused by two deprecated/unnecessary settings:

1. **`GL_ENABLE_GET_PROC_ADDRESS=1`** - This setting was removed from Emscripten 3.1.x. Modern versions handle OpenGL function loading automatically.

2. **`EXPORTED_RUNTIME_METHODS='["ccall","HEAPF32"]'`** - The code uses `EM_ASM` inline macros (not `ccall`), and `HEAPF32` was never used. Removing this eliminates the warning.

Both settings have been removed from `web/Dockerfile` and `Makefile`. The Docker build now completes successfully with no warnings.

---

## Files Changed

| File             | Change                                                                                            |
| ---------------- | ------------------------------------------------------------------------------------------------- |
| `web/Dockerfile` | Removed `-s GL_ENABLE_GET_PROC_ADDRESS=1` and `-s EXPORTED_RUNTIME_METHODS='["ccall","HEAPF32"]'` |
| `Makefile`       | Same removals in `EM_LDFLAGS` variable                                                            |

---

## Verification

```bash
make dev
```

or

```bash
docker compose build --no-cache
```

Result: Build completes successfully with no errors or warnings.

---

## WebAssembly Deployment Notes

### General WASM Deployment Requirements

WebAssembly applications are delivered as a combination of HTML, JavaScript, and `.wasm` binary files. Successful deployment depends on the web server correctly serving all generated assets.

Key requirements:

* Serve `.wasm` files with the MIME type `application/wasm`
* Enable static file hosting for all generated Emscripten artifacts (`.html`, `.js`, `.wasm`, `.data`)
* Ensure HTTP range requests and compression do not interfere with WASM delivery
* Use HTTPS in production environments to guarantee browser compatibility with modern web APIs
* Configure caching policies carefully during development to avoid stale WASM binaries

### Containerized Deployment Pattern

The current project follows a common production pattern:

1. Build the application using an Emscripten container
2. Copy generated artifacts into a lightweight web server image
3. Expose the web server through Docker Compose
4. Optionally publish through a reverse proxy or Cloudflare Tunnel

Benefits:

* Reproducible builds
* Small runtime image size
* Separation between build and serving stages
* Simplified deployment to VPS, cloud, or self-hosted environments

### Deployment Checklist

Before publishing a WASM application:

* Confirm the `.wasm` file is accessible from the browser
* Verify browser developer tools show successful WASM loading
* Check MIME type is `application/wasm`
* Validate mobile browser compatibility
* Test with browser cache cleared
* Verify all asset paths are relative or correctly configured for the deployment URL
* Confirm HTTPS is enabled for production deployments

### Common Deployment Issues

| Issue                               | Typical Cause                                   |
| ----------------------------------- | ----------------------------------------------- |
| WASM fails to load                  | Incorrect MIME type                             |
| Blank page after deployment         | Missing or inaccessible `.wasm` asset           |
| Works locally but not in production | Asset path or server configuration mismatch     |
| Changes not appearing after deploy  | Browser caching old WASM binary                 |
| Mobile-only failures                | Touch handling or viewport configuration issues |

### Current Project Status

The project now builds successfully as a WebAssembly target using Emscripten 3.1.39 and can be deployed through Docker Compose with Caddy serving the generated assets. The deployment architecture follows standard multi-stage container practices and is suitable for local hosting, VPS deployment, or exposure through a Cloudflare Tunnel.

#pragma once

// Timing
constexpr float MAX_FRAME_DELTA = 0.05f;
constexpr int   TARGET_FPS      = 144;

// Window
constexpr int   INITIAL_WIDTH   = 400;

// Aspect ratio (width = height / ASPECT_DIVISOR)
constexpr int   ASPECT_DIVISOR  = 2;

// Gameplay tuning
constexpr float HOP_DURATION    = 0.12f;
constexpr float HOP_HEIGHT      = 6.0f;
constexpr float TIP_TOLERANCE   = 0.15f;  // fraction of column width
constexpr float WALL_COOLDOWN   = 1.2f;
constexpr float SPEED_SCALE     = 1.15f;
constexpr float MAX_FALL_SPEED  = 0.75f;   // screen-heights/sec (was 600px/s @ 800px ref)
constexpr float ACCEL_SCALE     = 1.30f;
constexpr float MAX_ACCEL       = 1.125f;  // screen-heights/s² (was 900px/s² @ 800px ref)
constexpr int   ACCEL_INTERVAL  = 3;

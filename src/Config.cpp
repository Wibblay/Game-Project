#include "Config.h"

// Define constants that require initialization (like strings or non-primitive types)
const char* WindowConfig::WINDOW_TITLE = "2D Isometric Game";

const float CameraConfig::MIN_ZOOM_LEVEL = 1.0f;
const float CameraConfig::MAX_ZOOM_LEVEL = 30.0f;
const float CameraConfig::DEFAULT_ZOOM_LEVEL = 10.0f;
const float CameraConfig::ZOOM_INCREMENT_VALUE = 1.0f;

const float MapConfig::WATER_NOISE_THRESHOLD = 0.25f;
const float MapConfig::LOWER_HILL_NOISE_THRESHOLD = 0.45f;
const float MapConfig::UPPER_HILL_NOISE_THRESHOLD = 0.55f;
const Color MapConfig::MAP_GRASS_COLOR(0.05f, 0.82f, 0.23f);
const Color MapConfig::MAP_WATER_COLOR(0.13f, 0.32f, 0.96f);
const Color MapConfig::MAP_HILL_COLOR(0.14f, 0.9f, 0.37f);
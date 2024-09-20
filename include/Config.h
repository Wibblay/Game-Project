#ifndef CONFIG_H
#define CONFIG_H

#include <glm/glm.hpp>
#include <GL/glew.h>

struct WindowConfig 
{
    static const int DEFAULT_WINDOW_WIDTH = 800;    // Default window width
    static const int DEFAULT_WINDOW_HEIGHT = 600;   // Default window height
    static const char* WINDOW_TITLE;                // Window title
};

struct CameraConfig 
{
    static const float MIN_ZOOM_LEVEL;       // Minimum zoom level
    static const float MAX_ZOOM_LEVEL;       // Maximum zoom level
    static const float DEFAULT_ZOOM_LEVEL;   // Default zoom level
    static const float ZOOM_INCREMENT_VALUE; //Size of zoom increment
};

struct Color
{
    float r, g, b;

    Color(float red, float green, float blue) : r(red), g(green), b(blue) {}

    void apply() const {
        glColor3f(r, g, b);
    }
};

struct MapConfig
{
    static const int CHUNK_SIZE = 64;       //Width of a chunk in tiles
    static const int MAP_NOISE_SEED = 1337; //Seed used for map perlin noise
    static const int CHUNKS_TO_RENDER_GRID_SIZE = 3; //Size of grid of chunks to render (should be odd integer)
    static const float WATER_NOISE_THRESHOLD; //Between -1 and 1
    static const float LOWER_HILL_NOISE_THRESHOLD;
    static const float UPPER_HILL_NOISE_THRESHOLD;
    static const Color MAP_GRASS_COLOR;
    static const Color MAP_WATER_COLOR;
    static const Color MAP_HILL_COLOR;
};

#endif

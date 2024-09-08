#ifndef CONFIG_H
#define CONFIG_H

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


struct MapConfig
{
    static const int CHUNK_SIZE = 64;       //Width of a chunk in tiles
    static const int MAP_NOISE_SEED = 1337; //Seed used for map perlin noise
    static const int CHUNKS_TO_RENDER_GRID_SIZE = 3; //Size of grid of chunks to render (should be odd integer)
};

#endif

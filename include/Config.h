#ifndef CONFIG_H
#define CONFIG_H

#include <glm/glm.hpp>
#include <GL/glew.h>

namespace GlobalConfig 
{
    // WINDOW CONFIG
    inline constexpr int DEFAULT_WINDOW_WIDTH = 1000;    // Default window width
    inline constexpr int DEFAULT_WINDOW_HEIGHT = 800;   // Default window height
    inline static const char* WINDOW_TITLE = "2D Isometric Game";                // Window title

    // MAP CONFIG
    inline constexpr int CHUNK_SIZE = 64;       //Width of a chunk in tiles
    inline constexpr int MAP_NOISE_SEED = 1337; //Seed used for map perlin noise
}



#endif

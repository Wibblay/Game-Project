#pragma once

#include "FastNoiseLite.h"

class Noise 
{
public:
    Noise(int seed = 1337);    
    ~Noise() = default;

    float GetNoise(float x, float y) const;  // Generate noise for a specific coordinate

private:
    FastNoiseLite noiseGenerator;   // FastNoise generator instance

    const size_t octaves;              // Number of octaves
    const float persistence;     // Controls amplitude of octaves
    const float lacunarity;      // Controls frequency of octaves
};

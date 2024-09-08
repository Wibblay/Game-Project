#ifndef NOISE_H
#define NOISE_H

#include "FastNoiseLite.h"

class Noise 
{
public:
    Noise(int seed = 1337);    
    ~Noise() = default;

    float GetNoise(float x, float y) const;  // Generate noise for a specific coordinate

private:
    FastNoiseLite noiseGenerator;   // FastNoise generator instance
};

#endif 

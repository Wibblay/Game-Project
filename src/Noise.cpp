#include "Noise.h"

Noise::Noise(int seed) 
{
    noiseGenerator.SetSeed(seed);                                  // Set the seed for noise generation
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_Perlin);  // Use Perlin noise
    noiseGenerator.SetFrequency(0.05f);                            // Set frequency for noise generation (adjust as needed)
}

float Noise::GetNoise(float x, float y) const 
{
    // Generate Perlin noise value for the given x, y coordinates
    // FastNoiseLite returns values in the range [-1, 1], so scale as necessary
    return noiseGenerator.GetNoise(x, y);
}

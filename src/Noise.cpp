#include "Noise.h"

Noise::Noise(int seed) : octaves(4), persistence(0.5f), lacunarity(2.0f)
{
    noiseGenerator.SetSeed(seed);                                  // Set the seed for noise generation
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_Perlin);  // Use Perlin noise
    noiseGenerator.SetFrequency(1.0f);                            // Set frequency for noise generation (adjust as needed)
}

float Noise::GetNoise(float x, float y) const 
{    
    float noiseValue = 0.0f;            // Accumulates the final noise value
    float amplitude = 1.0f;             // Initial amplitude
    float frequency = 0.02f;             // Initial frequency
    float maxAmplitude = 0.0f;          // Normalizing factor to ensure the final noise is in [0,1]

    // Generate noise using multiple octaves
    for (size_t i = 0; i < octaves; ++i) 
    {
        // Get noise value for the current octave
        float octaveNoise = noiseGenerator.GetNoise(x * frequency, y * frequency);
        
        // Accumulate the noise value
        noiseValue += octaveNoise * amplitude;
        maxAmplitude += amplitude;

        // Update amplitude and frequency for the next octave
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    // Normalize the final noise value to the range [0, 1]
    noiseValue = (noiseValue / maxAmplitude) * 0.5f + 0.5f;

    return noiseValue;
}

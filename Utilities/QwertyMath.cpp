// ============================================================================
// Includes
// ============================================================================
#include "QwertyMath.h"

namespace AugCSynth
{

// ============================================================================
// Globals
// ============================================================================
static uint32_t gSeed;


// ============================================================================
// Public functions
// ============================================================================
uint32_t GetNextRand()
{
    gSeed *= 1664525;
    gSeed += 1013904223;
    return gSeed;
}

float FastUnitExp(float x)
{
    float g = 0.75 * x;
    g += 0.25f;
    x *= g;
    return x + 1.0f;
}

float ComputeLfoMult(float lfoValue, float lfoAmount)
{
    if(signbit(lfoAmount)) // If negative
    {
        lfoValue += 1.0f;
    }
    else
    {
        lfoValue -= 1.0f;
    }

    return 1.0f + lfoAmount * lfoValue;
}

float ComputeLoudnessMult(float loudness, float amount)
{
    loudness -= 1.0f;
    return 1.0f + loudness * amount;
}

float ShapeWave(float waveValue, float waveShape)
{
    float g = waveValue * waveValue;
    g *= g;
    // g *= g;

    g *= waveShape;
    g += (1.0f - waveShape);

    return waveValue * g;
}

#define DRIVE_K (0.9f)
#define DRIVE_M (4.0f*DRIVE_K + 1.0f)

#define DRIVE_A (DRIVE_K*DRIVE_M - DRIVE_K)
#define DRIVE_B ((DRIVE_K+1.0f)*(1.0f-DRIVE_M))
#define DRIVE_C (DRIVE_M)

float DrivenSample(float sample)
{
    float p;
    if(signbit(sample))
    {
        sample = -sample;
        if(sample > 1.0f)
        {
            return -1.0f;
        }
        p = DRIVE_B + DRIVE_A*sample;
        p *= sample;
        p += DRIVE_C;
        return -p*sample;
    }

    if(sample > 1.0f)
    {
        return 1.0f;
    }

    p = DRIVE_B + DRIVE_A*sample;
    p *= sample;
    p += DRIVE_C;
    return p*sample;
}

} // namespace AugCSynth

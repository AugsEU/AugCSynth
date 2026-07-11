// ============================================================================
// Sine wave generation functions
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>


namespace AugCSynth
{

// ============================================================================
// Public functions (inline for performance)
// ============================================================================

/// @brief Sine with quadratic formula
inline static float SineQuadraic(float phase)
{
    float x = phase - (float)(phase >= 0.5f);
    float x2 = fabs(x) * -16.0f;
	x2 += 8.0f;
	x *= x2;
	return x;
}

/// @brief Sine with quintic formula
inline static float SineQuintic(float phase)
{
    const float A = 73.69528f;
    const float B = -41.11008f;
    const float C = 6.28155;

    float x = phase;
    float x2;
    float x4;
    
    if (x < 0.25f)
    {
        x2 = x * x;
        x4 = x2 * x2;

        x2 *= B;
        x2 += C;

        x4 *= A;
        x4 += x2;

        x *= x4;
    }
    else if (x < 0.5f)
    {
        x = 0.5f - x;
        x2 = x * x;
        x4 = x2 * x2;

        x2 *= B;
        x2 += C;

        x4 *= A;
        x4 += x2;

        x *= x4;
    }
    else if (x < 0.75f)
    {
        x -= 0.5f;

        x2 = x * x;
        x4 = x2 * x2;

        x2 *= B;
        x2 += C;

        x4 *= -A;
        x4 -= x2;

        x *= x4;
    }
    else
    {
        x = 1.0f - x;

        x2 = x * x;
        x4 = x2 * x2;

        x2 *= B;
        x2 += C;

        x4 *= -A;
        x4 -= x2;

        x *= x4;
    }

    return x;
}

} // namespace AugCSynth

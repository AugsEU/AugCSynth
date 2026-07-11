// ============================================================================
// Square wave generation functions
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>

namespace AugCSynth
{

// ============================================================================
// Public functions (inline for performance)
// ============================================================================

/// @brief Mathematically perfect square wave.
/// Beware includes freq above nyquist!
static inline float SquareWave(float phase)
{
    return phase < 0.5f ? 1.0f : -1.0f;
}

/// @brief Compute wave for low frequency oscillator
static inline float SquareWaveLFO(float phase)
{
    const float DT = 0.01f;

    float value = phase < 0.5f ? 1.0f : -1.0f;
    if(phase < DT)
    {
        value = phase * (1.0f / DT);
        phase = 2.0f - value;
        value *= phase;
    }
    else if(phase < 0.5f - DT)
    {
        value = 1.0f;
    }
    else if (phase < 0.5f)
    {
        value = phase - 0.5f;
        value *= (1.0f / DT);
        value += 2.0f;
        phase = 2.0f - value;
        value *= phase;
    }
    else if(phase < 0.5f + DT)
    {
        value = phase - 0.5f;
        value *= (1.0f / DT);
        phase = value - 2.0f;
        value *= phase;
    }
    else if (phase < 1.0f - DT)
    {
        value = -1.0f;
    }
    else
    {
        value = phase-1;
        value *= (1.0f / DT);
        value += 2.0f;
        phase = value - 2.0f;
        value *= phase;
    }

    return value;
}

/// @brief Square wave with polyBLEP and wave shaping
static inline float SquareWaveBLEPShape(float phase, float dt, float shape)
{
    shape += 0.255f;
    shape *= 0.6125f;

    float value = phase < shape ? 1.0f : -1.0f;
    if(phase < dt)
    {
        value = phase / dt;
        phase = 2.0f - value;
        value *= phase;
    }
    else if(phase < shape - dt)
    {
        value = 1.0f;
    }
    else if (phase < shape)
    {
        value = phase - shape;
        value /= dt;
        value += 2.0f;
        phase = 2.0f - value;
        value *= phase;
    }
    else if(phase < shape + dt)
    {
        value = phase - shape;
        value /= dt;
        phase = value - 2.0f;
        value *= phase;
    }
    else if (phase < 1.0f - dt)
    {
        value = -1.0f;
    }
    else
    {
        value = phase-1;
        value /= dt;
        value += 2.0f;
        phase = value - 2.0f;
        value *= phase;
    }

    return value;
}

/// @brief Square wave with organ-like sound
static inline float SquareWaveOrgan(float phase, float shape)
{
    const float DT = 0.1f;
    shape += 0.45f;
    shape *= 0.52f;

    float value = phase < shape ? 1.0f : -1.0f;
    if(phase < DT)
    {
        value = phase * (1.0f / DT);
        phase = 2.0f - value;
        value *= phase;
    }
    else if(phase < shape - DT)
    {
        value = 1.0f;
    }
    else if (phase < shape)
    {
        value = phase - shape;
        value *= (1.0f / DT);
        value += 2.0f;
        phase = 2.0f - value;
        value *= phase;
    }
    else if(phase < shape + DT)
    {
        value = phase - shape;
        value *= (1.0f / DT);
        phase = value - 2.0f;
        value *= phase;
    }
    else if (phase < 1.0f - DT)
    {
        value = -1.0f;
    }
    else
    {
        value = phase-1;
        value *= (1.0f / DT);
        value += 2.0f;
        phase = value - 2.0f;
        value *= phase;
    }

    return value;
}

} // namespace AugCSynth

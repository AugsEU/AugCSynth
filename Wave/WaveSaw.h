// ============================================================================
// Sine wave generation functions
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Include
// ============================================================================
#include "WavePolyBlep.h"

namespace AugCSynth
{

// ============================================================================
// Public functions (inline for performance)
// ============================================================================

/// @brief Get value of oscillator as saw tooth with PolyBLEP
static inline float SawWaveBLEP(float phase, float dt)
{
	phase += 0.5;
    if (phase >= 1.0f) 
    {
        phase -= 1.0f;
    }

    float saw = 2.0f * phase - 1.0f;
    return saw - PolyBlep(phase, dt);
}

static inline float SawWaveLFO(float phase)
{
    const float DT = 0.01f;
    
    phase += 0.5;
    if (phase >= 1.0f) 
    {
        phase -= 1.0f;
    }

    float saw = 2.0f * phase - 1.0f;
    return saw - PolyBlep(phase, DT);
}

} // namespace AugCSynth

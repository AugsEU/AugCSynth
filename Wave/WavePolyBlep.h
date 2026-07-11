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

static inline float PolyBlep(float t, float dt)
{
    if (t < dt)
    {
        t /= dt;
        return 2.0f * t - t * t - 1.0f;
    }
    else if (t > 1.0f - dt)
    {
        t = (t - 1.0f) / dt;
        return t*t + 2.0f*t + 1.0f;
    }

    return 0.0f;
}

} // namespace AugCSynth

// ============================================================================
// Includes
// ============================================================================
#include "Oscillator.h"

// ============================================================================
// Defines
// ============================================================================
#define ACCURATE_SINE 0

namespace AugCSynth::Subtractive
{

// ============================================================================
// Public functions
// ============================================================================

/// @brief Initialise an oscillator 
void OscInit(Oscillator* osc)
{
    osc->mPhase = 0.0f;
}



/// @brief Increment phase of oscillator by 1 sample.
void OscPhaseInc(Oscillator* osc, float phaseInc)
{
    osc->mPhase += phaseInc;

	if (osc->mPhase >= 1.0f)
    {
		osc->mPhase -= 1.0f;
    }
}

}
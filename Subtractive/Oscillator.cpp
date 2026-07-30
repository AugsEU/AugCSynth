// ============================================================================
// Includes
// ============================================================================
#include "Oscillator.h"
#include "Subtractive/SubParams.h"

namespace AugCSynth::Subtractive
{

// ============================================================================
// Public functions
// ============================================================================

/// @brief Initialise an oscillator 
void Oscillator::Init()
{
    mPhase = 0.0f;
}



/// @brief Increment phase of oscillator by 1 sample.
void Oscillator::PhaseInc(float phaseInc)
{
    mPhase += phaseInc;

	if (mPhase >= 1.0f)
    {
		mPhase -= 1.0f;
    }
}

}
// ============================================================================
// Includes
// ============================================================================

#include "AugCSynth.h"
#include "AugCState.h"
#include "Tuning.h"

namespace AugCSynth{

// ============================================================================
// Public functions
// ============================================================================

void Initialise()
{
	State().mMode = SynthMode::Subtractive;

	SetTuning(Tuning::Equal12);

	Subtractive::SynthInit();
}

void FillSoundBuffer(int16_t* buf, uint16_t samples)
{
	switch (State().mMode)
	{
	case SynthMode::Subtractive:
		Subtractive::FillSoundBuffer(buf, samples);
		break;
	}
}

}
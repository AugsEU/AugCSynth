// ============================================================================
// Aggregate header file
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Include
// ============================================================================

#include <Config.h>
#include <Parameters.h>
#include <Voice.h>
#include <Filter/Filter.h>
#include <Delay/Delay.h>
#include <Parameters.h>

#include <Subtractive/SubParams.h>
#include <Subtractive/SubWaveGen.h>

namespace AugCSynth {

// ============================================================================
// Public interface
// ============================================================================
#ifndef AUGCSYNTH_NO_STATE

void Initialise();
void FillSoundBuffer(int16_t* buf, uint16_t samples);

#endif //!AUGCSYNTH_NO_STATE

}


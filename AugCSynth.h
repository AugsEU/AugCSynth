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

#include <Subtractive/SubParams.h>
#include <Subtractive/SubWaveGen.h>

namespace AugCSynth {

// ============================================================================
// Public interface
// ============================================================================

void Initialise();
void FillSoundBuffer(int16_t* buf, uint16_t samples);


}


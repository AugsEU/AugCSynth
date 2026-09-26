// ============================================================================
// Configuration for library
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Include
// ============================================================================
namespace AugCSynth
{

#if !defined(AUGCSYNTH_CONFIG_SAMPLE_RATE) || !defined(AUGCSYNTH_CONFIG_VOICE_POLYPHONY)
#error Please configure AugCSynth using passed in preprocessor flags -D[...]=X
#endif

constexpr size_t SAMPLE_RATE = AUGCSYNTH_CONFIG_SAMPLE_RATE;
constexpr float SAMPLE_PERIOD = ((1.0f / (float)SAMPLE_RATE));
constexpr uint8_t VOICE_POLYPHONY = AUGCSYNTH_CONFIG_VOICE_POLYPHONY;

}
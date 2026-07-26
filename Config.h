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

constexpr size_t SAMPLE_RATE = 41000ull;
constexpr float SAMPLE_PERIOD = ((1.0f / (float)SAMPLE_RATE));
constexpr uint8_t VOICE_POLYPHONY = 4;

}
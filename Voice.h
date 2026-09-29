// ============================================================================
// Logic for voices and voice allocations
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Include
// ============================================================================
#include "Subtractive/SubVoice.h"





namespace AugCSynth
{

// ============================================================================
// Public constants
// ============================================================================

constexpr uint8_t INVALID_NOTE = 0xFF;





// ============================================================================
// Voice meta object
// ============================================================================
#ifndef AUGCSYNTH_NO_STATE

struct Voice
{
    uint8_t mNoteNum;
    
    union
    {
        Subtractive::SubVoice mSubVoice;
    };

    bool CanBeAllocated();
};



// ============================================================================
// Public functions
// ============================================================================

void BeginVoice(uint8_t note);
void ReleaseVoice(uint8_t note);
void StopVoice(uint8_t note);

#endif // !AUGCSYNTH_NO_STATE

} // namespace AugCSynth

// ============================================================================
// Include
// ============================================================================
#include <Voice.h>
#include <Tuning.h>
#include <Config.h>
#include <algorithm>

namespace AugCSynth
{

// ============================================================================
// Globals
// ============================================================================
Voice gVoices[VOICE_POLYPHONY];
uint8_t gFreeNoteSearchStart = 0;




// ============================================================================
// Forward decl
// ============================================================================





// ============================================================================
// Voice
// ============================================================================
bool Voice::CanBeAllocated()
{
    return mNoteNum == INVALID_NOTE;
}





// ============================================================================
// Public functions
// ============================================================================
void BeginVoice(uint8_t note)
{
    if(note == 0xFF)
        return;

    uint8_t bestVoiceIdx = 0xFF;
    float bestVoiceValue = 0.0f;

    for(uint8_t i = gFreeNoteSearchStart; i < VOICE_POLYPHONY; i++)
    {
        Voice& newVoice = gVoices[i];
        float currVoiceValue = Subtractive::VoiceEligibility(&newVoice.mSubVoice, note);
        if(currVoiceValue > bestVoiceValue)
        {
            bestVoiceIdx = i;
            bestVoiceValue = currVoiceValue;    
            if(currVoiceValue >= 10.0f)
            {
                // This is good enough.
                break;
            }
        }
    }

    if(bestVoiceIdx != 0xFF)
    {
        Subtractive::VoiceOn(&gVoices[bestVoiceIdx].mSubVoice, note);
        gVoices[bestVoiceIdx].mNoteNum = note;
    }
}

void ReleaseVoice(uint8_t note)
{
    if(note == 0xFF)
        return;

    for(uint8_t i = 0; i < VOICE_POLYPHONY; i++)
    {
        if(gVoices[i].mNoteNum == note)
        {
            Subtractive::VoiceOff(&gVoices[i].mSubVoice);
            gFreeNoteSearchStart = std::min(i, gFreeNoteSearchStart);
            return;
        }       
    }
}

void StopVoice(uint8_t note)
{
    if(note == 0xFF)
        return;

    for(uint8_t i = 0; i < VOICE_POLYPHONY; i++)
    {
        if(gVoices[i].mNoteNum == note)
        {
            // ToDo Stop correctly
            Subtractive::VoiceOff(&gVoices[i].mSubVoice);
            gVoices[i].mNoteNum = INVALID_NOTE;
            gFreeNoteSearchStart = std::min(i, gFreeNoteSearchStart);
            return;
        }       
    }
}

// ============================================================================
// Private functions
// ============================================================================

} // namespace AugCSynth

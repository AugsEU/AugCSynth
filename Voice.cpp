// ============================================================================
// Include
// ============================================================================
#include <Voice.h>
#include <Tuning.h>
#include <Config.h>
#include <algorithm>
#include <AugCState.h>


namespace AugCSynth
{
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

    for(uint8_t i = State().mFreeNoteSearchStart; i < VOICE_POLYPHONY; i++)
    {
        Voice& newVoice = State().mVoices[i];
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
        Subtractive::VoiceOn(&State().mVoices[bestVoiceIdx].mSubVoice, note);
        State().mVoices[bestVoiceIdx].mNoteNum = note;
    }
}

void ReleaseVoice(uint8_t note)
{
    if(note == 0xFF)
        return;

    for(uint8_t i = 0; i < VOICE_POLYPHONY; i++)
    {
        if(State().mVoices[i].mNoteNum == note)
        {
            Subtractive::VoiceOff(&State().mVoices[i].mSubVoice);
            State().mFreeNoteSearchStart = std::min(i, State().mFreeNoteSearchStart);
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
        if(State().mVoices[i].mNoteNum == note)
        {
            // ToDo Stop correctly
            Subtractive::VoiceOff(&State().mVoices[i].mSubVoice);
            State().mVoices[i].mNoteNum = INVALID_NOTE;
            State().mFreeNoteSearchStart = std::min(i, State().mFreeNoteSearchStart);
            return;
        }       
    }
}

// ============================================================================
// Private functions
// ============================================================================

} // namespace AugCSynth

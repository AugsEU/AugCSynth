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
        // To do: fix this
        float currVoiceValue = newVoice.mSubVoice.Eligibility(note);
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
        // To do: Fix this
        State().mVoices[bestVoiceIdx].mSubVoice.VoiceOn(note);
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
            // To do: Fix this
            State().mVoices[i].mSubVoice.VoiceOff();
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
            State().mVoices[i].mSubVoice.VoiceOff();
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

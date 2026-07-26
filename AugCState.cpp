// ============================================================================
// Include
// ============================================================================
#include "AugCState.h"

#include <cstring>


namespace AugCSynth{

// ============================================================================
// Public functions
// ============================================================================

void AugCState::Init()
{
	std::memset(this, 0x00, sizeof(AugCState));

	for(int i = 0; i < VOICE_POLYPHONY; ++i)
	{
		mVoices[i].mNoteNum = INVALID_NOTE;
	}

	mFreeNoteSearchStart = 0;
}

}
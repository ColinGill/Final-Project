/*
  ==============================================================================

    LeadMode.cpp
    Created: 1 May 2018 2:36:05pm
    Author:  Donnacha

  ==============================================================================
*/

#include "LeadMode.h"

int LeadMode::pitchBend(int& pitchwheelVal, int fundamental , uint8_t &currentN, int midiNote)
{

	if (pitchwheelVal > 10000) {

		if (pitchwheelVal > 16000)
		{
			pitchwheelVal = 16000;
		}
		error = fundamental / guitarNoteArr[currentN - 40];
	}

	else {
		currentN = midiNoteArr[midiNote];
		error = fundamental / guitarNoteArr[midiNote];
	}
	pitchwheelVal = std::round(8192 + 4096 * 12 * log2(error));
	return 0;
}

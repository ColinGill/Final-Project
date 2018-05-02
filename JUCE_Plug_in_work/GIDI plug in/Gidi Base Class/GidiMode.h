/*
  ==============================================================================

    GidiMode.h
    Created: 22 Apr 2018 8:48:57pm
    Author:  Donnacha

  ==============================================================================
*/

#pragma once
#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"

class GidiMode
{
public:

    GidiMode();
    ~GidiMode();
	
	static const int autoCorrLength = 1024, numOfNotes = 49;
	int guitarNoteArr[numOfNotes], midiNoteArr[numOfNotes], noteRangeMin[numOfNotes], noteRangeMax[numOfNotes];

	void AutoCorr(float(&buffer)[autoCorrLength], const int samples, int olf, const int dsf);
	float getFundamental(const double sampleRate, const int dsf);
	virtual bool findChord(uint8_t & cNote, uint8_t & cNoteThird, uint8_t & dimin, int keySig);
	virtual int pitchBend(int& pitchwheelVal, int fundamental, uint8_t &currentN, int midiNote);

	template<typename T>
	bool mapToMidi(T f, uint8 &currentN, bool bends ,int &pitchWheel )
	{
		for (int i = 0; i < 49; i++)
		{	
			if (f <= noteRangeMax[i] && f >= noteRangeMin[i])
			{
				if (bends == true)
				{
					pitchBend(pitchWheel, f, currentN, i);
				}
				else
				{
					currentN = midiNoteArr[i];
				}
				return true;
				break;
			}
		}

		return false;
	}
	
private:

	//static const int numOfNotes = 49;
	std::vector<float> autoCorrOut;
};

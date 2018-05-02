/*
  ==============================================================================

    GidiMode.cpp
    Created: 22 Apr 2018 8:31:26pm
    Author:  Donnacha

  ==============================================================================
*/

#include "GidiMode.h"

GidiMode::GidiMode()
{
	float guitarNoteStart = 77.782;

	for (int i = 0; i < numOfNotes; i++)
	{
		midiNoteArr[i] = i + 40;
		guitarNoteStart = guitarNoteStart *1.059460968;
		guitarNoteArr[i] = guitarNoteStart;
		noteRangeMax[i] = std::round(guitarNoteArr[i] * 1.03);
		noteRangeMin[i] = std::round(guitarNoteArr[i] / 1.03);
	}
}


GidiMode::~GidiMode()
{
}


void GidiMode::AutoCorr(float(&buffer)[autoCorrLength],  const int  samples, int olf ,const int dsf)
{
	float sum = 0;
	autoCorrOut.clear();

	for (int n = 0; n < samples*olf; n = n + dsf)
	{

		for (int i = n; i < samples*olf; i = i + dsf)
		{
			sum = sum + (buffer[i] * buffer[i - n]);
		}

		autoCorrOut.push_back(sum);
		sum = 0;
	}
}

float GidiMode::getFundamental( const double sampleRate , const int dsf )
{
	float peak = 0, maxPeak = 0, fundamental = 0;

	for (int i = 2; i < autoCorrOut.size() - 1; i++)
	{

		if ((autoCorrOut.at(i) > autoCorrOut.at(i + 1)) && (autoCorrOut.at(i) > autoCorrOut.at(i - 1)))
		{
			if (autoCorrOut.at(i) > peak)
			{
				peak = autoCorrOut.at(i);
				maxPeak = i;
			}
		}
	}

	if ((autoCorrOut.at(0) / peak) >= 3)
	{
		fundamental = 0;
	}
	else
	{
		fundamental = (sampleRate/dsf) / maxPeak;
	}
	
	return fundamental;
}

bool GidiMode::findChord(uint8_t & cNote, uint8_t & cNoteThird, uint8_t & dimin, int keySig)
{
	return false;
}

int GidiMode::pitchBend(int& pitchwheelVal, int fundamental , uint8_t &currentN, int midiNote)
{
	return 0;
}

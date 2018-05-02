/*
  ==============================================================================

    ChordMode.h
    Created: 25 Apr 2018 11:00:08pm
    Author:  Donnacha

  ==============================================================================
*/

#pragma once

#include "GidiMode.h"

class ChordMode : public GidiMode
{
public:

	bool findChord(uint8_t & cNote, uint8_t & cNoteThird, uint8_t & dimin, int keySig) override;

private:

	const int intervals[7] = { 0,2,4,5,7,9,11 };
	


};
/*
  ==============================================================================

    ChordMode.cpp
    Created: 25 Apr 2018 10:59:57pm
    Author:  Donnacha

  ==============================================================================
*/

#include "ChordMode.h"

bool ChordMode::findChord(uint8_t & cNote, uint8_t & cNoteThird, uint8_t & dimin, int keySig)
{
	bool keyMatch = false;

	for (int i = 1; i <= 7; i++)
	{

		if ((cNote - ((40 + keySig - 1) - 12)) % 12 == intervals[i - 1])
		{
			dimin = 0;
			keyMatch = true;
			if (i - 1 == 0 || i - 1 == 3 || i - 1 == 4)
			{
				cNoteThird = cNote + 4;//major 3rd
				break;
			}
			else if (i - 1 == 1 || i - 1 == 2 || i - 1 == 5)
			{
				cNoteThird = cNote + 3;//minor 3rd
				break;
			}
			else
			{
				cNoteThird = cNote + 3;//diminished
				dimin = 1;
				break;
			}
		}
	}
	return keyMatch;
}

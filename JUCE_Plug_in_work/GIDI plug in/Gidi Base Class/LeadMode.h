/*
  ==============================================================================

    LeadMode.h
    Created: 1 May 2018 2:36:10pm
    Author:  Donnacha

  ==============================================================================
*/

#pragma once

#include "GidiMode.h"

class LeadMode : public GidiMode
{
private:
	 
	//variables
	 float error;

public:
	 //functions
	 int pitchBend(int& pitchwheelVal, int fundamental , uint8_t &currentN, int midiNote) override;
	 
};

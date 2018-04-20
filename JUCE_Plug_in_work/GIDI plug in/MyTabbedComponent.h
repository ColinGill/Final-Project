/*
==============================================================================

MyTabbedComponent.h
Created: 27 Mar 2018 5:28:49pm
Author:  Donnacha

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class MyTabbedComponent : public TabbedComponent
{
public:
	MyTabbedComponent(GidiPluginAudioProcessor &p) : processingReference(p), TabbedComponent(TabbedButtonBar::TabsAtTop)
	{
		
	}

	void currentTabChanged(int index, const String&) override;
	
	GidiPluginAudioProcessor &processingReference;
};

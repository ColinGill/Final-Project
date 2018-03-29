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
	MyTabbedComponent(GuiWorkAudioProcessor &p) : processingReference(p) ,TabbedComponent(TabbedButtonBar::TabsAtTop)
	{
		//TabChangedFunc = [](int) {};
	}

	void currentTabChanged(int index, const String&) override;
	/*	{
	TabChangedFunc(index);
	}
	*/
	//std::function<void(int)> TabChangedFunc;
	GuiWorkAudioProcessor &processingReference;
};

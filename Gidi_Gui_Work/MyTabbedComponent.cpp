/*
  ==============================================================================

    MyTabbedComponent.cpp
    Created: 27 Mar 2018 5:28:49pm
    Author:  Donnacha

  ==============================================================================
*/

#include "MyTabbedComponent.h"

void MyTabbedComponent::currentTabChanged(int index, const String &)
{
	float i = processingReference.getStuff();
	//processingReference.noteOnVel = 0.0;
	i++;
}

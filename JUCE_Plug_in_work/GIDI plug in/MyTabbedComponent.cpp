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
	processingReference.setMode(uint8(index));
	processingReference.setSensitivity(processingReference.getSensitivity(index),index);
	processingReference.setDynamics(processingReference.getDynamics(index), index);				
}

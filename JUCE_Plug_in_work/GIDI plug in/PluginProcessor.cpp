/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SinewaveSynth.h"

AudioProcessor* JUCE_CALLTYPE createPluginFilter();


//==============================================================================
JuceDemoPluginAudioProcessor::JuceDemoPluginAudioProcessor()
    : AudioProcessor (getBusesProperties())
{
    lastPosInfo.resetToDefault();

    // This creates our parameters. We'll keep some raw pointers to them in this class,
    // so that we can easily access them later, but the base class will take care of
    // deleting them for us.
    addParameter (gainParam  = new AudioParameterFloat ("Note",  "Note",           0.0f, 1.0f, 0.9f));
    addParameter (delayParam = new AudioParameterFloat ("delay", "Delay Feedback", 0.0f, 1.0f, 0.5f));

    initialiseSynth();
}

JuceDemoPluginAudioProcessor::~JuceDemoPluginAudioProcessor()
{
}

void JuceDemoPluginAudioProcessor::initialiseSynth()
{
    const int numVoices = 8;

    // Add some voices...
    for (int i = numVoices; --i >= 0;)
        synth.addVoice (new SineWaveVoice());

    // ..and give the synth a sound to play
    synth.addSound (new SineWaveSound());
}

//==============================================================================
bool JuceDemoPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // Only mono/stereo and input/output must have same layout
    const AudioChannelSet& mainOutput = layouts.getMainOutputChannelSet();
    const AudioChannelSet& mainInput  = layouts.getMainInputChannelSet();

    // input and output layout must either be the same or the input must be disabled altogether
    if (! mainInput.isDisabled() && mainInput != mainOutput)
        return false;

    // do not allow disabling the main buses
    if (mainOutput.isDisabled())
        return false;

    // only allow stereo and mono
    if (mainOutput.size() > 2)
        return false;

    return true;
}

AudioProcessor::BusesProperties JuceDemoPluginAudioProcessor::getBusesProperties()
{
    return BusesProperties().withInput  ("Input",  AudioChannelSet::stereo(), true)
                            .withOutput ("Output", AudioChannelSet::stereo(), true);
}

//==============================================================================
void JuceDemoPluginAudioProcessor::prepareToPlay (double newSampleRate, int /*samplesPerBlock*/)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate (newSampleRate);
    keyboardState.reset();

	guitarNoteStart = 77.782;

	for (int i = 0; i < numOfNotes; i++)
	{
		midiNoteArr[i] = i + 40;
		guitarNoteStart = guitarNoteStart *1.059460968;
		guitarNoteArr[i] = guitarNoteStart;
	}

	

    if (isUsingDoublePrecision())
    {
        delayBufferDouble.setSize (2, 12000);
        delayBufferFloat.setSize (1, 1);
    }
    else
    {
        delayBufferFloat.setSize (2, 12000);
        delayBufferDouble.setSize (1, 1);
    }

    reset();
}

void JuceDemoPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.reset();
}

void JuceDemoPluginAudioProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.
    delayBufferFloat.clear();
    delayBufferDouble.clear();
}

template <typename FloatType>
void JuceDemoPluginAudioProcessor::process (AudioBuffer<FloatType>& buffer,
                                            MidiBuffer& midiMessages,
                                            AudioBuffer<FloatType>& delayBuffer)
{
    const int numSamples = buffer.getNumSamples();

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, numSamples);

    // Now pass any incoming midi messages to our keyboard state object, and let it
    // add messages to the buffer if the user is clicking on the on-screen keys
    keyboardState.processNextMidiBuffer (midiMessages, 0, numSamples, true);

    // and now get our synth to process these midi events and generate its output.
    synth.renderNextBlock (buffer, midiMessages, 0, numSamples);

    // Apply our delay effect to the new output..
    applyDelay (buffer, midiMessages);

   // applyGain (buffer, delayBuffer); // apply our gain-change to the outgoing data..

    // Now ask the host for the current time so we can store it to be displayed later...
    updateCurrentTimeInfoFromHost();
}

template <typename FloatType>
void JuceDemoPluginAudioProcessor::applyGain (AudioBuffer<FloatType>& buffer, AudioBuffer<FloatType>& delayBuffer)
{
    ignoreUnused (delayBuffer);
    const float gainLevel = *gainParam;

    for (int channel = 0; channel < getTotalNumOutputChannels(); ++channel)
        buffer.applyGain (channel, 0, buffer.getNumSamples(), gainLevel);
}

template <typename FloatType>
void JuceDemoPluginAudioProcessor::applyDelay (AudioBuffer<FloatType>& buffer, MidiBuffer& midi)
{

    const int numSamples = buffer.getNumSamples();
    //const float delayLevel = *delayParam;
	float fund = 0;
	int delayPos = 0;
	auto sum = 0;
	

	for (int channel = 0; channel < getTotalNumOutputChannels(); ++channel)
	{
		
		auto channelData = buffer.getReadPointer(channel);
		delayPos = delayPosition;
		std::vector<float> autoCorrOut;

		float peak = 0, maxPeak = 0, sum = 0, power = 0;

		for (int samples = 0; samples < numSamples; samples++)
		{
			power = power + abs(channelData[samples]);
		}

		if (power < 5)
		{
			updateGui(String("Ready..."));
		}

		else
		{

			for (int n = 0; n < numSamples; n= n+dsf)
			{

				for (int i = n; i < numSamples; i = i+dsf)
				{
					sum = sum + (channelData[i] * channelData[i - n]);
				}

				autoCorrOut.push_back(sum);
				sum = 0;
			}

			for (int i = 2; i < autoCorrOut.size() - 1; i ++)
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

			fund = (44100/dsf) / maxPeak;
			int fundy = std::round(fund);
			autoCorrOut.clear();

			int max;
			int  min;
			bool noteFound = 0;

			for (int i = 0; i < 49; i++)
			{
				max = std::round(guitarNoteArr[i] * 1.03);
				min = std::round(guitarNoteArr[i] / 1.03);
				if (fundy <= max && fundy >= min)
				{
					noteFound = 1;
					currentNote = midiNoteArr[i];
					updateGui("midi note: " + String(midiNoteArr[i]));
					
					break;
				}

			}
			
		    if (noteFound == 0)
			{
				updateGui("unknown" + String(fundy));
				currentNote = lastNoteValue;
			}
			/*
			if (fundy < 112 && fundy > 108)
			{
				updateGui("A note");
				currentNote = 45;
			}
			
			else if (fundy < 84 && fundy > 80)
			{
				
				updateGui("E note");
				currentNote = 41;
			}

			*/
			
		}

		
	}
		
	auto noteDuration = 1024;
	MidiMessage msg;
	int ignore;
	midi.clear();

	if ((time + numSamples) >= noteDuration)
	{
		int r = (rand() % 60) + +60;
		auto offset = jmax(0, jmin((int)(noteDuration - time), numSamples - 1));

		if (lastNoteValue != currentNote)
		{

			midi.addEvent(MidiMessage::noteOff(1, lastNoteValue), offset);
			midi.addEvent(MidiMessage::noteOn(1, currentNote, (uint8)127), offset);
			lastNoteValue = currentNote;
			//midi.addEvent (MidiMessage::noteOff (1, lastNoteValue), offset);		
		}

		else 
		{

		}
		
	}

	time = (time + numSamples) % noteDuration;
	
}

void JuceDemoPluginAudioProcessor::updateCurrentTimeInfoFromHost()
{
    if (AudioPlayHead* ph = getPlayHead())
    {
        AudioPlayHead::CurrentPositionInfo newTime;

        if (ph->getCurrentPosition (newTime))
        {
            lastPosInfo = newTime;  // Successfully got the current time from the host..
            return;
        }
    }

    // If the host fails to provide the current time, we'll just reset our copy to a default..
    lastPosInfo.resetToDefault();
}

//==============================================================================
AudioProcessorEditor* JuceDemoPluginAudioProcessor::createEditor()
{
    return new JuceDemoPluginAudioProcessorEditor (*this);
}

//==============================================================================
void JuceDemoPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // Here's an example of how you can use XML to make it easy and more robust:

    // Create an outer XML element..
    XmlElement xml ("MYPLUGINSETTINGS");

    // add some attributes to it..
    xml.setAttribute ("uiWidth", lastUIWidth);
    xml.setAttribute ("uiHeight", lastUIHeight);

    // Store the values of all our parameters, using their param ID as the XML attribute
    for (auto* param : getParameters())
        if (auto* p = dynamic_cast<AudioProcessorParameterWithID*> (param))
            xml.setAttribute (p->paramID, p->getValue());

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xml, destData);
}

void JuceDemoPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr)
    {
        // make sure that it's actually our type of XML object..
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our last window size..
            lastUIWidth  = jmax (xmlState->getIntAttribute ("uiWidth", lastUIWidth), 400);
            lastUIHeight = jmax (xmlState->getIntAttribute ("uiHeight", lastUIHeight), 200);

            // Now reload our parameters..
            for (auto* param : getParameters())
                if (auto* p = dynamic_cast<AudioProcessorParameterWithID*> (param))
                    p->setValue ((float) xmlState->getDoubleAttribute (p->paramID, p->getValue()));
        }
    }
}

void JuceDemoPluginAudioProcessor::updateTrackProperties (const TrackProperties& properties)
{
    trackProperties = properties;

    if (auto* editor = dynamic_cast<JuceDemoPluginAudioProcessorEditor*> (getActiveEditor()))
        editor->updateTrackProperties ();
}

void JuceDemoPluginAudioProcessor::updateGui(String c)
{
	String text = c;
	if (auto* editor = dynamic_cast<JuceDemoPluginAudioProcessorEditor*> (getActiveEditor()))
		editor->updateGui(text);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuceDemoPluginAudioProcessor();
}

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
#include <chrono>
#include <ctime>
#include <math.h>  
#include "PluginProcessor.h"
#include "PluginEditor.h"


AudioProcessor* JUCE_CALLTYPE createPluginFilter();


//==============================================================================
GidiPluginAudioProcessor::GidiPluginAudioProcessor()
    : AudioProcessor (getBusesProperties())
{
   
    
}

GidiPluginAudioProcessor::~GidiPluginAudioProcessor()
{
}



//==============================================================================
bool GidiPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

AudioProcessor::BusesProperties GidiPluginAudioProcessor::getBusesProperties()
{
    return BusesProperties().withInput  ("Input",  AudioChannelSet::mono(), true)
                            .withOutput ("Output", AudioChannelSet::mono(), true);
}

//==============================================================================
void GidiPluginAudioProcessor::prepareToPlay (double newSampleRate, int /*samplesPerBlock*/)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
  
	guitarNoteStart = 77.782;

	for (int i = 0; i < numOfNotes; i++)
	{
		midiNoteArr[i] = i + 40;
		guitarNoteStart = guitarNoteStart *1.059460968;
		guitarNoteArr[i] = guitarNoteStart;
	}

	
    if (isUsingDoublePrecision())
    {
        BufferDouble.setSize (2, 12000);
        BufferFloat.setSize (1, 1);
    }
    else
    {
        BufferFloat.setSize (2, 12000);
        BufferDouble.setSize (1, 1);
    }

    reset();
}

void GidiPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void GidiPluginAudioProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.
    BufferFloat.clear();
    BufferDouble.clear();
}

template <typename FloatType>
void GidiPluginAudioProcessor::process (AudioBuffer<FloatType>& buffer,
                                            MidiBuffer& midiMessages,
                                            AudioBuffer<FloatType>& Buffer)
{
    const int numSamples = buffer.getNumSamples();
    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).

    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
       buffer.clear (i, 0, numSamples);

    // Apply effect to the new output..
    processBuffer(buffer, midiMessages);

}

template <typename FloatType>
void GidiPluginAudioProcessor::processBuffer (AudioBuffer<FloatType>& buffer, MidiBuffer& midi)
{
    const int numSamples = buffer.getNumSamples(), size = 1024;
	const int olf = size / numSamples;
	float fund = 0, RmsPower2 = 0, RmsPower1 = 0, maxValue =0 , AbsAvgPower1 =0 , AbsAvgPower2 =0;
	static bool BELOW_THRESHOLD = false;
	
	//std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	
	for (int channel = 0; channel < getTotalNumOutputChannels(); ++channel)
	{
		auto channelData = buffer.getReadPointer(channel);
		static std::vector<float> autoCorrOut;
		static float bufferSize_2[size] = { 0 };
		autoCorrOut.clear();
		
		
		for (int i = 0; i < (size / numSamples) - 1; i++)
		{
			for (int fillHalfBuffer = 0; fillHalfBuffer < numSamples; fillHalfBuffer++)
			{
				bufferSize_2[fillHalfBuffer + (numSamples * i)] = bufferSize_2[fillHalfBuffer + (numSamples* (i + 1))];
			}

		}
		for (int i = 0; i < numSamples; i++)
		{
			bufferSize_2[(size - numSamples) + i] = channelData[i];
		}

		for (int fillHalfBuffer = 0; fillHalfBuffer < size/2; fillHalfBuffer++)
		{
			AbsAvgPower1 = AbsAvgPower1 + abs(bufferSize_2[fillHalfBuffer]); //calculates sum of abs values in buffer
			RmsPower1 = RmsPower1 + (bufferSize_2[fillHalfBuffer] * bufferSize_2[fillHalfBuffer]);
		}

		for (int fillRemaining = size/2; fillRemaining < size; fillRemaining++)
		{			
			AbsAvgPower2 = AbsAvgPower2 + abs(bufferSize_2[fillRemaining]);
			RmsPower2 = RmsPower2 + (bufferSize_2[fillRemaining] * bufferSize_2[fillRemaining]);
		}

		//find max value in buffer (used for attack/velocity)
		for (int samples = 0; samples < size; samples++)
		{
			if ((bufferSize_2[samples]) > maxValue)
			{
				maxValue = abs(bufferSize_2[samples]);
			}
		}
		bufferSafe++;
		if (bufferSafe >= size/(numSamples*2))
		{
			bufferSafe = 0;
		}

		/*
			==============================================
			RIFF MODE CODE
			==============================================
		*/
		if (modeSelect == 0 && bufferSafe == 0)
		{
			const float sensitivityLevel = (1.01 - getSensitivity(modeSelect))*0.1; //get sensitivity level from editor thread
			const int dynamicsLevel= 1000 - (getDynamics(modeSelect) * 100);

			//check if power of either 512 chunk is less than the sensitivity threshold (set by user) 
			if (sqrt(RmsPower1 / 512) <= sensitivityLevel || sqrt(RmsPower2 / 512) <= sensitivityLevel)
			{
				BELOW_THRESHOLD = true;

				//if power is very low, ie: user has muted strings/stopped playing...
				if (AbsAvgPower2/512 <= 0.0005)
				{
					midi.addEvent(MidiMessage::noteOff(1, currentNote), 0);// send note Off msg 
				}
			}

			else
			{
				if (BELOW_THRESHOLD == true) //if previous chunk was below threshold...
				{
					//perform autoCorrelation and find fundamental pitch
					AutoCorr(bufferSize_2, autoCorrOut, numSamples, olf);
					fund = getFundamental(autoCorrOut);

					int fundy = std::round(fund);
					bool noteFound = mapToMidi(fundy,currentNote); //check if returned frequency can be mapped to a corresponding midi note

					if (noteFound == 0)
					{
						currentNote = lastNoteValue; //keep track of previous notes for note off midi msg
					}
	
					if (getGlobalDynamicsState() == false) { velocity = 127; }

					else
					{
						velocity = (int)((maxValue * dynamicsLevel)) * 2;

						if (velocity > 127)
						{
							velocity = 127;
						}
						else if (velocity < 0)
						{
							velocity = 0;
						}
					}

					midi.clear();
					midi.addEvent(MidiMessage::noteOff(1, lastNoteValue), 0);
					MidiMessage message = MidiMessage::noteOn(1, currentNote, (uint8)velocity);				
					midi.addEvent(message, 0);

					velocity = 0;
					lastNoteValue = currentNote;
					BELOW_THRESHOLD = false;
				}
			}
		}

		/*
		==============================================
		LEAD MODE CODE
		==============================================
		*/
		else if (modeSelect == 1 && bufferSafe == 0)
		{
			const float sensitivityLevel = (1.01 - getSensitivity(modeSelect))*0.01; //get sensitivity level from editor thread
			const int dynamicsLevel = 1000 - (getDynamics(modeSelect) * 100);

			
			if (AbsAvgPower2 / 512 <= sensitivityLevel || AbsAvgPower1 / 512 <= sensitivityLevel)
			{
				//midi.addEvent(MidiMessage::noteOff(1, currentNote), 0);// send note Off msg 
				PitchWheelVal = 8192;

				if (AbsAvgPower2 / 512 <= sensitivityLevel *0.1)
				{
					midi.addEvent(MidiMessage::noteOff(1, currentNote), 0);// send note Off msg 
					lastNoteValue = 0;
				}
					
			}
			
			else
			{
				//perform autoCorrelation and find fundamental pitch
				AutoCorr(bufferSize_2, autoCorrOut, numSamples, olf);
				fund = getFundamental(autoCorrOut);
				bool noteFound = mapToMidi(fund, currentNote); //check if returned frequency can be mapped to a corresponding midi note

			if (noteFound == true)
			{
				midi.clear();

				if (currentNote != lastNoteValue)
				{
					if (getGlobalDynamicsState() == false) { velocity = 127; }

					else
					{
						velocity = (int)((maxValue * dynamicsLevel)) * 2;

						if (velocity > 127)
						{
							velocity = 127;
						}
						else if (velocity < 0)
						{
							velocity = 0;
						}
					}

					if (PitchWheelVal > 10000) {}

					else
					{

						midi.addEvent(MidiMessage::noteOff(1, lastNoteValue), 0);
						midi.addEvent(MidiMessage::noteOn(1, currentNote, (uint8)velocity), 0);

						lastNoteValue = currentNote;
					}
					velocity = 0;
				}

				if (getBendBtnState() == true)
				{
					midi.addEvent(MidiMessage::pitchWheel(1, PitchWheelVal), 0);
				}
			}
		}


		}

		else if (modeSelect == 2 && bufferSafe == 0)
		{
			const float sensitivityLevel = (1.01 - getSensitivity(modeSelect))*0.1; //get sensitivity level from editor thread
			const int dynamicsLevel = 1000 - (getDynamics(modeSelect) * 100);

			//check if power of either 512 chunk is less than the sensitivity threshold (set by user) 
			if (sqrt(RmsPower1 / numSamples) <= sensitivityLevel || sqrt(RmsPower2 / numSamples) <= sensitivityLevel)
			{
				BELOW_THRESHOLD = true;

				//if power is very low, ie: user has muted strings/stopped playing...
				if (AbsAvgPower2 / 512 <= 0.0005)
				{
					midi.addEvent(MidiMessage::noteOff(1, currentNote), 0);// send note Off msg 
					midi.addEvent(MidiMessage::noteOff(1, currentNote+ (7- diminished)), 0);
					midi.addEvent(MidiMessage::noteOff(1, currentThird), 0);
				}
			}


			else
			{
				if (BELOW_THRESHOLD == true) //if previous chunk was below threshold...
				{
					//perform autoCorrelation and find fundamental pitch
					AutoCorr(bufferSize_2, autoCorrOut, numSamples, olf);
					fund = getFundamental(autoCorrOut);

					int fundy = std::round(fund);
					bool noteFound = mapToMidi(fundy, currentNote); //check if returned frequency can be mapped to a corresponding midi note

					if (noteFound == 0)
					{
						currentNote = lastNoteValue; //keep track of previous notes for note off midi msg
					}

					if (getGlobalDynamicsState() == false) { velocity = 127; }

					else
					{
						velocity = (int)((maxValue * dynamicsLevel)) * 2;

						if (velocity > 127)
						{
							velocity = 127;
						}
						else if (velocity < 0)
						{
							velocity = 0;
						}
					}

					midi.clear();
					bool keyMatch = false;

					for (int i = 1; i <= 7; i ++)
					{
						
						if ((currentNote-((40+ keySignature - 1)-12)) % 12 == intervals[i - 1])
						{
							diminished = 0;
							keyMatch = true;
							if (i - 1 == 0 || i - 1 == 3 || i - 1 == 4)
							{
								currentThird = currentNote + 4;//major 3rd
								break;
							}
							else if (i - 1 == 1 || i - 1 == 2 || i - 1 == 5)
							{
								currentThird = currentNote + 3;//minor 3rd
								break;
							}
							else
							{
								currentThird = currentNote + 3;//diminished
								diminished = 1;
								break;
							}
						}
					}
					if (keyMatch == true)
					{

						midi.addEvent(MidiMessage::noteOff(1, lastNoteValue + (7 - diminished)), 0);
						midi.addEvent(MidiMessage::noteOff(1, lastNoteValue), 0);
						midi.addEvent(MidiMessage::noteOff(1, lastThird), 0);
						midi.addEvent(MidiMessage::noteOn(1, currentNote, (uint8)velocity), 0);
						midi.addEvent(MidiMessage::noteOn(1, currentNote + (7-diminished), (uint8)velocity), 0); //5th is same most chords
						midi.addEvent(MidiMessage::noteOn(1, currentThird, (uint8)velocity), 0);
						velocity = 0;
						lastNoteValue = currentNote;
						lastThird = currentThird;
					}
					BELOW_THRESHOLD = false;
				}
			}
		}
		//std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		//std::chrono::duration<double> time_span = t2 - t1;
		//double time = time_span.count();
	}
		
}

void GidiPluginAudioProcessor::AutoCorr(float (& buffer)[autoCorrLength], std::vector<float>& autoCorr, const int  samples , int olf)
{
	float sum = 0;

	for (int n = 0; n < samples*olf; n = n + dsf)
	{

		for (int i = n; i < samples*olf; i = i + dsf)
		{
			sum = sum + (buffer[i] * buffer[i - n]);
		}

		autoCorr.push_back(sum);
		sum = 0;
	}
}

float GidiPluginAudioProcessor::getFundamental(std::vector<float>& autoCorrResult)
{
	float peak = 0, maxPeak = 0 , fundamental =0;

	for (int i = 2; i < autoCorrResult.size() - 1; i++)
	{

		if ((autoCorrResult.at(i) > autoCorrResult.at(i + 1)) && (autoCorrResult.at(i) > autoCorrResult.at(i - 1)))
		{
			if (autoCorrResult.at(i) > peak)
			{
				peak = autoCorrResult.at(i);
				maxPeak = i;
			}
		}
	}
	if ((autoCorrResult.at(0) / peak) >= 3)
	{
		fundamental = 0;
	}
	else
	{ 
		fundamental = (getSampleRate() / dsf) / maxPeak;
	}
	
	return fundamental;
}

template<typename T>
bool GidiPluginAudioProcessor::mapToMidi(T f, uint8 &currentN )
{	
	int max,min;
	
	
	for (int i = 0; i < 49; i++)
	{
		max = std::round(guitarNoteArr[i] * 1.03);
		min = std::round(guitarNoteArr[i] / 1.03);

		if (f <= max && f >= min)
		{	
			if (getBendBtnState() == true && modeSelect == 1)
			{
				if (PitchWheelVal > 10000) {

					if (PitchWheelVal > 16000)
					{
						PitchWheelVal = 16000;
					}
					error = f / guitarNoteArr[currentN - 40];
				}

				else {
					currentN = midiNoteArr[i];
					error = f / guitarNoteArr[i];
				}
				PitchWheelVal = std::round(8192 + 4096 * 12 * log2(error));
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

//==============================================================================
AudioProcessorEditor* GidiPluginAudioProcessor::createEditor()
{
    return new GuiWorkAudioProcessorEditor(*this);
}

//==============================================================================
void GidiPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // Here's an example of how you can use XML to make it easy and more robust:

    // Create an outer XML element..
    XmlElement xml ("MYPLUGINSETTINGS");

    // add some attributes to it..
   

    // Store the values of all our parameters, using their param ID as the XML attribute
   
    // then use this helper function to stuff it into the binary blob and return it..
   
}

void GidiPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
  
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GidiPluginAudioProcessor();
}

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

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <math.h>       



//==============================================================================
/**
    As the name suggest, this class does the actual audio processing.
*/
class GidiPluginAudioProcessor : public AudioProcessor
{
public:
    //==============================================================================
	GidiPluginAudioProcessor();
    ~GidiPluginAudioProcessor();

    //==============================================================================
	//Setters
	void setGlobalDynamicsEnable(bool dEnable) { globalDynamics = dEnable; };
	void setDynamics(int d , int i) { dynamics[i] = d; };
	void setMode(uint8 m) { modeSelect = m; };
	void setSensitivity(double s , int i) { sensitivity[i]= s; };
	void setBendBtnState(bool b) { Ebends = b; };
	void setKeySig(int k) { keySignature = k; };

	//Getters
	uint8 getMode() { return modeSelect; };
	int getDynamics(int i) { return dynamics[i]; };
	int getKeySig() { return keySignature; };
	double getSensitivity(int i ) { return sensitivity[i]; };
	bool getBendBtnState() { return Ebends; }
	bool getGlobalDynamicsState() { return globalDynamics; }

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void reset() override;

    //==============================================================================
    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override
    {
        jassert (! isUsingDoublePrecision());
        process (buffer, midiMessages, BufferFloat);
    }

    void processBlock (AudioBuffer<double>& buffer, MidiBuffer& midiMessages) override
    {
        jassert (isUsingDoublePrecision());
        process (buffer, midiMessages, BufferDouble);
    }

    //==============================================================================
    bool hasEditor() const override                                             { return true; }
	AudioProcessorEditor* createEditor() override;

    //==============================================================================
    const String getName() const override                                       { return JucePlugin_Name; }

    bool acceptsMidi() const override                                           { return true; }
    bool producesMidi() const override                                          { return true; }

    double getTailLengthSeconds() const override                                { return 0.0; }

    //==============================================================================
    int getNumPrograms() override                                               { return 0; }
    int getCurrentProgram() override                                            { return 0; }
    void setCurrentProgram (int /*index*/) override                             {}
    const String getProgramName (int /*index*/) override                        { return String(); }
    void changeProgramName (int /*index*/, const String& /*name*/) override     {}

    //==============================================================================
    void getStateInformation (MemoryBlock&) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
	AudioBuffer<float> BufferFloat;
	AudioBuffer<double> BufferDouble;

	static const int autoCorrLength = 1024;
	const int numOfNotes = 49, dsf = 1, intervals[7] = { 0,2,4,5,7,9,11 };
	bool Ebends = false, globalDynamics = false;
	float guitarNoteStart, rate, error = 1;
	double sensitivity[3] = { 0.8 , 0.8 , 0.8 };
	uint8 currentNote = 0, lastNoteValue, currentThird = 0, lastThird , modeSelect = 0;
	int guitarNoteArr[49], midiNoteArr[49], velocity, PitchWheelVal = 0, keySignature = 1, diminished = 0, bufferSafe = 0;
	double dynamics[3] = { 0.5 , 0.5 , 0.5 };
   
    template <typename FloatType>
    void process (AudioBuffer<FloatType>& buffer, MidiBuffer& midiMessages, AudioBuffer<FloatType>& Buffer);
  
    template <typename FloatType>
    void processBuffer (AudioBuffer<FloatType>&, MidiBuffer& midi);

	template <typename T>
	bool mapToMidi(T , uint8 &);

	void AutoCorr(float (&)[autoCorrLength], std::vector<float>&, const int  , int);
	float getFundamental(std::vector<float>&);
	
    static BusesProperties getBusesProperties();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GidiPluginAudioProcessor)
};

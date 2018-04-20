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
	void setDynamics(int d) { dynamics = d; };
	void setMode(uint8 m) { modeSelect = m; };
	void setSensitivity(double s) { sensitivity = s; };
	void setBendBtnState(bool b) { Ebends = b; };
	void setKeySig(int k) { keySignature = k; };

	//Getters
	uint8 getMode() { return modeSelect; };
	int getDynamics() { return dynamics; };
	int getKeySig() { return keySignature; };
	double getSensitivity() { return sensitivity; };
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
        process (buffer, midiMessages, delayBufferFloat);
    }

    void processBlock (AudioBuffer<double>& buffer, MidiBuffer& midiMessages) override
    {
        jassert (isUsingDoublePrecision());
        process (buffer, midiMessages, delayBufferDouble);
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

    //==============================================================================
    //void updateTrackProperties (const TrackProperties& properties) override;

	//void updateGui(String);

    //==============================================================================
    // These properties are public so that our editor component can access them
    // A bit of a hacky way to do it, but it's only a demo! Obviously in your own
    // code you'll do this much more neatly..

    // this is kept up to date with the midi messages that arrive, and the UI component
    // registers with it so it can represent the incoming messages
    MidiKeyboardState keyboardState;

    // this keeps a copy of the last set of time info that was acquired during an audio
    // callback - the UI component will read this and display it.
    AudioPlayHead::CurrentPositionInfo lastPosInfo;

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth = 400, lastUIHeight = 200;

    // Our parameters
    AudioParameterFloat* gainParam = nullptr;
    AudioParameterFloat* delayParam = nullptr;

    // Current track colour and name
    TrackProperties trackProperties;

private:
    //==============================================================================
	AudioBuffer<float> delayBufferFloat;
	AudioBuffer<double> delayBufferDouble;

	static const int autoCorrLength = 1024;
	const int numOfNotes = 49, dsf = 1;
	bool Ebends = false, globalDynamics = false;

	uint8 modeSelect = 0;
	int guitarNoteArr[49], midiNoteArr[49];
	const int intervals[7] = {0, 2,4,5,7,9,11 };
	
	float guitarNoteStart;
	int delayPosition = 0;
	uint8 currentNote = 0, lastNoteValue, currentThird = 0, lastThird;
	int dynamics = 0.5, velocity, PitchWheelVal = 0, keySignature = 1, diminished = 0;
	int time;
	float rate, sensitivity = 0.86, error = 1;
    int bufferSafe = 0;
	SortedSet<int> notes;
	Synthesiser synth;
	
    template <typename FloatType>
    void process (AudioBuffer<FloatType>& buffer, MidiBuffer& midiMessages, AudioBuffer<FloatType>& delayBuffer);
  
    template <typename FloatType>
    void processBuffer (AudioBuffer<FloatType>&, MidiBuffer& midi);

	template <typename T>
	bool mapToMidi(T , uint8 &);

	void AutoCorr(float (&)[autoCorrLength], std::vector<float>&, const int  , int);
	float getFundamental(std::vector<float>&);
	
    void initialiseSynth();
    static BusesProperties getBusesProperties();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GidiPluginAudioProcessor)
};

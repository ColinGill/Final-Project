/*
==============================================================================

This file was auto-generated!

It contains the basic framework code for a JUCE plugin editor.

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "MyTabbedComponent.h"
#include "GidiImages.h"


//==============================================================================
/**
*/

class GuiWorkAudioProcessorEditor : public AudioProcessorEditor, private Button::Listener


{

public:
	GuiWorkAudioProcessorEditor(GidiPluginAudioProcessor&);
	~GuiWorkAudioProcessorEditor();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;
	static const char* gidiBg_png;
	static const int gidiBg_pngSize;
	

private:

	MyTabbedComponent tab;
	GidiPluginAudioProcessor& processor;
	ToggleButton enableDynamics;
	Image cachedImage_gidiBg_png_1;
	Font font;

	void buttonClicked(Button * btnClicked) override;

	struct RiffMode : public Component, private Slider::Listener
	{

		RiffMode(GidiPluginAudioProcessor &p) : processingRef(p)
		{
			addAndMakeVisible(text);
			text.setText(TRANS("'Time to Chugg...'\n\n"
				"Riff mode allows for tremolo style playing, perfect for converting your favourite riffs into midi! "
				"A sensivity slider caters for different player styles (set it high if you play light, low if you play hard)."
				" The dynamics slider monitors the player's attack and maps it to the midi's velocity."
				" Set it low for minimum response, high for very a dynamic response\n"), NotificationType::dontSendNotification);
			text.setFont(Font("Pixellari", 14.00f, Font::plain));
			text.setJustificationType(Justification::centred);
			text.setEditable(false, false, false);
			text.setColour(Label::textColourId, Colour(0xf0ffffff));
			addAndMakeVisible(sensitivity);
			addAndMakeVisible(dynamics);
			addAndMakeVisible(slider1);
			addAndMakeVisible(slider2);
			slider1.setName("slider1");
			slider2.setName("slider2");
			slider1.addListener(this);
			slider2.addListener(this);
		}

		void sliderValueChanged(Slider * slider) override;
		void paint(Graphics &) override;

		void resized() override
		{
			slider1.setRange(0.0, 1.0, 0.01);
			slider1.setValue(processingRef.getSensitivity(0));
			slider1.setSize(80, 40);
			slider1.setBounds(30, 40, 200, 20);

			slider2.setRange(1.0, 10, 1);
			slider2.setValue(processingRef.getDynamics(0));
			slider2.setSize(80, 40);
			slider2.setBounds(30, 90, 200, 20);

			sensitivity.setText("SENSITIVITY", NotificationType::dontSendNotification);
			sensitivity.setBounds(120, 20, 150, 32);

			dynamics.setText("DYNAMICS", NotificationType::dontSendNotification);
			dynamics.setBounds(120, 70, 150, 32);

			text.setBounds(290, 20, 360, 200);
		}

		Slider slider1, slider2;
		Label sensitivity, dynamics,text;
		GidiPluginAudioProcessor &processingRef;

	};

	struct LeadModeComponents : public Component, private Slider::Listener, private ToggleButton::Listener
	{
		
		LeadModeComponents(GidiPluginAudioProcessor &p) : processingRef(p)
		{	
			addAndMakeVisible(text);
			text.setText(TRANS("'Time to Shred...'\n\n"
					"Lead mode allows for legato, tapping, sliding, vibrato and bending! To enable pitch bending simply click on the checkbox labelled \"Enable Bends\"\n"), NotificationType::dontSendNotification);
			text.setFont(Font("Pixellari", 14.00f, Font::plain));
			text.setJustificationType(Justification::centred);
			text.setEditable(false, false, false);
			text.setColour(Label::textColourId, Colour(0xf0ffffff));
			
			bendsBtn.setButtonText(TRANS("Enable Bends"));
			addAndMakeVisible(bendsBtn);		
			addAndMakeVisible(sensitivity);
			addAndMakeVisible(dynamics);
			addAndMakeVisible(slider1);
			addAndMakeVisible(slider2);

			slider1.setName("sSlider");
			slider2.setName("dSlider");
			bendsBtn.addListener(this);
			slider1.addListener(this);
			slider2.addListener(this);
		}

		void paint(Graphics&) override;
		void sliderValueChanged(Slider * slider) override;
		void buttonClicked(Button* buttonThatWasClicked) override;

		void resized() override
		{
			slider1.setRange(0.0, 1.0, 0.01);
			slider1.setValue(processingRef.getSensitivity(1));
			slider1.setSize(100, 50);
			slider1.setBounds(30, 40, 200, 20);

			slider2.setRange(1.0, 10, 1);
			slider2.setValue(processingRef.getDynamics(1));
			slider2.setSize(100, 50);
			slider2.setBounds(30, 100, 200, 20);

			sensitivity.setText("SENSITIVITY", NotificationType::dontSendNotification);
			sensitivity.setBounds(120, 20, 150, 32);

			dynamics.setText("DYNAMICS", NotificationType::dontSendNotification);
			dynamics.setBounds(120, 80, 150, 32);
			text.setBounds(310, 30, 300, 128);		
			bendsBtn.setBounds(90, 140, 150, 32);
		}

		Slider slider1, slider2;
		Label sensitivity, dynamics, text;
		GidiPluginAudioProcessor &processingRef;
		ToggleButton bendsBtn;
		
	};

	
	struct ChordModeComponent : public Component, private Slider::Listener
	{

		ChordModeComponent(GidiPluginAudioProcessor &p) : processingRef(p)
		{

			const char* chordWheel_png = chordWheel_png = (const char*)resource_NewComponent_chordWheel_png;
			const int chordWheel_pngSize = 2582;

			addAndMakeVisible(chordWheel);
			addAndMakeVisible(text);
			text.setText(TRANS("'Time to Compose...'\n\n"
				"Chord mode turns single notes into rich beautiful chords! Choose your Key Signature from the Key Wheel."
				"Any notes that correspond to a wrong chord will be ignored!"), NotificationType::dontSendNotification);
			text.setFont(Font("Pixellari", 14.00f, Font::plain));
			text.setJustificationType(Justification::centred);
			text.setEditable(false, false, false);
			text.setColour(Label::textColourId, Colour(0xf0ffffff));
			addAndMakeVisible(sensitivity);
			addAndMakeVisible(dynamics);
			addAndMakeVisible(slider1);
			addAndMakeVisible(slider2);
			chordWheel.setName("chordWheel");
			slider1.setName("Sslider");
			slider2.setName("Dslider");
			chordWheel.setValue(p.getKeySig());
			chordWheel.setRange(1, 12, 1);
			chordWheel.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
			chordWheel.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
			chordWheel.addListener(this);
			slider1.addListener(this);
			slider2.addListener(this);
			chordWheelpng = ImageCache::getFromMemory(chordWheel_png, chordWheel_pngSize);
		}

		void sliderValueChanged(Slider * slider) override;
		void paint(Graphics & g)override;

		void resized() override
		{
			slider1.setRange(0.0, 1.0, 0.01);
			slider1.setValue(processingRef.getSensitivity(2));
			slider1.setSize(80, 40);
			slider1.setBounds(10, 40, 200, 20);

			slider2.setRange(1.0, 10, 1);
			slider2.setValue(processingRef.getDynamics(2));
			slider2.setSize(80, 40);
			slider2.setBounds(10, 140, 200, 20);
			text.setSize(200, 200);
			text.setBounds(430, 50, 200, 128);
			chordWheel.setBounds(240, 34, 160, 150);

			sensitivity.setText("SENSITIVITY", NotificationType::dontSendNotification);
			sensitivity.setBounds(100, 20, 150, 32);

			dynamics.setText("DYNAMICS", NotificationType::dontSendNotification);
			dynamics.setBounds(100, 120, 150, 32);
		}

		Slider slider1, slider2 , chordWheel;
		Label sensitivity, dynamics,text;
		GidiPluginAudioProcessor &processingRef;
		Image chordWheelpng;

		static const char* chordWheel_png;
		static const int chordWheel_pngSize; 

	};

};

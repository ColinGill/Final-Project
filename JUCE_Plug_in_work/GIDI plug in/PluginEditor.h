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
	static const char* riffmode_png;
	static const int riffmode_pngSize;


private:

	
	MyTabbedComponent tab;
	GidiPluginAudioProcessor& processor;
	ToggleButton enableDynamics;
	Image cachedImage_gidiBg_png_1;
	Image cachedImage_riffmode_png_1;
	Font font;

	void buttonClicked(Button * btnClicked) override;

	struct SliderHolder : public Component, private Slider::Listener
	{

		SliderHolder(GidiPluginAudioProcessor &p) : processingRef(p)
		{
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


		void resized() override
		{
			slider1.setRange(0.0, 1.0, 0.01);
			slider1.setValue(processingRef.getSensitivity());
			slider1.setSize(100, 50);
			slider1.setBounds(30, 40, 200, 40);

			slider2.setRange(1.0, 10, 1);
			slider2.setValue(processingRef.getDynamics());
			slider2.setSize(100, 50);
			slider2.setBounds(30, 120, 200, 40);

			sensitivity.setText("SENSITIVITY", NotificationType::dontSendNotification);
			sensitivity.setBounds(120, 20, 150, 32);

			dynamics.setText("DYNAMICS", NotificationType::dontSendNotification);
			dynamics.setBounds(120, 100, 150, 32);
		}

		Slider slider1, slider2;
		Label sensitivity, dynamics;
		GidiPluginAudioProcessor &processingRef;

	};

	struct LeadModeComponents : public Component, private Slider::Listener, private ToggleButton::Listener
	{
		
		LeadModeComponents(GidiPluginAudioProcessor &p) : processingRef(p)
		{
			addAndMakeVisible(bendsBtn);
			bendsBtn.setButtonText(TRANS("Enable Bends"));
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

		void sliderValueChanged(Slider * slider) override;
		void buttonClicked(Button* buttonThatWasClicked) override;

		void resized() override
		{
			slider1.setRange(0.0, 1.0, 0.01);
			slider1.setValue(processingRef.getSensitivity());
			slider1.setSize(100, 50);
			slider1.setBounds(30, 40, 200, 40);

			slider2.setRange(1.0, 10, 1);
			slider2.setValue(processingRef.getDynamics());
			slider2.setSize(100, 50);
			slider2.setBounds(30, 120, 200, 40);

			sensitivity.setText("SENSITIVITY", NotificationType::dontSendNotification);
			sensitivity.setBounds(120, 20, 150, 32);

			dynamics.setText("DYNAMICS", NotificationType::dontSendNotification);
			dynamics.setBounds(120, 100, 150, 32);
			
			bendsBtn.setBounds(100, 180, 150, 32);
		}

		Slider slider1, slider2;
		Label sensitivity, dynamics;
		GidiPluginAudioProcessor &processingRef;
		ToggleButton bendsBtn;

	};



};

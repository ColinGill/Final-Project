/*
==============================================================================

This file was auto-generated!

It contains the basic framework code for a JUCE plugin editor.

==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "GidiImages.h"


//==============================================================================
GuiWorkAudioProcessorEditor::GuiWorkAudioProcessorEditor(GidiPluginAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p), tab(p)
{

	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	getLookAndFeel().setDefaultSansSerifTypefaceName("Aharoni");
	getLookAndFeel().setColour(Slider::thumbColourId, Colours::sandybrown);
	
	enableDynamics.setButtonText(TRANS("Enable Dynamics"));
	enableDynamics.addListener(this);
	enableDynamics.setToggleState(true, dontSendNotification);
	enableDynamics.setColour(ToggleButton::textColourId, Colour(0xC0ffffff));
	addAndMakeVisible(enableDynamics);

	addAndMakeVisible(tab);
	tab.setTabBarDepth(36);
	tab.addTab(".                                .", Colour(0x30004cFF), new SliderHolder(processor), true);
	tab.addTab(".    LEAD MODE               .", Colour(0x30FF0000), new LeadModeComponents(processor), true);
	tab.addTab("     CHORD MODE      ", Colour(0x60004c87), new LeadModeComponents(processor), true);
	//tab.addTab("About This Software", Colour(0x60004c87), &midiVolume ,true);
	tab.setCurrentTabIndex(0);

	cachedImage_gidiBg_png_1 = ImageCache::getFromMemory(gidiBg_png, gidiBg_pngSize);
	cachedImage_riffmode_png_1 = ImageCache::getFromMemory(riffmode_png, riffmode_pngSize);

	setSize(700, 400);
}

GuiWorkAudioProcessorEditor::~GuiWorkAudioProcessorEditor()
{

}

//==============================================================================
void GuiWorkAudioProcessorEditor::paint(Graphics& g)
{
	g.fillAll(Colour(0xff1f5a77));


	{
		int x = 0, y = 0, width = 700, height = 400;
		
		g.setColour(Colours::black);
		g.drawImage(cachedImage_gidiBg_png_1,
			x, y, width, height,
			0, 0, cachedImage_gidiBg_png_1.getWidth(), cachedImage_gidiBg_png_1.getHeight());
	}
	{
		int x = 50, y = 80, width = 200, height = 32;
		
		g.setColour(Colours::black);
		g.drawImage(cachedImage_riffmode_png_1,
			x, y, width, height,
			0, 0, cachedImage_riffmode_png_1.getWidth(), cachedImage_riffmode_png_1.getHeight());
	}


}


void GuiWorkAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
	enableDynamics.setBounds(10, 370, 150, 32);
	tab.setBounds(50, 80, 600, 280);
}


//button for global dynamics enable
void GuiWorkAudioProcessorEditor::buttonClicked(Button * btnClicked)
{
	static bool state = true;
	if (btnClicked->getButtonText() == "Enable Dynamics")
	{
		state = !state;
		processor.setGlobalDynamicsEnable(state);
	}
}


void GuiWorkAudioProcessorEditor::SliderHolder::sliderValueChanged(Slider * slider)
{

	if (slider->getName() == "slider1")
	{
		processingRef.setSensitivity(slider->getValue());
	}
	else if (slider->getName() == "slider2")
	{
		processingRef.setDynamics(int(slider->getValue()));
	}
}

void GuiWorkAudioProcessorEditor::LeadModeComponents::sliderValueChanged(Slider * slider)
{
	if (slider->getName() == "sSlider")
	{
		processingRef.setSensitivity(slider->getValue());
	}
	else if (slider->getName() == "dSlider")
	{
		processingRef.setDynamics(int(slider->getValue()));
	}
}

void GuiWorkAudioProcessorEditor::LeadModeComponents::buttonClicked(Button * buttonThatWasClicked)
{
	static bool state = false;

	if (buttonThatWasClicked->getButtonText() == "Enable Bends")
	{	
		state = !state;
		processingRef.setBendBtnState(state);
	}

}

const char* GuiWorkAudioProcessorEditor::gidiBg_png = (const char*)resource_NewComponent_gidiBg_png;
const int GuiWorkAudioProcessorEditor::gidiBg_pngSize = 682305;


const char* GuiWorkAudioProcessorEditor::riffmode_png = (const char*)resource_guicomponent_riffmode_png;
const int GuiWorkAudioProcessorEditor::riffmode_pngSize = 620;


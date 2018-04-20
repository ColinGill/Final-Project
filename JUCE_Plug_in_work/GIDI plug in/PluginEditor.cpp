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
	enableDynamics.setToggleState(p.getGlobalDynamicsState(), dontSendNotification);
	enableDynamics.setColour(ToggleButton::textColourId, Colour(0xC0ffffff));
	addAndMakeVisible(enableDynamics);

	int i = p.getMode();	
	tab.setTabBarDepth(40);
	tab.addTab(".                                      .", Colour(0x60004cFF), new RiffMode(processor), true);
	tab.addTab(".                                      .", Colour(0x60ffda00), new LeadModeComponents(processor), true);
	tab.addTab(".                                      .", Colour(0x60ff6d00), new ChordModeComponent(processor), true);
	addAndMakeVisible(tab);
	tab.setCurrentTabIndex(i);

	cachedImage_gidiBg_png_1 = ImageCache::getFromMemory(gidiBg_png, gidiBg_pngSize);
	setSize(700, 400);
}

GuiWorkAudioProcessorEditor::~GuiWorkAudioProcessorEditor()
{

}

//==============================================================================
void GuiWorkAudioProcessorEditor::paint(Graphics& g)
{
	g.fillAll(Colour(0xff1f5a77));
	//enableDynamics.setToggleState()

	{
		int x = 0, y = 0, width = 700, height = 400;
		
		g.setColour(Colours::black);
		g.drawImage(cachedImage_gidiBg_png_1,
			x, y, width, height,
			0, 0, cachedImage_gidiBg_png_1.getWidth(), cachedImage_gidiBg_png_1.getHeight());
	}
	
}


void GuiWorkAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
	enableDynamics.setBounds(530, 368, 150, 32);
	tab.setBounds(22, 130, 652, 230);
}


//button for global dynamics enable
void GuiWorkAudioProcessorEditor::buttonClicked(Button * btnClicked)
{	
		static bool state = processor.getGlobalDynamicsState();
		state = !state;
		processor.setGlobalDynamicsEnable(state);	
}


void GuiWorkAudioProcessorEditor::RiffMode::sliderValueChanged(Slider * slider)
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

void GuiWorkAudioProcessorEditor::RiffMode::paint(Graphics & g)
{
	{
		int x = 380, y = 10, width = 172, height = 30;
		String text(TRANS("RIFF MODE"));
		Colour fillColour = Colour(0xfff2f2f2);
		//[UserPaintCustomArguments] Customize the painting arguments here..
		//[/UserPaintCustomArguments]
		g.setColour(fillColour);
		g.setFont(Font("8BIT WONDER Nominal", 18.00f, Font::plain).withTypefaceStyle("Regular"));
		g.drawText(text, x, y, width, height,
			Justification::centred, true);
	}
}

void GuiWorkAudioProcessorEditor::LeadModeComponents::paint(Graphics & g)
{
	{
		int x = 20, y = 20, width = 264, height = 50;
		Colour strokeColour = Colour(0xdde2e2e2);
		//[UserPaintCustomArguments] Customize the painting arguments here..
		//[/UserPaintCustomArguments]
		g.setColour(strokeColour);
		g.drawRect(x, y, width, height, 3);
		g.drawRect(x, y + 60, width, height, 3);
		g.drawRect(x, y + 120, width, height-20, 3);
	}
	
    {
        int x = 380, y = 10, width = 172, height = 30;
        String text (TRANS("LEAD MODE"));
        Colour fillColour = Colour (0xfff2f2f2);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font ("8BIT WONDER Nominal", 18.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);	
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

void GuiWorkAudioProcessorEditor::ChordModeComponent::sliderValueChanged(Slider * slider)
{
	if (slider->getName() == "chordWheel")
	{
		processingRef.setKeySig(slider->getValue());
	}
}

void GuiWorkAudioProcessorEditor::ChordModeComponent::paint(Graphics & g)
{
	
	{
		int x = 230, y = 10, width = 176, height = 176;
		
		g.drawImage(chordWheelpng,
			x, y, width, height,
			0, 0, chordWheelpng.getWidth(), chordWheelpng.getHeight());
	}
	{
		int x = 430, y = 10, width = 200, height = 30;
		String text(TRANS("CHORD MODE"));
		Colour fillColour = Colour(0xfff2f2f2);
		
		g.setColour(fillColour);
		g.setFont(Font("8BIT WONDER Nominal", 18.00f, Font::plain).withTypefaceStyle("Regular"));
		g.drawText(text, x, y, width, height,
			Justification::centred, true);
	}
	{
		int x = 6, y = 14, width = 210, height = 50;
		Colour strokeColour = Colour(0xdde2e2e2);
		
		g.setColour(strokeColour);
		g.drawRect(x, y, width, height, 3);
		g.drawRect(x, y + 102, width, height, 3);
		g.drawRect(x+220, y -8 , width+200, height +130, 3);
	}

	
}


const char* GuiWorkAudioProcessorEditor::gidiBg_png = (const char*)resource_NewComponent_gidiUi_jpg;
const int GuiWorkAudioProcessorEditor::gidiBg_pngSize = 682305;



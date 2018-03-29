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
GuiWorkAudioProcessorEditor::GuiWorkAudioProcessorEditor (GuiWorkAudioProcessor& p )
    : AudioProcessorEditor (&p), processor (p) , tab(p)
{
	
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	getLookAndFeel().setDefaultSansSerifTypefaceName("Aharoni");
	getLookAndFeel().setColour(Slider::thumbColourId, Colours::sandybrown);
	
	
	addAndMakeVisible(tab);
	tab.setTabBarDepth(36);	
	
	tab.addTab(".                                .", Colour(0x30004cFF), new SliderHolder(processor)  , true);
	tab.addTab(".    LEAD MODE               .", Colour(0x30FF0000), &altSlider, true);
	tab.addTab("     CHORD MODE      ", Colour(0x60004c87), &midiVolume , true);
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
void GuiWorkAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(Colour(0xff1f5a77));
	
	
	{
		int x = 0, y = 0, width = 700, height = 400;
		//[UserPaintCustomArguments] Customize the painting arguments here..
		//[/UserPaintCustomArguments]
		g.setColour(Colours::black);
		g.drawImage(cachedImage_gidiBg_png_1,
			x, y, width, height,
			0, 0, cachedImage_gidiBg_png_1.getWidth(), cachedImage_gidiBg_png_1.getHeight());
	}
	{
		int x = 50, y = 80, width = 200,  height = 32;
		//[UserPaintCustomArguments] Customize the painting arguments here..
		//[/UserPaintCustomArguments]
		g.setColour(Colours::black);
		g.drawImage(cachedImage_riffmode_png_1,
			x, y, width, height,
			0, 0, cachedImage_riffmode_png_1.getWidth(), cachedImage_riffmode_png_1.getHeight());
	}

		
}
    // (Our component is opaque, so we must completely fill the background with a solid colour)
	/*
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
	g.fillAll(Colours::white);
	// set the current drawing colour to black
	g.setColour(Colours::black);
	// set the font size and draw text to the screen
	g.setFont(15.0f);
	g.drawFittedText("Midi Volume", 0, 0, getWidth(), 30, Justification::centred, 1);
	*/
	

void GuiWorkAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	
	tab.setBounds(50, 80, 600, 300);
	//midiVolume.setBounds(40, 30, 20, getHeight() - 60);
	
	// fill the whole window white

	
}




void GuiWorkAudioProcessorEditor::sliderValueChanged(Slider * slider)
{
	
	processor.noteOnVel = midiVolume.getValue();

}


void GuiWorkAudioProcessorEditor::SliderHolder::sliderValueChanged(Slider * slider)
{
	
	if (slider->getName() == "slider1")
	{
		float i = processingRef.getStuff();
		 i++;
	}
	else if (slider->getName() == "slider2")
	{
	
	}
	
	
}


const char* GuiWorkAudioProcessorEditor::gidiBg_png = (const char*)resource_NewComponent_gidiBg_png;
const int GuiWorkAudioProcessorEditor::gidiBg_pngSize = 682305;


const char* GuiWorkAudioProcessorEditor::riffmode_png = (const char*)resource_guicomponent_riffmode_png;
const int GuiWorkAudioProcessorEditor::riffmode_pngSize = 620;

/*
void MyTabComponent::currentTabChanged(int index, const String &)
{
	int i = 0;
}
*/
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ArpegiatorAudioProcessorEditor::ArpegiatorAudioProcessorEditor (ArpegiatorAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (580, 200);
    
    speedLabel.setText("Speed: ", dontSendNotification);
    addAndMakeVisible(&speedLabel);
    speedSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    speedSlider.setRange(0.f, 1.f);
    speedSlider.setValue(0.1f);
    addAndMakeVisible(&speedSlider);
    
    speedSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "speed", speedSlider);

    
    for(int i = 0; sizeof(noteSelectors)/sizeof(*noteSelectors) > i; i++)
    {
        noteSelectors[i].addItem("C", 1);
        noteSelectors[i].addItem("C#", 2);
        noteSelectors[i].addItem("D", 3);
        noteSelectors[i].addItem("D#", 4);
        noteSelectors[i].addItem("E", 5);
        noteSelectors[i].addItem("F", 6);
        noteSelectors[i].addItem("F#", 7);
        noteSelectors[i].addItem("G", 8);
        noteSelectors[i].addItem("G#", 9);
        noteSelectors[i].addItem("A", 10);
        noteSelectors[i].addItem("A#", 11);
        noteSelectors[i].addItem("B", 12);
        
        noteSelectors[i].setSelectedId(1);
        noteSelectors[i].setColour (ComboBox::outlineColourId, Colours::transparentBlack);
        addAndMakeVisible (noteSelectors[i]);
        
        String indexString = std::to_string(i);
        String attachmentId = "note" + indexString;
        noteSelectorAtachments[i] = new AudioProcessorValueTreeState::ComboBoxAttachment(processor.tree, attachmentId, noteSelectors[i]);
    }
    
    for(int i = 0; sizeof(octaveSelectors)/sizeof(*octaveSelectors) > i; i++)
    {
        octaveSelectors[i].addItem("-2", 1);
        octaveSelectors[i].addItem("-1", 2);
        octaveSelectors[i].addItem("0", 3);
        octaveSelectors[i].addItem("1", 4);
        octaveSelectors[i].addItem("2", 5);
        octaveSelectors[i].addItem("3", 6);
        octaveSelectors[i].addItem("4", 7);
        octaveSelectors[i].addItem("5", 8);
        octaveSelectors[i].addItem("6", 9);
        octaveSelectors[i].addItem("7", 10);

        octaveSelectors[i].setSelectedId(5);
        octaveSelectors[i].setColour (ComboBox::outlineColourId, Colours::transparentBlack);
        addAndMakeVisible (octaveSelectors[i]);
        
        String indexString = std::to_string(i);
        String attachmentId = "octave" + indexString;
        octaveSelectorAttachments[i] = new AudioProcessorValueTreeState::ComboBoxAttachment(processor.tree, attachmentId, octaveSelectors[i]);
    }
    
    for(int i = 0; sizeof(noteActiveButtons)/sizeof(*noteActiveButtons) > i; i++)
    {
        noteActiveButtons[i].setButtonText ("Active");
        noteActiveButtons[i].setToggleState (true, dontSendNotification);
        
        addAndMakeVisible (noteActiveButtons[i]);
        
        String indexString = std::to_string(i);
        String attachmentId = "noteActive" + indexString;
        noteActiveAttachments[i] = new AudioProcessorValueTreeState::ButtonAttachment(processor.tree, attachmentId,  noteActiveButtons[i]);
    }

    
}

ArpegiatorAudioProcessorEditor::~ArpegiatorAudioProcessorEditor()
{
}

//==============================================================================
void ArpegiatorAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void ArpegiatorAudioProcessorEditor::resized()
{
    speedLabel.setBounds(15, 0, 100, 25);
    speedSlider.setBounds(15, 30, 250, 25);
    speedSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "speed", speedSlider);

    
    int paintGuiELementsCursor = 15;
    int space = 70;
    
    
    for(int i = 0; sizeof(octaveSelectors)/sizeof(*octaveSelectors) > i; i++)
    {
        noteActiveButtons[i].setBounds(paintGuiELementsCursor, 70, 90, 18);
        paintGuiELementsCursor = paintGuiELementsCursor + space;
    }
    
    paintGuiELementsCursor = 15;
    
    for(int i = 0; sizeof(noteSelectors)/sizeof(*noteSelectors) > i; i++)
    {
        noteSelectors[i].setBounds(paintGuiELementsCursor, 100, 60, 30);
        paintGuiELementsCursor = paintGuiELementsCursor + space;
    }
    
    paintGuiELementsCursor = 15;
    
    for(int i = 0; sizeof(octaveSelectors)/sizeof(*octaveSelectors) > i; i++)
    {
        octaveSelectors[i].setBounds(paintGuiELementsCursor, 140, 60, 30);
        paintGuiELementsCursor = paintGuiELementsCursor + space;
    }
}

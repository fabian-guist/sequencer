#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class ArpegiatorAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    ArpegiatorAudioProcessorEditor (ArpegiatorAudioProcessor&);
    ~ArpegiatorAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ArpegiatorAudioProcessor& processor;
    
    Slider speedSlider;
    
    Label speedLabel;
    
    ComboBox noteSelectors[8];
    ComboBox octaveSelectors[8];
    ToggleButton noteActiveButtons[8];
    
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> speedSliderAttachment;
    ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> noteSelectorAtachments[8];
    ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> octaveSelectorAttachments[8];
    ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> noteActiveAttachments[8];


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArpegiatorAudioProcessorEditor)
};

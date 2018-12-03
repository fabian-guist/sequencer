#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ArpegiatorAudioProcessor::ArpegiatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
tree (*this, nullptr)
#endif
{
    NormalisableRange<float> speedParam (0.f, 1.f);
    NormalisableRange<float> noteActive (0, 1);
    NormalisableRange<float> noteParam (0, 12);
    NormalisableRange<float> octaveParam (0, 10);
    
    tree.createAndAddParameter("speed", "speed", "speed", speedParam, 0.2f, nullptr, nullptr);
    
    tree.createAndAddParameter("noteActive0", "noteActive0", "noteActive0", noteParam, 1, nullptr, nullptr);
    tree.createAndAddParameter("noteActive1", "noteActive1", "noteActive1", noteParam, 1, nullptr, nullptr);
    tree.createAndAddParameter("noteActive2", "noteActive2", "noteActive2", noteParam, 1, nullptr, nullptr);
    tree.createAndAddParameter("noteActive3", "noteActive3", "noteActive3", noteParam, 1, nullptr, nullptr);
    tree.createAndAddParameter("noteActive4", "noteActive4", "noteActive4", noteParam, 1, nullptr, nullptr);
    tree.createAndAddParameter("noteActive5", "noteActive5", "noteActive5", noteParam, 1, nullptr, nullptr);
    tree.createAndAddParameter("noteActive6", "noteActive6", "noteActive6", noteParam, 1, nullptr, nullptr);
    tree.createAndAddParameter("noteActive7", "noteActive7", "noteActive7", noteParam, 1, nullptr, nullptr);
    tree.createAndAddParameter("noteActive8", "noteActive8", "noteActive8", noteParam, 1, nullptr, nullptr);
    tree.createAndAddParameter("noteActive9", "noteActive9", "noteActive9", noteParam, 1, nullptr, nullptr);
    tree.createAndAddParameter("noteActive10", "noteActive10", "noteActive10", noteParam, 1, nullptr, nullptr);
    tree.createAndAddParameter("noteActive11", "noteActive11", "noteActive11", noteParam, 1, nullptr, nullptr);
    
    tree.createAndAddParameter("note0", "note0", "note0", noteParam, 0, nullptr, nullptr);
    tree.createAndAddParameter("note1", "note1", "note1", noteParam, 0, nullptr, nullptr);
    tree.createAndAddParameter("note2", "note3", "note2", noteParam, 0, nullptr, nullptr);
    tree.createAndAddParameter("note3", "note4", "note3", noteParam, 0, nullptr, nullptr);
    tree.createAndAddParameter("note4", "note5", "note4", noteParam, 0, nullptr, nullptr);
    tree.createAndAddParameter("note5", "note6", "note5", noteParam, 0, nullptr, nullptr);
    tree.createAndAddParameter("note6", "note7", "note6", noteParam, 0, nullptr, nullptr);
    tree.createAndAddParameter("note7", "note8", "note7", noteParam, 0, nullptr, nullptr);
    tree.createAndAddParameter("note8", "note9", "note8", noteParam, 0, nullptr, nullptr);
    tree.createAndAddParameter("note9", "note10", "note9", noteParam, 0, nullptr, nullptr);
    tree.createAndAddParameter("note10", "note11", "note10", noteParam, 0, nullptr, nullptr);
    tree.createAndAddParameter("note11", "note12", "note11", noteParam, 0, nullptr, nullptr);
    
    tree.createAndAddParameter("octave0", "octave0", "octave0", octaveParam, 5, nullptr, nullptr);
    tree.createAndAddParameter("octave1", "octave1", "octave1", octaveParam, 5, nullptr, nullptr);
    tree.createAndAddParameter("octave2", "octave2", "octave2", octaveParam, 5, nullptr, nullptr);
    tree.createAndAddParameter("octave3", "octave3", "octave3", octaveParam, 5, nullptr, nullptr);
    tree.createAndAddParameter("octave4", "octave4", "octave4", octaveParam, 5, nullptr, nullptr);
    tree.createAndAddParameter("octave5", "octave5", "octave5", octaveParam, 5, nullptr, nullptr);
    tree.createAndAddParameter("octave6", "octave6", "octave6", octaveParam, 5, nullptr, nullptr);
    tree.createAndAddParameter("octave7", "octave7", "octave7", octaveParam, 5, nullptr, nullptr);
    tree.createAndAddParameter("octave8", "octave8", "octave8", octaveParam, 5, nullptr, nullptr);

    tree.state = ValueTree("StepSequencerTree");
}

ArpegiatorAudioProcessor::~ArpegiatorAudioProcessor()
{
}

//==============================================================================
const String ArpegiatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ArpegiatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ArpegiatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ArpegiatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ArpegiatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ArpegiatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ArpegiatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ArpegiatorAudioProcessor::setCurrentProgram (int index)
{
}

const String ArpegiatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void ArpegiatorAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ArpegiatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    ignoreUnused(samplesPerBlock);
    
    currentNote = startNote;
    
    time = 0.0;
    rate = static_cast<float> (sampleRate);
    
    stepCursor = 0;
}

void ArpegiatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ArpegiatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

bool noteDown = false;

void ArpegiatorAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midi)
{
    
    speed = tree.getRawParameterValue("speed")[0];
    
    int currentNotesActiveState[8];
    for(int i = 0; sizeof(currentNotesActiveState)/sizeof(*currentNotesActiveState) > i; i++)
    {
        String indexString = std::to_string(i);
        String noteActiveId = "noteActive" + indexString;
        
        currentNotesActiveState[i] = static_cast<int>(tree.getRawParameterValue(noteActiveId)[0]);
        
    }
    
   int currentNotes[8];
    for(int i = 0; sizeof(currentNotes)/sizeof(*currentNotes) > i; i++)
    {
        String indexString = std::to_string(i);
        String noteId = "note" + indexString;
        String octaveId = "octave" + indexString;
        
        currentNotes[i] = static_cast<int>(tree.getRawParameterValue(noteId)[0]);
        
    }
    
    int currentOctaves[8];
    for(int i = 0; sizeof(currentNotes)/sizeof(*currentNotes) > i; i++)
    {
        String indexString = std::to_string(i);
        String noteId = "octave" + indexString;
        currentOctaves[i] = static_cast<int>(tree.getRawParameterValue(noteId)[0]);
    }
    
        
    jassert (buffer.getNumChannels() == 0);
    auto numSamples = buffer.getNumSamples();
    auto noteDuration = static_cast<int> (std::ceil (rate * 0.25f * (0.1f + (1.0f - (speed)))));
    
    if ((time + numSamples) >= noteDuration)
    {
        if(stepCursor > 6)
        {
            stepCursor = 0;
        }else{
            stepCursor++;
        }
        
        auto offset = jmax (0, jmin ((int) (noteDuration - time), numSamples - 1));

        int midiMessage = currentNotes[stepCursor] + currentOctaves[stepCursor] * 12;
        
        if(currentNotesActiveState[stepCursor] == 1){
            midi.addEvent(MidiMessage::noteOn(1, midiMessage, (uint8) 127), offset);
        }
    }
    time = (time + numSamples) % noteDuration; //reset time when noteDuration is reached
}

//==============================================================================
bool ArpegiatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ArpegiatorAudioProcessor::createEditor()
{
    return new ArpegiatorAudioProcessorEditor (*this);
}

//==============================================================================
void ArpegiatorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ArpegiatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ArpegiatorAudioProcessor();
}

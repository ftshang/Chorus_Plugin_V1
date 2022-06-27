/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JUCE_Chorus_WalkthroughAudioProcessor::JUCE_Chorus_WalkthroughAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    treeState.addParameterListener("rate", this);
    treeState.addParameterListener("depth", this);
    treeState.addParameterListener("centreDelay", this);
    treeState.addParameterListener("feedback", this);
    treeState.addParameterListener("mix", this);
}

JUCE_Chorus_WalkthroughAudioProcessor::~JUCE_Chorus_WalkthroughAudioProcessor()
{
    treeState.removeParameterListener("rate", this);
    treeState.removeParameterListener("depth", this);
    treeState.removeParameterListener("centreDelay", this);
    treeState.removeParameterListener("feedback", this);
    treeState.removeParameterListener("mix", this);
}

//==============================================================================
const juce::String JUCE_Chorus_WalkthroughAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JUCE_Chorus_WalkthroughAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JUCE_Chorus_WalkthroughAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JUCE_Chorus_WalkthroughAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JUCE_Chorus_WalkthroughAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JUCE_Chorus_WalkthroughAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JUCE_Chorus_WalkthroughAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JUCE_Chorus_WalkthroughAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JUCE_Chorus_WalkthroughAudioProcessor::getProgramName (int index)
{
    return {};
}

void JUCE_Chorus_WalkthroughAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void JUCE_Chorus_WalkthroughAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    updateParameters();

    chorus.prepare(spec);
}

void JUCE_Chorus_WalkthroughAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JUCE_Chorus_WalkthroughAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void JUCE_Chorus_WalkthroughAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> sampleBlock{ buffer };
    chorus.process(juce::dsp::ProcessContextReplacing<float>(sampleBlock));
}

//==============================================================================
bool JUCE_Chorus_WalkthroughAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JUCE_Chorus_WalkthroughAudioProcessor::createEditor()
{
    return new JUCE_Chorus_WalkthroughAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void JUCE_Chorus_WalkthroughAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    // Create MemoryOutputStream object
    juce::MemoryOutputStream stream(destData, false);

    // calling writeToStream
    treeState.state.writeToStream(stream);
}

void JUCE_Chorus_WalkthroughAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    auto tree = juce::ValueTree::readFromData(data, size_t (sizeInBytes));

    if (tree.isValid())
    {
        treeState.state = tree;
        updateParameters();
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JUCE_Chorus_WalkthroughAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout JUCE_Chorus_WalkthroughAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("rate", "Rate", 0.0f, 90.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("depth", "Depth", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("centreDelay", "CentreDelay", 1.0f, 100.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("feedback", "Feedback", -1.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("mix", "Mix", 0.0f, 1.0f, 0.0f));

    return { params.begin(), params.end() };
}

void JUCE_Chorus_WalkthroughAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    updateParameters();
}

void JUCE_Chorus_WalkthroughAudioProcessor::updateParameters()
{
    chorus.setRate(treeState.getRawParameterValue("rate")->load());
    chorus.setDepth(treeState.getRawParameterValue("depth")->load());
    chorus.setCentreDelay(treeState.getRawParameterValue("centreDelay")->load());
    chorus.setFeedback(treeState.getRawParameterValue("feedback")->load());
    chorus.setMix(treeState.getRawParameterValue("mix")->load());
}
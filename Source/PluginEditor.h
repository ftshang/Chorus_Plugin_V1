/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class JUCE_Chorus_WalkthroughAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JUCE_Chorus_WalkthroughAudioProcessorEditor (JUCE_Chorus_WalkthroughAudioProcessor&);
    ~JUCE_Chorus_WalkthroughAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider rateDial;
    juce::Slider depthDial;
    juce::Slider centreDelayDial;
    juce::Slider feedbackDial;
    juce::Slider mixDial;

    juce::Label rateLabel;
    juce::Label depthLabel;
    juce::Label centreDelayLabel;
    juce::Label feedbackLabel;
    juce::Label mixLabel;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JUCE_Chorus_WalkthroughAudioProcessor& audioProcessor;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateDialAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> depthDialAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> centreDelayDialAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackDialAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixDialAttachment; 
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JUCE_Chorus_WalkthroughAudioProcessorEditor)
};

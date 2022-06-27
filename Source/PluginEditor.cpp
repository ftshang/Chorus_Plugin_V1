/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JUCE_Chorus_WalkthroughAudioProcessorEditor::JUCE_Chorus_WalkthroughAudioProcessorEditor(JUCE_Chorus_WalkthroughAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Dials

    rateDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    rateDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 72, 36);
    addAndMakeVisible(rateDial);

    rateDialAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "rate", rateDial);

    depthDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    depthDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 72, 36);
    addAndMakeVisible(depthDial);

    depthDialAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "depth", depthDial);

    centreDelayDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    centreDelayDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 72, 36);
    addAndMakeVisible(centreDelayDial);

    centreDelayDialAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "centreDelay", centreDelayDial);

    feedbackDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    feedbackDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 72, 36);
    addAndMakeVisible(feedbackDial);

    feedbackDialAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "feedback", feedbackDial);

    mixDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mixDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 72, 36);
    addAndMakeVisible(mixDial);

    mixDialAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "mix", mixDial);

    // Labels

    addAndMakeVisible(rateLabel);
    rateLabel.setText("Rate", juce::NotificationType::dontSendNotification);
    rateLabel.attachToComponent(&rateDial, false);
    rateLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(depthLabel);
    depthLabel.setText("Depth", juce::NotificationType::dontSendNotification);
    depthLabel.attachToComponent(&depthDial, false);
    depthLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(centreDelayLabel);
    centreDelayLabel.setText("Centre Delay", juce::NotificationType::dontSendNotification);
    centreDelayLabel.attachToComponent(&centreDelayDial, false);
    centreDelayLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(feedbackLabel);
    feedbackLabel.setText("Feedback", juce::NotificationType::dontSendNotification);
    feedbackLabel.attachToComponent(&feedbackDial, false);
    feedbackLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(mixLabel);
    mixLabel.setText("Mix", juce::NotificationType::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.attachToComponent(&mixDial, false);

    //setSize(1000, 500);

    //AudioProcessorEditor::setResizable(true, true);
    //AudioProcessorEditor::setResizeLimits(getWidth() * .75, getHeight() * .75, getWidth() * 1.25, getHeight() * 1.25);
    //AudioProcessorEditor::getConstrainer()->setFixedAspectRatio(2.0);

    // Grab the window instance and create a rectangle
    juce::Rectangle<int> r = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea;

    // Using the width is more useful than the height, because we know the height will always be < than width
    int x = r.getWidth();

    auto width = 0;

    if (r.getWidth() <= 1920)
    {
        width = x * 0.7f;
    }

    else
    {
        width = x * 0.4f;
    }

    auto height = width * 0.5;

    //Making the window resizable by aspect ratio and setting size
    AudioProcessorEditor::setResizable(true, true);
    AudioProcessorEditor::setResizeLimits(width * 0.5, height * 0.5, width * 1.25, height * 1.25);
    AudioProcessorEditor::getConstrainer()->setFixedAspectRatio(2.0);

    setSize(width, height);
    
}

JUCE_Chorus_WalkthroughAudioProcessorEditor::~JUCE_Chorus_WalkthroughAudioProcessorEditor()
{
}

//==============================================================================
void JUCE_Chorus_WalkthroughAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkred.darker(0.22f));
}

void JUCE_Chorus_WalkthroughAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    const auto leftMargin = getWidth() * 0.03;
    const auto dialSize = getWidth() * 0.19;
    const auto topMargin = getHeight() * 0.355;
    const auto labelFont = getWidth() * 0.018;

    rateDial.setBounds(leftMargin, topMargin, dialSize, dialSize);
    depthDial.setBounds(rateDial.getX() + rateDial.getWidth(), topMargin, dialSize, dialSize);
    centreDelayDial.setBounds(depthDial.getX() + depthDial.getWidth(), topMargin, dialSize, dialSize);
    feedbackDial.setBounds(centreDelayDial.getX() + centreDelayDial.getWidth(), topMargin, dialSize, dialSize);
    mixDial.setBounds(feedbackDial.getX() + feedbackDial.getWidth(), topMargin, dialSize, dialSize);

    rateLabel.setFont(juce::Font("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
    depthLabel.setFont(juce::Font("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
    centreDelayLabel.setFont(juce::Font("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
    feedbackLabel.setFont(juce::Font("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));
    mixLabel.setFont(juce::Font("Helvetica", labelFont, juce::Font::FontStyleFlags::bold));

    //juce::Rectangle<int> pluginArea = getLocalBounds();

  /*  rateDial.setBounds(pluginArea.removeFromLeft(getWidth() / 5));
    depthDial.setBounds(pluginArea.removeFromLeft(getWidth() / 5));
    centreDelayDial.setBounds(pluginArea.removeFromLeft(getWidth() / 5));
    feedbackDial.setBounds(pluginArea.removeFromLeft(getWidth() / 5));
    mixDial.setBounds(pluginArea);*/

    //rateLabel.setBounds(0, 0, 100, 50);

}

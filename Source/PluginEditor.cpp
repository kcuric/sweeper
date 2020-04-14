/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BoomerAudioProcessorEditor::BoomerAudioProcessorEditor (BoomerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    addAndMakeVisible(speedKnob = new Slider("Speed"));
    speedKnob->setSliderStyle(Slider::Rotary);
    speedKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    //speedKnob->addListener(this);

    addAndMakeVisible(highFreqKnob = new Slider("HighFreq"));
    highFreqKnob->setSliderStyle(Slider::Rotary);
    highFreqKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
    
    addAndMakeVisible(lowFreqKnob = new Slider("LowFreq"));
    lowFreqKnob->setSliderStyle(Slider::Rotary);
    lowFreqKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);

    addAndMakeVisible(mixKnob = new Slider("Mix"));
    mixKnob->setSliderStyle(Slider::Rotary);
    mixKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);

    addAndMakeVisible(volumeKnob = new Slider("Volume"));
    volumeKnob->setSliderStyle(Slider::Rotary);
    volumeKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);


    // Look and feel
    speedKnob->setLookAndFeel(&otherLookAndFeel);
    highFreqKnob->setLookAndFeel(&otherLookAndFeel);
    lowFreqKnob->setLookAndFeel(&otherLookAndFeel);
    mixKnob->setLookAndFeel(&otherLookAndFeel);
    volumeKnob->setLookAndFeel(&otherLookAndFeel);

    // Attachments
    speedAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "SpeedAttachment", *speedKnob);
    highFreqAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "HighFreqAttachment", *highFreqKnob);
    lowFreqAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "LowFreqAttachment", *lowFreqKnob);
    mixAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "MixAttachment", *mixKnob);
    volumeAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "VolumeAttachment", *volumeKnob);

    addAndMakeVisible(labelOne = new Label("LabelOne", JucePlugin_VersionString));

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 300);
}

void BoomerAudioProcessorEditor::setText(float value) 
{
    labelOne->setText(std::to_string(value), NotificationType::dontSendNotification);
}

void BoomerAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    float value = speedKnob->getValue();
    setText(value);
}

BoomerAudioProcessorEditor::~BoomerAudioProcessorEditor()
{
}

//==============================================================================
void BoomerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    Image background = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    g.drawImageAt(background, 0, 0);
}

void BoomerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    speedKnob->setBounds(getWidth() / 2 - 75, getHeight() / 2 - 75, 150, 150);
    highFreqKnob->setBounds(getWidth() / 2 + 100, getHeight() / 2 - 50, 100, 100);
    lowFreqKnob->setBounds(getWidth() / 2 - 200, getHeight() / 2 - 50, 100, 100);
    mixKnob->setBounds(getWidth() / 2 - 100, getHeight() / 2 - 125, 50, 50);
    volumeKnob->setBounds(getWidth() / 2 + 50, getHeight() / 2 - 125, 50, 50);
    labelOne->setBounds(0, 0, 100, 15);
}

/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class OtherLookAndFeel : public LookAndFeel_V4 
{
public:
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override
    {
        float diameter = jmin(width, height);
        float radius = diameter / 2;
        
        // Center position of the app.
        float centerX = x + width / 2;
        float centerY = y + height / 2;
        float rx = centerX - radius;
        float ry = centerY - radius;
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));


        Rectangle<float> dialArea(x, y, diameter, diameter);
        
        //g.setColour(Colours::slategrey);
        //g.fillEllipse(dialArea);

        // Tick

        Path dialTick;
        dialTick.addRectangle(0, -radius, 2.0, radius * 0.4);
        g.setColour(Colours::black);
        g.fillPath(dialTick, AffineTransform::rotation(angle).translated(centerX, centerY));

        g.drawEllipse(rx, ry, diameter, diameter, 2.0f);
    }
    
};

//==============================================================================
/**
*/
class BoomerAudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener
{
public:
    BoomerAudioProcessorEditor (BoomerAudioProcessor&);
    ~BoomerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void setText(float value);
    void sliderValueChanged(Slider* slider) override;

private:
    ScopedPointer<Slider> speedKnob;
    ScopedPointer<Slider> highFreqKnob;
    ScopedPointer<Slider> lowFreqKnob;
    ScopedPointer<Slider> mixKnob;
    ScopedPointer<Slider> volumeKnob;

    ScopedPointer<Label> labelOne;

    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> speedAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> highFreqAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> lowFreqAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;

    // Look
    OtherLookAndFeel otherLookAndFeel;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BoomerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BoomerAudioProcessorEditor)
};

/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "gui/include/SCLookAndFeel.h"
#include "gui/include/LabeledSlider.h"
#include "gui/include/Meter.h"

//==============================================================================
/**
*/
class CtagdrcAudioProcessorEditor : public AudioProcessorEditor, public Button::Listener, Timer
{
public:
    CtagdrcAudioProcessorEditor(CtagdrcAudioProcessor&, AudioProcessorValueTreeState&);
    ~CtagdrcAudioProcessorEditor();

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;
    void buttonClicked(Button*) override;
    void timerCallback() override;

private:
    void initWidgets();
    void setGUIState(bool);

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CtagdrcAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;

    //LookAndFeel
    SCLookAndFeel scLaF;
    Colour backgroundApp;

    //Widgets
    MeterBackground meterbg;
    Meter meter;

    LabeledSlider inGainLSlider;
    LabeledSlider makeupGainLSlider;
    LabeledSlider treshLSlider;
    LabeledSlider ratioLSlider;
    LabeledSlider kneeLSlider;
    LabeledSlider attackLSlider;
    LabeledSlider releaseLSlider;
    LabeledSlider mixLSlider;

    TextButton lahButton;
    TextButton autoAttackButton;
    TextButton autoReleaseButton;
    TextButton autoMakeupButton;
    DrawableButton powerButton;

    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lahAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> autoAttackAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> autoReleaseAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> autoMakeupAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> powerAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CtagdrcAudioProcessorEditor)
};

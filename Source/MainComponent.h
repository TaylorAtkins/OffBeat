#pragma once

#include <JuceHeader.h>
#include "MetronomeComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
//==============================================================================
class MainComponent : public juce::Component, public juce::Button::Listener
{
public:
    MainComponent();
    ~MainComponent() override;
    void buttonClicked(juce::Button *button) override;
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    MetronomeComponent metronome;
    juce::TextButton playButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

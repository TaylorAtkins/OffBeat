#pragma once

#include <JuceHeader.h>
#include "MetronomeComponent.h"
#include <unistd.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
//==============================================================================
enum RadioButtonIds
{
    PlayerNumber = 1001
};

class MainComponent : public juce::Component, public juce::Button::Listener, public juce::ChangeListener, public juce::Slider::Listener
{
public:
    MainComponent();
    ~MainComponent() override;
    void buttonClicked(juce::Button *button) override;
    void sliderValueChanged (juce::Slider* slider) override;
    void paint(juce::Graphics &) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;

private:
    MetronomeComponent metronome;
    juce::TextButton playButton;
    juce::TextButton startButton;
    juce::TextButton playAgainButton;
    juce::TextButton mainMenuButton;
    juce::TextButton playerNumberButtons[4];
    juce::Slider sensitivitySlider;
    int playerNum;
    float sensitivity;
    juce::ChangeBroadcaster roundBroadcaster;
    juce::ChangeBroadcaster clapBroadcaster;
    juce::ChangeBroadcaster loseBroadcaster;
    juce::ChangeBroadcaster offBeatBroadcaster;
    int rounds;
    int claps;
    int offBeats;
    juce::Label sensitivityLabel;
    juce::Label roundLabel;
    juce::Label clapLabel;
    juce::Label scoreLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

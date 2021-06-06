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
    void sliderValueChanged(juce::Slider *slider) override;
    void paint(juce::Graphics &) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;

private:
    juce::TextButton playButton;
    juce::TextButton instructionsButton;
    juce::TextButton backButton;
    juce::TextButton startButton;
    juce::TextButton playAgainButton;
    juce::TextButton mainMenuButton;
    juce::TextButton playerNumberButtons[4];
    juce::ToggleButton debugCheckbox;
    juce::Slider sensitivitySlider;
    juce::ChangeBroadcaster roundBroadcaster;
    juce::ChangeBroadcaster onBeatBroadcaster;
    juce::ChangeBroadcaster loseBroadcaster;
    juce::ChangeBroadcaster offBeatBroadcaster;
    juce::ChangeBroadcaster clapBroadcaster;
    juce::Label instructionsLabel;
    juce::Label sensitivityLabel;
    juce::Label roundLabel;
    juce::Label recordedBeatsLabel;
    juce::Label scoreLabel;
    MetronomeComponent metronome;
    int playerNum;
    int rounds;
    int claps;
    int onBeats;
    int offBeats;
    bool debug;
    float sensitivity;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

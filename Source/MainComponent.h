#pragma once

#include <JuceHeader.h>
#include "MetronomeComponent.h"
#include <unistd.h>

enum RadioButtonIds
{
    PlayerNumber = 1001
};

// This component is the main component in the GUI window. All other components reside within
// here as data members and are made visible by this component, based on various conditions. In general,
// the MainCompnent is in charge of updating the GUI based off of user input
class MainComponent : public juce::Component, public juce::Button::Listener, public juce::ChangeListener, public juce::Slider::Listener
{
public:
    MainComponent();
    ~MainComponent() override;
    // Updates the GUI and game settings based off of buttons being clicked
    void buttonClicked(juce::Button *button) override;
    // Updates game settings based off of slider values being changed
    void sliderValueChanged(juce::Slider *slider) override;
    void paint(juce::Graphics &) override;
    // Sets the size and placement of all components in the GUI window
    void resized() override;
    // Listens for any signals being broadcasted by the RhythmProcessor and updates the GUI to reflect them
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
    juce::Label playerNumLabel;
    juce::Label sensitivityLabel;
    juce::Label roundLabel;
    juce::Label recordedBeatsLabel;
    juce::Label scoreLabel;
    MetronomeComponent metronome;
    bool debug;
    int playerNum;
    int rounds;
    int claps;
    int onBeats;
    int offBeats;
    float sensitivity;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

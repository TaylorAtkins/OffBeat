/*
  ==============================================================================

    MetronomeComponent.h
    Created: 6 May 2021 1:32:28pm
    Author:  Taylor Atkins

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "AudioManager.h"
#include "Beat.h"

// This class generates a random rhythm based on user settings provided by the MainComponent class.
// After the rhythm is generated, it continually paints the component to create metrenome animation
// based off of the beats in the rhythm
class MetronomeComponent : public juce::AnimatedAppComponent
{
public:
  MetronomeComponent();
  ~MetronomeComponent() override;
  void update() override;
  // Paints the current frame of the metrenome animation
  void paint(juce::Graphics &) override;
  void resized() override;
  // A wrapper used to set the RhythmProcessor's broadcasters by sending them to the AudioManager
  void setBroadcaster(juce::ChangeBroadcaster *roundBroadcaster, juce::ChangeBroadcaster *onBeatBroadcaster, juce::ChangeBroadcaster *loseBroadcaster, juce::ChangeBroadcaster *offBeatBroadcaster, juce::ChangeBroadcaster *clapBroadcaster);
  // Updates settings needed for rhythm generation (provided by the MetronomeComponent) and sends setting information
  // to the RhythmProcessor, through the AudioManager class.
  void updateSettings(int playerNum, float sensitivity, bool debug);
  // Generates a new random rhythm based off the number of players in a game and calls a function
  // in the AudioManager class to start audio playing/processing based off of this rhythm
  void newRhythm();

private:
  AudioManager audioManager;
  Beat *beats;
  int playerNum;
  int totalBeats;
  int currentBeat;
  int previousFrames;
  int minBPM;
  int maxBPM;
  float secPerBeat;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MetronomeComponent)
};

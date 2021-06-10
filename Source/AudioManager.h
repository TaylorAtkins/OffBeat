#pragma once

#include <iostream>
#include <JuceHeader.h>
#include "RhythmProcessor.h"

// This class manages the connection between the audio devices on a computer and the RhythmProcessor.
class AudioManager
{
public:
  AudioManager();
  ~AudioManager();
  // A wrapper to set the RhythmProcessor's signal broadcasters, so RhythmProcessor can comunicate with the MainComponent
  void setBroadcaster(juce::ChangeBroadcaster *roundBroadcaster, juce::ChangeBroadcaster *onBeatBroadcaster, juce::ChangeBroadcaster *loseBroadcaster, juce::ChangeBroadcaster *offBeatBroadcaster, juce::ChangeBroadcaster *clapBroadcaster);
  // A wrapper to update the RhythmProcessor's settigns based on user input from the MainComponent
  void updateSettings(float sensitivity, bool debug);
  // A wrapper to generate a rhythm's audio and begin the game's audio processing
  void generateRhythm(Beat *beats, int totalBeats, float secPerBeat);

private:
  juce::AudioProcessorPlayer audioPlayer;
  juce::AudioDeviceManager audioDevice;
  RhythmProcessor processor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioManager)
};

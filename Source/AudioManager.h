#pragma once

#include <iostream>

#include <JuceHeader.h>

#include "RhythmProcessor.h"

// Manages
class AudioManager
{
public:
  AudioManager();
  ~AudioManager();
  void setBroadcaster(juce::ChangeBroadcaster *roundBroadcaster, juce::ChangeBroadcaster *onBeatBroadcaster, juce::ChangeBroadcaster *loseBroadcaster, juce::ChangeBroadcaster *offBeatBroadcaster, juce::ChangeBroadcaster *clapBroadcaster);
  void updateSettings(float sensitivity, bool debug);
  void generateRhythm(Beat *beats, int totalBeats, float secPerBeat);

private:
  RhythmProcessor processor;
  juce::AudioProcessorPlayer audioPlayer;
  juce::AudioDeviceManager audioDevice;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioManager)
};

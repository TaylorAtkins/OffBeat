/*
  ==============================================================================

    AudioManager.h
    Created: 22 May 2021 11:12:02pm
    Author:  Taylor Atkins

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <iostream>
#include "RhythmProcessor.h"

class AudioManager
{
public:
  AudioManager();
  ~AudioManager();
    void setBroadcaster(juce::ChangeBroadcaster *roundBroadcaster, juce::ChangeBroadcaster *onBeatBroadcaster, juce::ChangeBroadcaster *loseBroadcaster, juce::ChangeBroadcaster *offBeatBroadcaster);
    void setSensitivity(float sensitivity);
  void generateRhythm(Beat *beats, int totalBeats, float secPerBeat);

private:
  RhythmProcessor processor;
  juce::AudioProcessorPlayer audioPlayer;
  juce::AudioDeviceManager audioDevice;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioManager)
};

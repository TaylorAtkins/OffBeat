/*
  ==============================================================================

    AudioManagerComponent.h
    Created: 14 May 2021 9:13:36pm
    Author:  Taylor Atkins

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <iostream>
#include "Beat.h"

//==============================================================================
/*
*/
class AudioManagerComponent : public juce::AudioAppComponent
{
public:
  AudioManagerComponent();
  ~AudioManagerComponent() override;
  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
  void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;
  void releaseResources() override;
  void paint(juce::Graphics &) override;
  void resized() override;
  void generateRhythm(Beat *beats, int totalBeats, float secPerBeat);

private:
  float *samples;
  int currentSample;
  int totalSamples;
  int roundCount;
  float toneDuration;
  float currentSampleRate;
    float *frequencies;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioManagerComponent)
};

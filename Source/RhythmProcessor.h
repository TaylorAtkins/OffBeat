/*
  ==============================================================================

    RhythmProcessor.h
    Created: 22 May 2021 9:14:43pm
    Author:  Taylor Atkins

  ==============================================================================
*/
#include <JuceHeader.h>
#include <iostream>
#include "Beat.h"

#pragma once

class RhythmProcessor : public juce::AudioProcessor
{
public:
  RhythmProcessor();
  ~RhythmProcessor();
  void prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) override;
  void releaseResources() override;
  void processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) override;
  void setBroadcaster(juce::ChangeBroadcaster *roundBroadcaster, juce::ChangeBroadcaster *clapBroadcaster);
  void generateRhythm(Beat *beats, int totalBeats, float secPerBeat);
  const juce::String getName() const override;
  virtual double getTailLengthSeconds() const override;
  bool acceptsMidi() const override;
  bool producesMidi() const override;
  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String &newName) override;
  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

private:
  float *samples;
  int currentSample;
  int totalSamples;
  int samplesPerBeat;
  int currentBeat;
  int totalBeats;
  int roundCount;
  int windowSize;
  int soundDuration;
  int clapCount;
  int minDuration;
  int maxDuration;
  float threshold;
  std::list<float> sampleWindow;
  float toneDuration;
  float currentSampleRate;
  float *frequencies;
  Beat *beats;
  juce::ChangeBroadcaster *roundBroadcaster;
  juce::ChangeBroadcaster *clapBroadcaster;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RhythmProcessor)
};

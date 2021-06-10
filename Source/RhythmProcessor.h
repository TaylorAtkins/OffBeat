#pragma once

#include <iostream>
#include <JuceHeader.h>
#include "Beat.h"

class RhythmProcessor : public juce::AudioProcessor
{
public:
  RhythmProcessor();
  ~RhythmProcessor();
  void prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) override;
  void releaseResources() override;
  void processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) override;
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
  void setBroadcaster(juce::ChangeBroadcaster *roundBroadcaster, juce::ChangeBroadcaster *onBeatBroadcaster, juce::ChangeBroadcaster *loseBroadcaster, juce::ChangeBroadcaster *offBeatBroadcaster, juce::ChangeBroadcaster *clapBroadcaster);
  void generateRhythm(Beat *beats, int totalBeats, float secPerBeat);
  void updateSettings(float sensitivity, bool debug);
  void loadFilterCoeffs();

private:
  juce::ChangeBroadcaster *roundBroadcaster;
  juce::ChangeBroadcaster *onBeatBroadcaster;
  juce::ChangeBroadcaster *loseBroadcaster;
  juce::ChangeBroadcaster *offBeatBroadcaster;
  juce::ChangeBroadcaster *clapBroadcaster;
  std::list<float> sampleWindow;
  std::vector<float> filterCoeffs;
  Beat *beats;
  bool isPlaying;
  bool isProcessing;
  bool debug;
  int currentSample;
  int totalSamples;
  int samplesPerBeat;
  int currentBeat;
  int totalBeats;
  int missedBeats;
  int roundCount;
  int windowSize;
  int soundDuration;
  int clapCount;
  int minDuration;
  int maxDuration;
  int coeffNum;
  float threshold;
  float toneDuration;
  float currentSampleRate;
  float frequencies[5];
  float *samples;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RhythmProcessor)
};

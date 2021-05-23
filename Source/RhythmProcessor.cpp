/*
  ==============================================================================

    RhythmProcessor.cpp
    Created: 22 May 2021 9:14:43pm
    Author:  Taylor Atkins

  ==============================================================================
*/
#include "RhythmProcessor.h"

RhythmProcessor::RhythmProcessor()
{
  samples = nullptr;
  currentSample = 0;
  roundCount = -1;
  toneDuration = 0.25f;
  frequencies = new float[5];
  frequencies[0] = 0.00;   // No Audio (Rest)
  frequencies[1] = 293.66; // D4
  frequencies[2] = 369.99; // F#4
  frequencies[3] = 440.00; // A4
  frequencies[4] = 523.25; // C5
  currentSampleRate = 48000;
}

RhythmProcessor::~RhythmProcessor()
{

  delete[] frequencies;
  if (samples)
    delete[] samples;
}
void RhythmProcessor::prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock)
{
  currentSampleRate = sampleRate;
}
void RhythmProcessor::releaseResources()
{
}
void RhythmProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{

  for (int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); ++sampleIndex)
  {
    if (currentSample >= totalSamples)
    {
      currentSample = 0;
      if (roundCount > -1)
        ++roundCount;
      if (roundCount >= 4)
        roundBroadcaster->sendChangeMessage();
    }
    //std::cout << "test";

    //float sample = rhythmSynth.getNextSample();
    //std::cout << samples[currentSample] << '\t';
    float sample = 0.0f;
    if (roundCount >= 4)
    {
      roundBroadcaster->sendChangeMessage();
    }
    else if (samples)
    {
      sample = samples[currentSample] * 0.2f;
      assert(sample <= 1.0f && sample >= -1.0f);
    }

    for (int j = 0; j < buffer.getNumChannels(); ++j)
    {

      buffer.setSample(j, sampleIndex, sample);
    }
    ++currentSample;
  }
}
void RhythmProcessor::setBroadcaster(juce::ChangeBroadcaster *roundBroadcaster)
{
  this->roundBroadcaster = roundBroadcaster;
}

void RhythmProcessor::generateRhythm(Beat *beats, int totalBeats, float secPerBeat)
{
  if (samples)
    delete[] samples;

  totalSamples = secPerBeat * totalBeats * currentSampleRate;
  samples = new float[totalSamples];

  int samplesPerBeat = secPerBeat * currentSampleRate;
  float waitTime = (secPerBeat - toneDuration) / 2.0f;
  if (waitTime < 0)
    waitTime = 0.0f;
  for (int i = 0; i < totalSamples; ++i)
    samples[i] = 0.0f;

  for (int beatIndex = 0; beatIndex < totalBeats; ++beatIndex)
  {
    float beatFrequency = frequencies[beats[beatIndex].player];
    float phaseChange = beatFrequency * 2.0f * juce::MathConstants<float>::pi / currentSampleRate;
    float phase = 0.0f;

    for (int sample = 0; sample < samplesPerBeat; ++sample)
    {
      if (((float)sample / currentSampleRate) > waitTime && ((float)sample / currentSampleRate) <= (waitTime + toneDuration))
      {

        samples[sample + samplesPerBeat * beatIndex] = (float)std::sin(phase * phaseChange);
        //phase += phaseChange;
        ++phase;
      }
    }
  }
  roundCount = 0;
  currentSample = 0;
}

const juce::String RhythmProcessor::getName() const
{
  return "RhythmProcessor";
}
double RhythmProcessor::getTailLengthSeconds() const { return 0.0; }
bool RhythmProcessor::acceptsMidi() const { return false; }
bool RhythmProcessor::producesMidi() const { return false; }
juce::AudioProcessorEditor *RhythmProcessor::createEditor() { return nullptr; }
bool RhythmProcessor::hasEditor() const { return false; }
int RhythmProcessor::getNumPrograms() { return 0; }
int RhythmProcessor::getCurrentProgram() { return 0; }
void RhythmProcessor::setCurrentProgram(int index) {}
const juce::String RhythmProcessor::getProgramName(int index) { return ""; }
void RhythmProcessor::changeProgramName(int index, const juce::String &newName) {}
void RhythmProcessor::getStateInformation(juce::MemoryBlock &destData) {}
void RhythmProcessor::setStateInformation(const void *data, int sizeInBytes) {}

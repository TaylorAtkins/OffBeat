/*
  ==============================================================================

    AudioManagerComponent.cpp
    Created: 14 May 2021 9:13:36pm
    Author:  Taylor Atkins

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioManagerComponent.h"

//==============================================================================
AudioManagerComponent::AudioManagerComponent()
{
  samples = NULL;
  currentSample = 0;
  roundCount = 0;
  toneDuration = 0.25f;
    frequencies = new float[5];
    frequencies[0] = 0.00;   // No Audio (Rest)
    frequencies[1] = 293.66; // D4
    frequencies[2] = 369.99; // F#4
    frequencies[3] = 440.00; // A4
    frequencies[4] = 523.25; // C5
  setAudioChannels(2, 2);
}

AudioManagerComponent::~AudioManagerComponent()
{  delete[] frequencies;
  if (samples)
    delete[] samples;

  shutdownAudio();
}

void AudioManagerComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
  auto device = deviceManager.getAudioDeviceSetup();

  device.sampleRate = 48000;
  currentSampleRate = device.sampleRate;
}

void AudioManagerComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill)
{
  auto *leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
  auto *rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

  for (int sampleIndex = 0; sampleIndex < bufferToFill.numSamples; ++sampleIndex)
  {
    //std::cout << "test";
    if (currentSample >= totalSamples)
    {
      currentSample = 0;
      ++roundCount;
    }
    //float sample = rhythmSynth.getNextSample();
    //std::cout << samples[currentSample] << '\t';
    float sample = 0.0f;
    if (roundCount < 4 && samples)
    {
      sample = samples[currentSample] * 0.2f;
      assert(sample <= 1.0f && sample >= -1.0f );
    }
    leftBuffer[sampleIndex] = sample;
    rightBuffer[sampleIndex] = sample;
    ++currentSample;
  }
}

void AudioManagerComponent::generateRhythm(Beat *beats, int totalBeats, float secPerBeat)
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


void AudioManagerComponent::releaseResources()
{
}

void AudioManagerComponent::paint(juce::Graphics &g)
{
}

void AudioManagerComponent::resized()
{
}

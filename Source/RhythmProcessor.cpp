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
  samplesPerBeat = 0;
  beats = nullptr;
  totalBeats = 0;
  threshold = 0.0f;

  soundDuration = 0;
  clapCount = 0;
  minDuration = 5;
  maxDuration = 30;
  threshold = 0.30;
  windowSize = 20;
  for (int i = 0; i < windowSize; ++i)
    sampleWindow.push_back(0.0f);

  currentBeat = 0;
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
      currentBeat = 0;
      if (roundCount > -1)
        ++roundCount;
      if (roundCount >= 4)
        roundBroadcaster->sendChangeMessage();
    }

    //If we are on the next beat, verify number of claps and reset variables
    if (roundCount > -1)
    {
      if (currentBeat != currentSample % samplesPerBeat)
      {
        // If more than one player clapped on a beat or a player clapper during a rest, end game
        //if((clapCount != 1 && beats[currentBeat].player != 0) || (clapCount > 0 && beats[currentBeat].player == 0))
        //if(clapCount > 0)
        //clapBroadcaster->sendChangeMessage();
        ++currentBeat;
        //soundDuration = 0;
        clapCount = 0;
      }

      //Average the left and right inputs
      //float input = buffer.getSample(0, sampleIndex) * buffer.getSample(1, sampleIndex) / 2.0f;

      float input = buffer.getSample(0, sampleIndex);

      // Update window with new value
      sampleWindow.push_back(input);
      sampleWindow.pop_front();

      float average = 0.0;
      //Average all values in sampleWindow
      for (float sampleToAverage : sampleWindow)
      {
        average += sampleToAverage;
      }
      average /= (float)windowSize;

      if (average > threshold)
      {
        ++soundDuration;
      }
      else
      {
        if (soundDuration >= minDuration && soundDuration <= maxDuration)
        {
          ++clapCount;
          clapBroadcaster->sendChangeMessage();
        }
        soundDuration = 0;
      }

      //assert(soundDuration ==0);
    }

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
    //assert(leftInput[sampleIndex] ==0);
    //Left Channel
    //leftOutput[sampleIndex] = sample;
    //Right Channel
    //rightOutput[sampleIndex] = sample;

    //Left Channel
    buffer.setSample(0, sampleIndex, sample);
    //Right Channel
    buffer.setSample(1, sampleIndex, sample);

    ++currentSample;
  }
}
void RhythmProcessor::setBroadcaster(juce::ChangeBroadcaster *roundBroadcaster, juce::ChangeBroadcaster *clapBroadcaster)
{
  this->roundBroadcaster = roundBroadcaster;
  this->clapBroadcaster = clapBroadcaster;
}

void RhythmProcessor::generateRhythm(Beat *beats, int totalBeats, float secPerBeat)
{
  if (samples)
    delete[] samples;

  totalSamples = secPerBeat * totalBeats * currentSampleRate;
  samples = new float[totalSamples];
  samplesPerBeat = secPerBeat * currentSampleRate;
  this->totalBeats = totalBeats;
  this->beats = beats;
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
  currentBeat = 0;
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
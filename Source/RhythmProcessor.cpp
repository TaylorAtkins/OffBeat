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
  roundBroadcaster = nullptr;
  onBeatBroadcaster = nullptr;
  loseBroadcaster = nullptr;
  offBeatBroadcaster = nullptr;
  clapBroadcaster = nullptr;
  beats = nullptr;
  samples = nullptr;

  windowSize = 20;
  for (int i = 0; i < windowSize; ++i)
    sampleWindow.push_back(0.0f);

  isPlaying = false;
  isProcessing = false;
  debug = false;
  currentSampleRate = 48000;
  samplesPerBeat = 0;
  totalBeats = 0;
  soundDuration = 0;
  currentSample = 0;
  currentBeat = 0;
  clapCount = 0;
  roundCount = 0;
  minDuration = 30;
  maxDuration = 500;
  threshold = 0.0f;
  toneDuration = 0.25f;

  frequencies[0] = 0.00;   // No Audio (Rest)
  frequencies[1] = 293.66; // D4
  frequencies[2] = 369.99; // F#4
  frequencies[3] = 440.00; // A4
  frequencies[4] = 523.25; // C5

  loadFilterCoeffs();
}

RhythmProcessor::~RhythmProcessor()
{
  if (samples)
    delete[] samples;
}

void RhythmProcessor::loadFilterCoeffs()
{
  // Loads coefficients from binary resource file "filterCoeffs.txt"
  juce::String coeffString = BinaryData::FilterCoeffs_txt;
  int startIndex = 0;
  int endIndex = 0;
  int strLen = coeffString.length();

  while (endIndex != -1 && startIndex < strLen)
  {
    if (coeffString[startIndex] != '\n')
    {
      // Parse the next coefficient in the string and add it to the vector of coefficients
      endIndex = coeffString.indexOf(startIndex, "\n");
      try
      {
        float coefficient = 0;
        if (endIndex == -1)
          coefficient = coeffString.substring(startIndex, strLen).getFloatValue();
        else
          coefficient = coeffString.substring(startIndex, endIndex).getFloatValue();
        filterCoeffs.push_back(coefficient);
      }
      catch (...)
      {
        // If an error occured while trying to parse the text file, remove any previous data read
        // in as filtered coefficents and stop processing the file.
        std::cerr << "Failed to parse necessary resource file \"filterCoeffs.txt\"\n";
        filterCoeffs.clear();
        coeffNum = 0;
        return;
      }
      startIndex = endIndex + 1;
    }
    else
      ++startIndex;
  }

  coeffNum = (int)filterCoeffs.size();
}

void RhythmProcessor::prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock)
{
  currentSampleRate = sampleRate;
}

void RhythmProcessor::releaseResources() {}

void RhythmProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
  int numSamples = buffer.getNumSamples();
  for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
  {
    // If we are on the next beat, verify number of claps and reset variables
    if (isProcessing)
    {
      if (currentBeat != ((int)currentSample / samplesPerBeat))
      {
        // If more than one player clapped on a beat or a player clapper during a rest, end game
        if ((clapCount != 1 && beats[currentBeat].player != 0) || (clapCount > 0 && beats[currentBeat].player == 0))
        {
          ++missedBeats;
          offBeatBroadcaster->sendChangeMessage();
        }
        else
        {
          onBeatBroadcaster->sendChangeMessage();
        }

        // Resets clap detection variables
        currentBeat = (int)(currentSample / samplesPerBeat);
        soundDuration = 0;
        clapCount = 0;

        // Clears samples in clap detection window from the previous beat
        sampleWindow.clear();
        for (int i = 0; i < windowSize; ++i)
          sampleWindow.push_back(0.0f);
      }
    }

    // If at the end of the rhythm, check is the rhythm should be played again
    if (currentSample >= totalSamples)
    {
      // Reset samples to the begining of the rhythm
      currentSample = 0;
      currentBeat = 0;
      if (isPlaying)
      {
        if (roundCount == 0)
        {
          // Starts processing audio input after the thythm has been played one time through
          isProcessing = true;
        }
        else if (roundCount >= 3)
        {
          // Triggers callback to generate new rhythm & pauses audio output and processing
          isPlaying = false;
          isProcessing = false;
          if ((float)missedBeats > (float)totalBeats * 3.0 / 2.0)
          {
            loseBroadcaster->sendChangeMessage();
          }
          else
          {
            roundBroadcaster->sendChangeMessage();
          }
        }
        ++roundCount;
      }
    }

    // Process input audio to look for claps
    if (isProcessing)
    {
      //If we are on the next beat, verify number of claps and reset variables
      if (currentBeat != ((int)currentSample / samplesPerBeat))
      {
        // If more than one player clapped on a beat or a player clapper during a rest, end game
        if ((clapCount != 1 && beats[currentBeat].player != 0) || (clapCount > 0 && beats[currentBeat].player == 0))
        {
          ++missedBeats;
          offBeatBroadcaster->sendChangeMessage();
        }
        else
        {
          onBeatBroadcaster->sendChangeMessage();
        }

        // Resets clap detection variables
        currentBeat = (int)(currentSample / samplesPerBeat);
        soundDuration = 0;
        clapCount = 0;

        // Clears samples in clap detection window from the previous beat
        sampleWindow.clear();
        for (int i = 0; i < windowSize; ++i)
          sampleWindow.push_back(0.0f);
      }

      // High pass filter
      float filteredSample = 0.0;
      for (int coeffIndex = 0; coeffIndex < coeffNum; ++coeffIndex)
      {
        if (sampleIndex - coeffIndex >= 0)
          filteredSample += filterCoeffs[coeffIndex] * buffer.getSample(0, sampleIndex - coeffIndex);
      }

      // Update window with new value
      sampleWindow.push_back(filteredSample);
      sampleWindow.pop_front();

      //Average all values in sampleWindow
      float average = 0.0;
      for (float sampleToAverage : sampleWindow)
      {
        average += sampleToAverage;
      }
      average /= (float)windowSize;

      // Check if noise was loud enough to be a clap
      if (average > threshold)
      {
        ++soundDuration;
      }
      else
      {
        // Check if the noise was the correct duration of a clap
        if (soundDuration >= minDuration && soundDuration <= maxDuration)
        {
          ++clapCount;

          if (debug)
          {
            clapBroadcaster->sendChangeMessage();
          }
        }
        soundDuration = 0;
      }
    }

    // If the audio should be playing get the sample to output
    float sample = 0.0f;
    if (isPlaying && samples)
    {
      sample = samples[currentSample] * 0.2f;
      assert(sample <= 1.0f && sample >= -1.0f);
    }

    //Left Channel
    buffer.setSample(0, sampleIndex, sample);
    //Right Channel
    buffer.setSample(1, sampleIndex, sample);

    ++currentSample;
  }
}

void RhythmProcessor::setBroadcaster(juce::ChangeBroadcaster *roundBroadcaster, juce::ChangeBroadcaster *onBeatBroadcaster, juce::ChangeBroadcaster *loseBroadcaster, juce::ChangeBroadcaster *offBeatBroadcaster, juce::ChangeBroadcaster *clapBroadcaster)
{
  this->roundBroadcaster = roundBroadcaster;
  this->onBeatBroadcaster = onBeatBroadcaster;
  this->loseBroadcaster = loseBroadcaster;
  this->offBeatBroadcaster = offBeatBroadcaster;
  this->clapBroadcaster = clapBroadcaster;
}

void RhythmProcessor::updateSettings(float sensitivity, bool debug)
{
  // This seems to be a good range for the treshold
  threshold = 0.004 + sensitivity * 0.0002;
  this->debug = debug;
}

void RhythmProcessor::generateRhythm(Beat *beats, int totalBeats, float secPerBeat)
{
  this->totalBeats = totalBeats;
  this->beats = beats;

  // Initialize array for audio output samples
  if (samples)
    delete[] samples;

  totalSamples = secPerBeat * totalBeats * currentSampleRate;
  samples = new float[totalSamples];
  for (int i = 0; i < totalSamples; ++i)
    samples[i] = 0.0f;

  samplesPerBeat = secPerBeat * currentSampleRate;
  float waitTime = (secPerBeat - toneDuration) / 2.0f;
  if (waitTime < 0)
    waitTime = 0.0f;

  // Generate the samples for metronome output audio
  for (int beatIndex = 0; beatIndex < totalBeats; ++beatIndex)
  {
    float beatFrequency = frequencies[beats[beatIndex].player];
    float phaseChange = beatFrequency * 2.0f * juce::MathConstants<float>::pi / currentSampleRate;
    float phase = 0.0f;

    // Generate sine wave samples for the current beat
    for (int sample = 0; sample < samplesPerBeat; ++sample)
    {
      if (((float)sample / currentSampleRate) > waitTime && ((float)sample / currentSampleRate) <= (waitTime + toneDuration))
      {

        samples[sample + samplesPerBeat * beatIndex] = (float)std::sin(phase * phaseChange);
        ++phase;
      }
    }
  }

  // Initialize audio processing variables
  roundCount = 0;
  currentSample = 0;
  currentBeat = 0;
  missedBeats = 0;
  isPlaying = true;
}

const juce::String RhythmProcessor::getName() const { return "RhythmProcessor"; }

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

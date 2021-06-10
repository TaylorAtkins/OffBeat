#include "AudioManager.h"

AudioManager::AudioManager()
{
  // Sets up application to use one input and two output audio devices
  audioDevice.initialiseWithDefaultDevices(1, 2);
  auto deviceSetup = audioDevice.getAudioDeviceSetup();
  deviceSetup.sampleRate = 48000;

  // Connects the audio input and output to the rhythm processor
  audioPlayer.setProcessor(&processor);
  audioDevice.addAudioCallback(&audioPlayer);
}

AudioManager::~AudioManager()
{
  audioDevice.removeAudioCallback(&audioPlayer);
}

void AudioManager::setBroadcaster(juce::ChangeBroadcaster *roundBroadcaster, juce::ChangeBroadcaster *onBeatBroadcaster, juce::ChangeBroadcaster *loseBroadcaster, juce::ChangeBroadcaster *offBeatBroadcaster, juce::ChangeBroadcaster *clapBroadcaster)
{
  processor.setBroadcaster(roundBroadcaster, onBeatBroadcaster, loseBroadcaster, offBeatBroadcaster, clapBroadcaster);
}

void AudioManager::updateSettings(float sensitivity, bool debug)
{
  processor.updateSettings(sensitivity, debug);
}

void AudioManager::generateRhythm(Beat *beats, int totalBeats, float secPerBeat)
{
  processor.generateRhythm(beats, totalBeats, secPerBeat);
}

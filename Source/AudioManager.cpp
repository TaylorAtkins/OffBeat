#include "AudioManager.h"

AudioManager::AudioManager()
{
  audioDevice.initialiseWithDefaultDevices(2, 2);
  auto deviceSetup = audioDevice.getAudioDeviceSetup();
  deviceSetup.sampleRate = 48000;
  audioPlayer.setProcessor(&processor);
  audioDevice.addAudioCallback(&audioPlayer);
}

AudioManager::~AudioManager()
{
  audioDevice.removeAudioCallback(&audioPlayer);
}

void AudioManager::setBroadcaster(juce::ChangeBroadcaster *roundBroadcaster, juce::ChangeBroadcaster *onBeatBroadcaster, juce::ChangeBroadcaster *loseBroadcaster, juce::ChangeBroadcaster *offBeatBroadcaster,juce::ChangeBroadcaster *clapBroadcaster)
{
  processor.setBroadcaster(roundBroadcaster, onBeatBroadcaster, loseBroadcaster, offBeatBroadcaster, clapBroadcaster);
}

void AudioManager::updateSettings(float sensitivity, bool debug){
    processor.updateSettings(sensitivity, debug);
}
void AudioManager::generateRhythm(Beat *beats, int totalBeats, float secPerBeat)
{
  processor.generateRhythm(beats, totalBeats, secPerBeat);
}

/*
  ==============================================================================

    AudioManager.cpp
    Created: 22 May 2021 11:12:02pm
    Author:  Taylor Atkins

  ==============================================================================
*/

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

void AudioManager::setBroadcaster(juce::ChangeBroadcaster *roundBroadcaster, juce::ChangeBroadcaster *onBeatBroadcaster, juce::ChangeBroadcaster *loseBroadcaster, juce::ChangeBroadcaster *offBeatBroadcaster)
{
  processor.setBroadcaster(roundBroadcaster, onBeatBroadcaster, loseBroadcaster, offBeatBroadcaster);
}

void AudioManager::setSensitivity(float sensitivity){
    processor.setSensitivity(sensitivity);
}
void AudioManager::generateRhythm(Beat *beats, int totalBeats, float secPerBeat)
{
  processor.generateRhythm(beats, totalBeats, secPerBeat);
}

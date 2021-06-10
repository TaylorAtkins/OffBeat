/*
  ==============================================================================

    MetronomeComponent.h
    Created: 6 May 2021 1:32:28pm
    Author:  Taylor Atkins

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "AudioManager.h"
#include "Beat.h"

//==============================================================================
/*
*/

class MetronomeComponent : public juce::AnimatedAppComponent
{
public:
  MetronomeComponent();
  ~MetronomeComponent() override;
  void update() override;
  void paint(juce::Graphics &) override;
  void resized() override;
  void setBroadcaster(juce::ChangeBroadcaster *roundBroadcaster, juce::ChangeBroadcaster *onBeatBroadcaster, juce::ChangeBroadcaster *loseBroadcaster, juce::ChangeBroadcaster *offBeatBroadcaster, juce::ChangeBroadcaster *clapBroadcaster);
  void updateSettings(int playerNum, float sensitivity, bool debug);
  void newRhythm();

private:
  AudioManager audioManager;
  Beat *beats;
  int playerNum;
  int totalBeats;
  int currentBeat;
  int previousFrames;
  int minBPM;
  int maxBPM;
  float secPerBeat;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MetronomeComponent)
};

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
  void setBroadcaster(juce::ChangeBroadcaster *roundBroadcaster, juce::ChangeBroadcaster *clapBroadcaster, juce::ChangeBroadcaster *loseBroadcaster, juce::ChangeBroadcaster *offBeatBroadcaster);
  void updateSettings(int playerNum, float sensitivity);
  void newRhythm();
  void mouseDoubleClick(const juce::MouseEvent &event) override;

private:
  int playerNum;
  int totalBeats;
  int currentBeat;
  int previousFrames;
  //int currentPlayer;
  float secPerBeat;
  int minBPM;
  int maxBPM;
  Beat *beats;
  AudioManager audioManager;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MetronomeComponent)
};

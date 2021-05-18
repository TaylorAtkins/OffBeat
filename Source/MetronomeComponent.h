/*
  ==============================================================================

    MetronomeComponent.h
    Created: 6 May 2021 1:32:28pm
    Author:  Taylor Atkins

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AudioManagerComponent.h"
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
  void setPlayerNumber(int playerNum);
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
  AudioManagerComponent audioManager;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MetronomeComponent)
};

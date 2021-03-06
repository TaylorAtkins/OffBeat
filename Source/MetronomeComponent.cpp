#include "MetronomeComponent.h"
#include <iostream>
#include <JuceHeader.h>

MetronomeComponent::MetronomeComponent()
{
  totalBeats = 0;
  currentBeat = 0;
  playerNum = 2;
  secPerBeat = 0.0;
  beats = nullptr;
  previousFrames = 0;
  minBPM = 60;
  maxBPM = 200;

  // Sets number of frames in metrenome animation
  setFramesPerSecond(60);
}

MetronomeComponent::~MetronomeComponent()
{
  if (beats)
    delete[] beats;
}

void MetronomeComponent::setBroadcaster(juce::ChangeBroadcaster *roundBroadcaster, juce::ChangeBroadcaster *onBeatBroadcaster, juce::ChangeBroadcaster *loseBroadcaster, juce::ChangeBroadcaster *offBeatBroadcaster, juce::ChangeBroadcaster *clapBroadcaster)
{
  audioManager.setBroadcaster(roundBroadcaster, onBeatBroadcaster, loseBroadcaster, offBeatBroadcaster, clapBroadcaster);
}

void MetronomeComponent::updateSettings(int playerNum, float sensitivity, bool debug)
{
  this->playerNum = playerNum;
  audioManager.updateSettings(sensitivity, debug);
}

void MetronomeComponent::newRhythm()
{
  if (beats)
    delete[] beats;

  // Randomly set umber of beats in the rhythm
  auto &random = juce::Random::getSystemRandom();
  totalBeats = random.nextInt(juce::Range<int>(playerNum, playerNum * 3));

  beats = new Beat[totalBeats];
  int *colorDisbursement;
  int assignOptions;

  // If there are more beats than players, allow rests to be assigned in rhythm
  if (totalBeats > playerNum)
  {
    assignOptions = playerNum + 1;
    colorDisbursement = new int[assignOptions];
    colorDisbursement[playerNum] = 0;
  }
  else
  {
    assignOptions = playerNum;
    colorDisbursement = new int[assignOptions];
  }

  //Ensure each player is assigned atleast one beat
  for (int i = 0; i < playerNum; ++i)
    colorDisbursement[i] = 1;

  int toBeAssigned = totalBeats - playerNum;

  // Randomly distributes the rest of the beats between the players and rests
  for (int i = 0; i < toBeAssigned; ++i)
  {
    int color = random.nextInt(juce::Range<int>(0, assignOptions));
    ++colorDisbursement[color];
  }

  // Loops through each beat an assigns it a player and color
  for (int i = 0; i < totalBeats; ++i)
  {
    int assignment = random.nextInt(juce::Range<int>(0, assignOptions));
    // If the color selected has already been place for all beats it has been allocated for,
    // check the next color in line to see if it can be assigned to the beat
    while (colorDisbursement[assignment] == 0)
    {
      ++assignment;
      if (assignment >= assignOptions)
        assignment = 0;
    }

    colorDisbursement[assignment] -= 1;

    // Assign each beat a player number and color
    if (assignment == 0)
    {
      beats[i].player = 1;

      // Pink
      beats[i].defaultColor = juce::Colour(0xFFFF658A);
      beats[i].selectedColor = juce::Colour(0xFFB1135F);
    }
    else if (assignment == 1 && playerNum >= 2)
    {
      beats[i].player = 2;

      // Green
      beats[i].defaultColor = juce::Colour(0xFF55FFAA);
      beats[i].selectedColor = juce::Colour(0xFF00A265);
    }
    else if (assignment == 2 && playerNum >= 3)
    {
      beats[i].player = 3;

      // Blue
      beats[i].defaultColor = juce::Colour(0xFF37F7FF);
      beats[i].selectedColor = juce::Colour(0xFF1D6B9F);
    }
    else if (assignment == 3 && playerNum >= 4)
    {
      beats[i].player = 4;

      // Purple
      beats[i].defaultColor = juce::Colour(0xFFC853D8);
      beats[i].selectedColor = juce::Colour(0xFF7F2A8B);
    }
    else
    {
      // Rests
      beats[i].player = 0;

      // Grey
      beats[i].defaultColor = juce::Colour(0xFFBEBEBE);
      beats[i].selectedColor = juce::Colour(0xFF7E7E7E);
    }
  }
  delete[] colorDisbursement;

  previousFrames = getFrameCounter();
  secPerBeat = 60.0 / (float)(random.nextInt(juce::Range<int>(minBPM, maxBPM)));
  currentBeat = 0;
  audioManager.generateRhythm(beats, totalBeats, secPerBeat);
}

void MetronomeComponent::paint(juce::Graphics &g)
{
  if (secPerBeat > 0.0)
  {
    currentBeat = int((getFrameCounter() - previousFrames) / 60.0 / secPerBeat) % totalBeats;
  }

  g.fillAll(juce::Colour(0xFF000000));

  if (beats)
  {
    int spacing = juce::Component::getWidth() / (totalBeats + 1);
    int height = juce::Component::getHeight() / 2;

    // Draw the beats for the metrenome animation
    for (int i = 0; i < totalBeats; ++i)
    {
      if (i == currentBeat)
      {
        // Highlight the current beat
        g.setColour(beats[i].selectedColor);
        g.fillEllipse((i + 1) * spacing - 15, height - 15, 60, 60);
        g.setColour(beats[i].defaultColor);
      }
      else
      {
        g.setColour(beats[i].defaultColor);
      }
      g.fillEllipse((i + 1) * spacing, height, 30, 30);
    }
  }
}

void MetronomeComponent::resized() {}

void MetronomeComponent::update() {}

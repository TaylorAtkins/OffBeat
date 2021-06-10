#pragma once

#include <JuceHeader.h>

// A simple data structure to map a beat of a rhythm to a player and a color.
// This data structure is mostly used by the MetronomeComponent and the
// RhythmProcessor, to represent the current rhythm
struct Beat
{
  int player;
  juce::Colour defaultColor;
  juce::Colour selectedColor;
};

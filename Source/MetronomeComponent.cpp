/*
  ==============================================================================

    MetronomeComponent.cpp
    Created: 6 May 2021 1:32:28pm
    Author:  Taylor Atkins

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MetronomeComponent.h"

//==============================================================================
MetronomeComponent::MetronomeComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setFramesPerSecond (60);
    totalBeats = 0;
    currentBeat = 0;
    playerNum = 2;
    secPerBeat = 0.0;
    beats = nullptr;
}

MetronomeComponent::~MetronomeComponent()
{
  if(beats)
    delete [] beats;
}

void MetronomeComponent::newRhythm(){
  if(beats)
    delete [] beats;

  totalBeats = 4;
  beats = new Beat[totalBeats];
    for( int i = 0; i < totalBeats; ++i){
      if(i % 2 ){
        beats[i].player = 1;
          beats[i].defaultColor = juce::Colours::lightpink;
          beats[i].selectedColor = juce::Colours::white;
      }
      else{
        beats[i].player = 2;
          beats[i].defaultColor = juce::Colours::lightskyblue;
          beats[i].selectedColor = juce::Colours::white;
      }
    }

  secPerBeat = 0.5;
  currentBeat = 0;
}

void MetronomeComponent::update()
{
}

void MetronomeComponent::paint (juce::Graphics& g)
{
    if(secPerBeat > 0.0)
      currentBeat = int(getFrameCounter() / 60.0 / secPerBeat) % totalBeats;
    if(beats)
    {
        for( int i = 0; i < totalBeats; ++i)
        {
            if(i == currentBeat){
                //g.setColour (beats[i].defaultColor);
                //g.fillEllipse((i + 1) * 100, 200, 30, 30);
                g.setColour (beats[i].selectedColor);
            }
            else
                g.setColour (beats[i].defaultColor);
            g.fillEllipse((i + 1) * 100, 200, 30, 30);
        }
    }
}

void MetronomeComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

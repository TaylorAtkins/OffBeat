#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent():juce::Component()
{
    setSize (600, 400);
    addAndMakeVisible(metronome);
    metronome.newRhythm();
}

MainComponent::~MainComponent()
{
}
//==============================================================================

void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::blue);

    //g.setFont (juce::Font (16.0f));
    //g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);A

}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    metronome.setBounds (0, 0, getWidth(), getHeight());
}

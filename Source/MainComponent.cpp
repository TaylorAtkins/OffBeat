#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : juce::Component()
{
    setSize(600, 400);
    addAndMakeVisible(playButton);
    playButton.setButtonText ("Play!");
    playButton.addListener (this);
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFFFFFFF));
   // playButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xFFFF658A));
    playButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF000000));
    //playButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFFFF658A));
    addChildComponent(metronome);
    //addAndMakeVisible(metronome);
    //metronome.newRhythm();
}

MainComponent::~MainComponent()
{
}
//==============================================================================

void MainComponent::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colour(0xFF000000));
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    metronome.setBounds(0, 0, getWidth(), getHeight());
    int buttonHeight = getHeight()/8;
    //playButton.changeWidthToFitText(buttonHeight);
    int buttonWidth = getWidth()/4;
    playButton.setBounds(getWidth() / 2 - buttonWidth/2, getHeight()/ 2 - buttonHeight/2, buttonWidth, buttonHeight);
}

void MainComponent::buttonClicked(juce::Button *button)
{
    // If play button is clicked disable button and enable settings buttons
    if(button == &playButton)
    {
        playButton.setEnabled(false);
        playButton.setVisible(false);
        metronome.setVisible(true);
        metronome.newRhythm();
    }
}

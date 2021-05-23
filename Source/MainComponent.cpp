#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : juce::Component()
{
    setSize(600, 400);
    addAndMakeVisible(playButton);
    playButton.setButtonText("Play");
    playButton.addListener(this);
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFFFFFFF));
    playButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF000000));

    addChildComponent(startButton);
    startButton.setEnabled(false);
    startButton.setVisible(false);
    startButton.setButtonText("Start Game");
    startButton.addListener(this);
    startButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFFFFFFF));
    startButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF000000));

    for (int i = 0; i < 4; ++i)
    {
        addChildComponent(playerNumberButtons[i]);
        playerNumberButtons[i].setEnabled(false);
        playerNumberButtons[i].setVisible(false);
        playerNumberButtons[i].setButtonText(std::to_string(i + 1));
        playerNumberButtons[i].setRadioGroupId(PlayerNumber);
        playerNumberButtons[i].setClickingTogglesState(true);

        playerNumberButtons[i].setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFFFFFFF));
        playerNumberButtons[i].setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF000000));
        playerNumberButtons[i].setColour(juce::TextButton::textColourOnId, juce::Colour(0xFF000000));

        if (i == 0)
        {
            playerNumberButtons[i].onClick = [this]() { playerNum = 1; };
            playerNumberButtons[i].setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xFFFF658A));
        }
        else if (i == 1)
        {
            playerNumberButtons[i].onClick = [this]() { playerNum = 2; };
            playerNumberButtons[i].setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xFF55FFAA));
        }
        else if (i == 2)
        {
            playerNumberButtons[i].onClick = [this]() { playerNum = 3; };
            playerNumberButtons[i].setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xFF37F7FF));
        }
        else
        {
            playerNumberButtons[i].onClick = [this]() { playerNum = 4; };
            playerNumberButtons[i].setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xFFC853D8));
        }
    }
    playerNumberButtons[0].setToggleState(true, juce::dontSendNotification);
    playerNum = 1;

    //addAndMakeVisible(roundBroadcaster);
    roundBroadcaster.addChangeListener(this);

    roundLabel.setJustificationType(juce::Justification::centred);

    roundLabel.setFont(juce::Font(20.0, juce::Font::bold));
    roundLabel.setColour(juce::Label::backgroundColourId, juce::Colour(0xFFFFFFFF));
    // roundLabel.setColour(juce::Label::outlineColourId, juce::Colour(0xFFFF658A));
    roundLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFFF658A));
    rounds = 1;
    addChildComponent(roundLabel);
    // addAndMakeVisible(roundLabel);

    metronome.setBroadcaster(&roundBroadcaster);
    addChildComponent(metronome);
}

MainComponent::~MainComponent()
{
    //delete metronome;
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

    int playButtonHeight = getHeight() / 8;
    int playButtonWidth = getWidth() / 4;
    playButton.setBounds(getWidth() / 2 - playButtonWidth / 2, getHeight() / 2 - playButtonHeight / 2, playButtonWidth, playButtonHeight);

    int startButtonHeight = getHeight() / 8;
    int startButtonWidth = getWidth() / 4;
    startButton.setBounds(getWidth() / 2 - startButtonWidth / 2, getHeight() - (startButtonHeight + 10), startButtonWidth, startButtonHeight);

    int playerButtonHeight = getHeight() / 4;
    int playerButtonWidth = playerButtonHeight;
    int spacing = (getWidth() - playerButtonWidth * 4) / 5;
    for (int i = 0; i < 4; ++i)
    {
        playerNumberButtons[i].setBounds((i + 1) * spacing + i * playerButtonWidth, getHeight() / 2 - playerButtonHeight / 2, playerButtonWidth, playerButtonHeight);
    }

    int roundLabelHeight = getHeight() / 4;
    roundLabel.setBounds(0, getHeight() / 2 - playButtonHeight / 2, getWidth(), roundLabelHeight);
}

void MainComponent::buttonClicked(juce::Button *button)
{
    // If play button is clicked disable button and enable settings buttons
    if (button == &playButton)
    {
        playButton.setEnabled(false);
        playButton.setVisible(false);

        startButton.setEnabled(true);
        startButton.setVisible(true);

        for (int i = 0; i < 4; ++i)
        {
            playerNumberButtons[i].setEnabled(true);
            playerNumberButtons[i].setVisible(true);
        }
    }
    if (button == &startButton)
    {
        startButton.setEnabled(false);
        startButton.setVisible(false);

        for (int i = 0; i < 4; ++i)
        {
            playerNumberButtons[i].setEnabled(false);
            playerNumberButtons[i].setVisible(false);
        }
        showRoundBanner();
        metronome.setPlayerNumber(playerNum);
        metronome.newRhythm();
        roundLabel.setVisible(false);
        metronome.setVisible(true);
        ++rounds;
    }
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    if (source == &roundBroadcaster)
    {
        metronome.setVisible(false);
        showRoundBanner();
        metronome.newRhythm();
        roundLabel.setVisible(false);
        metronome.setVisible(true);
    }
}

void MainComponent::showRoundBanner()
{
    std::string message = "Round " + std::to_string(rounds);
    roundLabel.setText(message, juce::dontSendNotification);
    roundLabel.setVisible(true);
    roundLabel.setVisible(false);
}

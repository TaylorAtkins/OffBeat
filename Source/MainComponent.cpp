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

    addChildComponent(playAgainButton);
    playAgainButton.setEnabled(false);
    playAgainButton.setVisible(false);
    playAgainButton.setButtonText("Play Again");
    playAgainButton.addListener(this);
    playAgainButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFFFFFFF));
    playAgainButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF000000));

    addChildComponent(mainMenuButton);
    mainMenuButton.setEnabled(false);
    mainMenuButton.setVisible(false);
    mainMenuButton.setButtonText("Main Menu");
    mainMenuButton.addListener(this);
    mainMenuButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFFFFFFF));
    mainMenuButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF000000));

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

    addChildComponent(sensitivityLabel);
    sensitivityLabel.setText("Sensitivity", juce::dontSendNotification);
    sensitivityLabel.setColour( juce::Label::outlineColourId, juce::Colour(0xFFFF658A));
    
    addChildComponent(sensitivitySlider);
    sensitivitySlider.setRange(1.0, 10.0, 1);
    sensitivitySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    sensitivitySlider.setColour( juce::Slider::trackColourId, juce::Colour(0xFFFF658A));
    sensitivitySlider.setColour( juce::Slider::thumbColourId, juce::Colour(0xFFFFFFFF));
    sensitivitySlider.setColour( juce::Slider::textBoxOutlineColourId, juce::Colour(0xFFFF658A));
    sensitivitySlider.addListener(this);
    sensitivityLabel.attachToComponent(&sensitivitySlider, true);
    sensitivitySlider.setValue(5.0);
    sensitivitySlider.setTextBoxStyle(juce::Slider::TextBoxRight ,true, sensitivityLabel.getWidth(), sensitivityLabel.getHeight());
    sensitivity = 5.0f;
    
    roundBroadcaster.addChangeListener(this);
    onBeatBroadcaster.addChangeListener(this);
    loseBroadcaster.addChangeListener(this);
    offBeatBroadcaster.addChangeListener(this);

    metronome.setBroadcaster(&roundBroadcaster, &onBeatBroadcaster, &loseBroadcaster, &offBeatBroadcaster);
    addChildComponent(metronome);

    roundLabel.setJustificationType(juce::Justification::centred);
    roundLabel.setFont(juce::Font(20.0, juce::Font::bold));
    roundLabel.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    roundLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFFF658A));
    rounds = 1;
    std::string message = "Round " + std::to_string(rounds);
    roundLabel.setText(message, juce::dontSendNotification);
    addChildComponent(roundLabel);

    scoreLabel.setJustificationType(juce::Justification::centred);
    scoreLabel.setFont(juce::Font(20.0, juce::Font::bold));
    scoreLabel.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    scoreLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFFF658A));
    addChildComponent(scoreLabel);
    
    recordedBeatsLabel.setJustificationType(juce::Justification::centred);
    recordedBeatsLabel.setFont(juce::Font(20.0, juce::Font::bold));
    recordedBeatsLabel.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    recordedBeatsLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFFF658A));
    addChildComponent(recordedBeatsLabel);
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
    metronome.setBounds(0, 0, getWidth(), getHeight());

    int buttonWidth = getWidth() / 4;
    int buttonHeight = getHeight() / 8;
    playButton.setBounds(getWidth() / 2 - buttonWidth / 2, getHeight() / 2 - buttonHeight / 2, buttonWidth, buttonHeight);
    startButton.setBounds(getWidth() / 2 - buttonWidth / 2, getHeight() - (buttonHeight + 10), buttonWidth, buttonHeight);
    playAgainButton.setBounds(buttonWidth * 3 / 4, getHeight() - (buttonHeight + 10), buttonWidth, buttonHeight);
    mainMenuButton.setBounds(buttonWidth * 9 / 4, getHeight() - (buttonHeight + 10), buttonWidth, buttonHeight);
    
    int playerButtonHeight = getHeight() / 4;
    int playerButtonWidth = playerButtonHeight;
    int spacing = (getWidth() - playerButtonWidth * 4) / 5;
    int sliderHeight = getHeight() / 16;
    
    for (int i = 0; i < 4; ++i)
    {
        playerNumberButtons[i].setBounds((i + 1) * spacing + i * playerButtonWidth, getHeight() / 2 - playerButtonHeight / 2 + sliderHeight, playerButtonWidth, playerButtonHeight);
    }
    
    sensitivitySlider.setBounds(getWidth()/4 + sensitivityLabel.getWidth() / 2, getHeight() - (buttonHeight + 20 + sliderHeight), getWidth() / 2 , sliderHeight);
    
    int roundLabelHeight = getHeight() / 4;
    roundLabel.setBounds(0, getHeight() - (roundLabelHeight + 10), getWidth(), roundLabelHeight);
    int clapLabelHeight = getHeight() / 4;
    recordedBeatsLabel.setBounds(0, 0, getWidth(), clapLabelHeight);
    int scoreLabelHeight = getHeight() - (getHeight() / 6);
    scoreLabel.setBounds(0, 0, getWidth(), scoreLabelHeight);
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
        
        sensitivitySlider.setEnabled(true);
        sensitivitySlider.setVisible(true);

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
        sensitivitySlider.setEnabled(false);
        sensitivitySlider.setVisible(false);

        for (int i = 0; i < 4; ++i)
        {
            playerNumberButtons[i].setEnabled(false);
            playerNumberButtons[i].setVisible(false);
        }

        claps = 0;
        offBeats = 0;
        std::string message = "OnBeats " + std::to_string(claps) + " OffBeats: " + std::to_string(offBeats);
        recordedBeatsLabel.setText(message, juce::dontSendNotification);
        recordedBeatsLabel.setVisible(true);

        rounds = 1;
        message = "Round " + std::to_string(rounds);
        roundLabel.setText(message, juce::dontSendNotification);
        roundLabel.setVisible(true);

        metronome.updateSettings(playerNum, sensitivity);
        metronome.newRhythm();
        metronome.setVisible(true);
    }
    if (button == &playAgainButton)
    {
        scoreLabel.setVisible(false);
        recordedBeatsLabel.setVisible(false);
        playAgainButton.setVisible(false);
        playAgainButton.setEnabled(false);
        mainMenuButton.setVisible(false);
        mainMenuButton.setEnabled(false);

        claps = 0;
        offBeats = 0;
        std::string message = "OnBeats " + std::to_string(claps) + " OffBeats: " + std::to_string(offBeats);
        recordedBeatsLabel.setText(message, juce::dontSendNotification);
        recordedBeatsLabel.setVisible(true);

        rounds = 1;
        message = "Round " + std::to_string(rounds);
        roundLabel.setText(message, juce::dontSendNotification);
        roundLabel.setVisible(true);

        metronome.newRhythm();
        metronome.setVisible(true);
    }
    if (button == &mainMenuButton)
    {
        scoreLabel.setVisible(false);
        recordedBeatsLabel.setVisible(false);
        playAgainButton.setVisible(false);
        playAgainButton.setEnabled(false);
        mainMenuButton.setVisible(false);
        mainMenuButton.setEnabled(false);
        playButton.setEnabled(true);
        playButton.setVisible(true);
    }
}

void MainComponent::sliderValueChanged (juce::Slider* slider){
    if(slider == &sensitivitySlider)
    {
        sensitivity = sensitivitySlider.getValue();
    }
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    if (source == &roundBroadcaster)
    {
        metronome.setVisible(false);

        ++rounds;
        std::string message = "Round " + std::to_string(rounds);
        roundLabel.setText(message, juce::dontSendNotification);

        claps = 0;
        offBeats = 0;
        message = "OnBeats " + std::to_string(claps) + " OffBeats: " + std::to_string(offBeats);
        recordedBeatsLabel.setText(message, juce::dontSendNotification);

        metronome.newRhythm();
        metronome.setVisible(true);
    }
    else if (source == &onBeatBroadcaster)
    {
        ++claps;
        std::string message = "OnBeats " + std::to_string(claps) + " OffBeats: " + std::to_string(offBeats);
        recordedBeatsLabel.setText(message, juce::dontSendNotification);
    }
    else if (source == &offBeatBroadcaster)
    {
        ++offBeats;
        std::string message = "OnBeats " + std::to_string(claps) + " OffBeats: " + std::to_string(offBeats);
        recordedBeatsLabel.setText(message, juce::dontSendNotification);
    }
    else if (source == &loseBroadcaster)
    {
        metronome.setVisible(false);
        roundLabel.setVisible(false);
        recordedBeatsLabel.setVisible(true);

        std::string message = "You played too many offBeats!\nYou Lose!\nFinal Score: " + std::to_string(rounds) + " Round/s";
        scoreLabel.setText(message, juce::dontSendNotification);
        scoreLabel.setVisible(true);

        playAgainButton.setEnabled(true);
        playAgainButton.setVisible(true);
        mainMenuButton.setEnabled(true);
        mainMenuButton.setVisible(true);
    }
}

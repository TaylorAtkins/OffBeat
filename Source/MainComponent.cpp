#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : juce::Component()
{
    setSize(600, 400);
    
    // Initialize home screen components
    addAndMakeVisible(playButton);
    playButton.setButtonText("Play");
    playButton.addListener(this);
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFFFFFFF));
    playButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF000000));
    
    addAndMakeVisible(instructionsButton);
    instructionsButton.setButtonText("Instructions");
    instructionsButton.addListener(this);
    instructionsButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFFFFFFF));
    instructionsButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF000000));
    
    // Initialize instruction screen components
    addChildComponent(instructionsLabel);
    instructionsLabel.setJustificationType(juce::Justification::centred);
    instructionsLabel.setFont(juce::Font(17.0, juce::Font::bold));
    instructionsLabel.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    instructionsLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFFF658A));
    std::string instructions = "Instructions\n\tOff-Beat is a 1-4 player rhythm game. This game consist of multiple rounds in which a rhythm will be generated with a random number of beats and a random tempo. The beats are divided randomly between the players. To differentiate the beats in a rhythm, each player will have a unique color and tone assigned to them (pink, green, blue, or purple). Rests may also be present in a rhythm, denoted by a grey color. Before a round begins, the expected rhythm will be played once for the players to listen to. The players are then expected to clap the rhythm 3 times through. While clapping, off-beats and on-beats are tallied to show players how accurate their rhythm clapping is. An off-beat is registered if a player claps on a rest or if more than one player claps or no one claps on any given beat. At the end of the round, if more beats were on-beat than off-beat, the players move on to another round with a new rhythm. If there were more off-beats than on-beats, the game will end and the number of rhythms successfully played will be displayed as a score.";
    instructionsLabel.setText(instructions, juce::dontSendNotification);
    
    addChildComponent(backButton);
    backButton.setButtonText("Main Menu");
    backButton.addListener(this);
    backButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFFFFFFF));
    backButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF000000));

    // Initialize settings screen components
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

    addChildComponent(debugCheckbox);
    debugCheckbox.setButtonText("Debug Mode");
    debugCheckbox.setColour( juce::ToggleButton::tickColourId, juce::Colour(0xFFFF658A));
    debugCheckbox.changeWidthToFitText();
    debug = false;
    
    addChildComponent(sensitivityLabel);
    sensitivityLabel.setText("Sensitivity", juce::dontSendNotification);
    sensitivityLabel.setColour( juce::Label::outlineColourId, juce::Colour(0xFFFF658A));
    sensitivityLabel.attachToComponent(&sensitivitySlider, true);
    
    addChildComponent(sensitivitySlider);
    sensitivitySlider.setRange(1.0, 10.0, 1);
    sensitivitySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    sensitivitySlider.setColour( juce::Slider::trackColourId, juce::Colour(0xFFFF658A));
    sensitivitySlider.setColour( juce::Slider::thumbColourId, juce::Colour(0xFFFFFFFF));
    sensitivitySlider.setColour( juce::Slider::textBoxOutlineColourId, juce::Colour(0xFFFF658A));
    sensitivitySlider.setTextBoxStyle(juce::Slider::TextBoxRight ,true, sensitivityLabel.getWidth(), sensitivityLabel.getHeight());
    sensitivitySlider.addListener(this);
    sensitivitySlider.setValue(5.0);
    sensitivity = 5.0f;

    // Initialize broadcasters to send signals from the rhythm processor
    roundBroadcaster.addChangeListener(this);
    onBeatBroadcaster.addChangeListener(this);
    loseBroadcaster.addChangeListener(this);
    offBeatBroadcaster.addChangeListener(this);
    clapBroadcaster.addChangeListener(this);
    metronome.setBroadcaster(&roundBroadcaster, &onBeatBroadcaster, &loseBroadcaster, &offBeatBroadcaster, &clapBroadcaster);

    // Initialize game screen components
    addChildComponent(metronome);

    addChildComponent(roundLabel);
    roundLabel.setJustificationType(juce::Justification::centred);
    roundLabel.setFont(juce::Font(20.0, juce::Font::bold));
    roundLabel.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    roundLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFFF658A));
    rounds = 1;
    std::string message = "Round " + std::to_string(rounds);
    roundLabel.setText(message, juce::dontSendNotification);
    
    recordedBeatsLabel.setJustificationType(juce::Justification::centred);
    recordedBeatsLabel.setFont(juce::Font(20.0, juce::Font::bold));
    recordedBeatsLabel.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    recordedBeatsLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFFF658A));
    addChildComponent(recordedBeatsLabel);
    
    // Initialize game over screen components
    scoreLabel.setJustificationType(juce::Justification::centred);
    scoreLabel.setFont(juce::Font(20.0, juce::Font::bold));
    scoreLabel.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    scoreLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFFF658A));
    addChildComponent(scoreLabel);
    
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
    
    resized();
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
    int buttonWidth = getWidth() / 4;
    int buttonHeight = getHeight() / 8;
    int heightSpacing = 10;
    
    // Home screen
    playButton.setBounds(getWidth() / 2 - buttonWidth / 2, getHeight() / 2 - buttonHeight - heightSpacing, buttonWidth, buttonHeight);
    instructionsButton.setBounds(getWidth() / 2 - buttonWidth / 2, getHeight() / 2 + heightSpacing, buttonWidth, buttonHeight);
    
    // Instructions Page
    instructionsLabel.setBounds(0, 0, getWidth(), getHeight() * 2 / 3);
    backButton.setBounds(getWidth() / 2 - buttonWidth / 2, getHeight() * 5 / 6 - buttonHeight / 2, buttonWidth, buttonHeight);
    
    // Settings screen
    int playerButtonHeight = getHeight() / 4;
    int playerButtonWidth = playerButtonHeight;
    int widthSpacing = (getWidth() - playerButtonWidth * 4) / 5;
    int sliderHeight = getHeight() / 16;
    
    int checkboxWidth = debugCheckbox.getWidth();
    debugCheckbox.setBounds(getWidth() / 2 - checkboxWidth / 2, getHeight() / 2 - playerButtonHeight / 2 - buttonHeight - heightSpacing, checkboxWidth, buttonHeight);
    
    for (int i = 0; i < 4; ++i)
    {
        playerNumberButtons[i].setBounds((i + 1) * widthSpacing + i * playerButtonWidth, getHeight() / 2 - playerButtonHeight / 2 + sliderHeight, playerButtonWidth, playerButtonHeight);
    }
    
    sensitivitySlider.setBounds(getWidth()/4 + sensitivityLabel.getWidth() / 2, getHeight() - (buttonHeight + 20 + sliderHeight), getWidth() / 2 , sliderHeight);
    
    startButton.setBounds(getWidth() / 2 - buttonWidth / 2, getHeight() - (buttonHeight + heightSpacing), buttonWidth, buttonHeight);
    
    // Game Screen
    int clapLabelHeight = getHeight() / 4;
    recordedBeatsLabel.setBounds(0, 0, getWidth(), clapLabelHeight);
    
    metronome.setBounds(0, 0, getWidth(), getHeight());
    
    int roundLabelHeight = getHeight() / 4;
    roundLabel.setBounds(0, getHeight() - (roundLabelHeight + 10), getWidth(), roundLabelHeight);
    
    // GameOver screen
    int scoreLabelHeight = getHeight() / 4;
    scoreLabel.setBounds(0, getHeight() / 2 - scoreLabelHeight / 2, getWidth(), scoreLabelHeight);
    
    playAgainButton.setBounds(buttonWidth * 3 / 4, getHeight() - (buttonHeight + heightSpacing), buttonWidth, buttonHeight);
    mainMenuButton.setBounds(buttonWidth * 9 / 4, getHeight() - (buttonHeight + heightSpacing), buttonWidth, buttonHeight);
}

void MainComponent::buttonClicked(juce::Button *button)
{
    // If play button is clicked disable button and enable settings buttons
    if (button == &playButton)
    {
        // Make home screen components invisible
        playButton.setEnabled(false);
        playButton.setVisible(false);
        instructionsButton.setEnabled(false);
        instructionsButton.setVisible(false);

        // Make settings screen components visible
        debugCheckbox.setEnabled(true);
        debugCheckbox.setVisible(true);
        sensitivitySlider.setEnabled(true);
        sensitivitySlider.setVisible(true);
        startButton.setEnabled(true);
        startButton.setVisible(true);

        for (int i = 0; i < 4; ++i)
        {
            playerNumberButtons[i].setEnabled(true);
            playerNumberButtons[i].setVisible(true);
        }
    }
    else if (button == &instructionsButton)
    {
        // Make home screen components invisible
        playButton.setEnabled(false);
        playButton.setVisible(false);
        instructionsButton.setEnabled(false);
        instructionsButton.setVisible(false);
        
        // Make instruction screen components visible
        instructionsLabel.setVisible(true);
        backButton.setEnabled(true);
        backButton.setVisible(true);
    }
    else if (button == &backButton)
    {
        // Make instruction screen components invisible
        instructionsLabel.setVisible(false);
        backButton.setEnabled(false);
        backButton.setVisible(false);
        
        // Make home screen components visible
        playButton.setEnabled(true);
        playButton.setVisible(true);
        instructionsButton.setEnabled(true);
        instructionsButton.setVisible(true);
    }
    else if (button == &startButton)
    {
        // Make settings screen components invisible
        debugCheckbox.setEnabled(false);
        debugCheckbox.setVisible(false);
        sensitivitySlider.setEnabled(false);
        sensitivitySlider.setVisible(false);
        startButton.setEnabled(false);
        startButton.setVisible(false);

        for (int i = 0; i < 4; ++i)
        {
            playerNumberButtons[i].setEnabled(false);
            playerNumberButtons[i].setVisible(false);
        }

        // Initilizes game variable
        claps = 0;
        offBeats = 0;
        rounds = 1;
        debug = debugCheckbox.getToggleState();
        metronome.updateSettings(playerNum, sensitivity, debug);
        
        // Make game screen components visible
        std::string message;
        if(debug){
            message = "OnBeats " + std::to_string(onBeats) + " OffBeats: " + std::to_string(offBeats) + "\nClaps: " + std::to_string(claps);
        }
        else{
            message = "OnBeats " + std::to_string(onBeats) + " OffBeats: " + std::to_string(offBeats);
        }        recordedBeatsLabel.setText(message, juce::dontSendNotification);
        recordedBeatsLabel.setVisible(true);

        message = "Round " + std::to_string(rounds);
        roundLabel.setText(message, juce::dontSendNotification);
        roundLabel.setVisible(true);
        
        // Starts game
        metronome.newRhythm();
        metronome.setVisible(true);
    }
    else if (button == &playAgainButton)
    {
        // Make game over screen components invisible
        scoreLabel.setVisible(false);
        recordedBeatsLabel.setVisible(false);
        playAgainButton.setEnabled(false);
        playAgainButton.setVisible(false);
        mainMenuButton.setEnabled(false);
        mainMenuButton.setVisible(false);

        // Resets game variables
        claps = 0;
        offBeats = 0;
        rounds = 1;
        
        // Make game screen components visible
        std::string message;
        if(debug){
            message = "OnBeats " + std::to_string(onBeats) + " OffBeats: " + std::to_string(offBeats) + "\nClaps: " + std::to_string(claps);
        }
        else{
            message = "OnBeats " + std::to_string(onBeats) + " OffBeats: " + std::to_string(offBeats);
        }
        recordedBeatsLabel.setText(message, juce::dontSendNotification);
        recordedBeatsLabel.setVisible(true);

        message = "Round " + std::to_string(rounds);
        roundLabel.setText(message, juce::dontSendNotification);
        roundLabel.setVisible(true);

        // Starts game
        metronome.newRhythm();
        metronome.setVisible(true);
    }
    else if (button == &mainMenuButton)
    {
        // Make game over screen components invisible
        scoreLabel.setVisible(false);
        recordedBeatsLabel.setVisible(false);
        playAgainButton.setEnabled(false);
        playAgainButton.setVisible(false);
        mainMenuButton.setEnabled(false);
        mainMenuButton.setVisible(false);
        
        // Make home screen components visible
        playButton.setEnabled(true);
        playButton.setVisible(true);
        instructionsButton.setEnabled(true);
        instructionsButton.setVisible(true);
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

        // Increment round counter
        ++rounds;
        std::string message = "Round " + std::to_string(rounds);
        roundLabel.setText(message, juce::dontSendNotification);

        // Reset beat counters
        claps = 0;
        onBeats = 0;
        offBeats = 0;
        if(debug){
            message = "OnBeats " + std::to_string(onBeats) + " OffBeats: " + std::to_string(offBeats) + "\nClaps: " + std::to_string(claps);
        }
        else{
            message = "OnBeats " + std::to_string(onBeats) + " OffBeats: " + std::to_string(offBeats);
        }
        recordedBeatsLabel.setText(message, juce::dontSendNotification);

        // Generate new rhythm for next round
        metronome.newRhythm();
        metronome.setVisible(true);
    }
    else if (source == &onBeatBroadcaster)
    {
        // Increments onBeat counter
        ++onBeats;
        
        std::string message;
        if(debug){
            message = "OnBeats " + std::to_string(onBeats) + " OffBeats: " + std::to_string(offBeats) + "\nClaps: " + std::to_string(claps);
        }
        else{
            message = "OnBeats " + std::to_string(onBeats) + " OffBeats: " + std::to_string(offBeats);
        }
        recordedBeatsLabel.setText(message, juce::dontSendNotification);
    }
    else if (source == &offBeatBroadcaster)
    {
        // Increments offBeat counter
        ++offBeats;
        
        std::string message;
        if(debug){
            message = "OnBeats " + std::to_string(onBeats) + " OffBeats: " + std::to_string(offBeats) + "\nClaps: " + std::to_string(claps);
        }
        else{
            message = "OnBeats " + std::to_string(onBeats) + " OffBeats: " + std::to_string(offBeats);
        }
        recordedBeatsLabel.setText(message, juce::dontSendNotification);
    }
    else if (source == &clapBroadcaster)
    {
        // Increments offBeat counter
        ++claps;
        
        std::string message;
        if(debug){
            message = "OnBeats " + std::to_string(onBeats) + " OffBeats: " + std::to_string(offBeats) + "\nClaps: " + std::to_string(claps);
        }
        else{
            message = "OnBeats " + std::to_string(onBeats) + " OffBeats: " + std::to_string(offBeats);
        }
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

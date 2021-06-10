# Off-Beat

## Authors

Taylor Atkins

## Project Description

Off-Beat is a multi-player rhythm game, where players work together to clap a single rhythm. This program is written in C++, using the cross-platform application framework, JUCE. The purpose of this project was to practice generating and processing computer audio. In particular, this project uses simple audio synthesis to generate and play an audio clip similar to a metronome, audio filtering to remove the metronome audio clip’s frequencies from any recorded audio, and audio processing to detect the number of claps in a given timeframe of a recorded audio clip. The algorithm use for detecting a clap was heavily inspired by Algorithm 2 in the following paper: [Clap detection with Microcontroller](https://pub.tik.ee.ethz.ch/students/2013-FS/GA-2013-03.pdf).

## Build Instructions

Currently this program is only supported on the macOS platform. To build the program, the following IDE's must be installed:

- [JUCE](https://juce.com/get-juce)
- [Xcode](https://developer.apple.com/xcode/resources/)

After these IDE's are installed, the project can be built by:

1. Opening the `OffBeat.jucer` project file in Projucer
2. Setting the `Selected exporter` drop-down menu (located at the top center of the Projucer window) to `Xcode (macOS)`
3. Navigating to the `file` menu and selecting `Save Project and Open in IDE...` or pressing Shift-Command-L, to open the project in Xcode
4. Pressing the play button in top left corner of the Xcode window to build and run the program

A more in-depth description for running a Juce project can be found here: [Getting started with the Projucer](https://docs.juce.com/master/tutorial_new_projucer_project.html)

## Testing

Developing tests was difficult for this project because of dependencies within the functions and classes. As a result, tests were not written for this application. However, manual testing did occur. The debugger in Xcode was used to verify the code was behaving properly in important parts of the program, such as rhythm generation and audio processing. A debug mode was added to the application to count anytime a clap was registered by the audio processor, too. This made it possible to judge how accurate the clap detection was during the game. Using the debug mode, the game was ran multiple times using a variety of setting combinations. Every path through the GUI's components were also exercised, to ensure the navigation throughout the pages of the game behaved as expected. Finally, since part of this project required filtering the input audio, it was important to verify the filter was producing an output such that claps could still be identified. To test the affects of various filters, a small python scrip was written to read in a wave file, filter its contents using the same algorithm as off-beat's audio processor, and write the filtered audio out to a new wave file. This script was ran with a sample of input audio from the game, recorded using the audio software Audacity. The resulting wave file was also inspected using Audacity. In this way, the effectiveness of various audio filters could be assessed.

## GUI Operation

### Before Launching the GUI

After building the project, the macOS will show a pop-up window asking to grant the program permission to use the built-in microphone on the device. For this program to work, this permission must be enabled.

### Main Menu

After launching the program, the main menu will appear, showing to buttons: `Instructions` and `Play`. Clicking on the `Instructions` button will take you to a page explaining how to play the game. Clicking on the `Play` button bring you to the settings page for the game.

### Settings Page

The settings page has three options that need to be set before starting the game:

| Settings          | Description                                                                                                                                                                                                                                                                                                                            |
| ----------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Debug Mode        | When debug mode is enable, the number of claps detected by the auidio processor is displayed during the game. By default, Debug mode is not enabled.                                                                                                                                                                                   |
| Number of Players | The player number determines how many distinct colors/tones to use when differentiating between the beats in a rhythm. Each color/tone combo is used to identify which player should clap which beat. The default player number is 1, however this game is recommended to be played with 2-4 people.                                   |
| Sensitivity       | The sensitivity slider affects the minimum amplitute threshold required for a noise to be registered as a clap. The lower the number, the lower the threshhold (i.e. a lower sensitivity means a sound will be more likley to be registered as a clap). The default sensitivity level is 5. This is the recommended sensitivity level. |

Once the preferded settings are selected, the game can be started by clicking the `Start` button.

### Playing the Game

Once the game is started, a random number of circles will appear representing the beats in a rhythm. Each beats in a rhythm, is assigned to a player or assigned as a rest, based off of its color.

| Beat Assignment | Color  |
| --------------- | ------ |
| Rest            | Grey   |
| Player 1        | Pink   |
| Player 2        | Green  |
| Player 3        | Blue   |
| Player 4        | Purple |

When a round begins, the rhythm will be played once for all players to listen to. After the rhythm is played once, the players are expected to clap the rhythm 3 times through. While clapping, off-beats and on-beats will be tallied to show players how accurate their rhythm clapping is. An off-beat is registered if a player claps on a rest or if more than one player claps or no one claps on any given beat. At the end of the round, if more beats were on-beat than off-beat, the players move on to another round with a new rhythm. If there were more off-beats than on-beats, the game will end and the Game Over page will be dispalyed.

### Game Over Page

This page displays the number of On-Beats and Off-Beats clapped during player's losing round, as well as the number of rounds completed. From here, the `Play Again` button can be pressed to play the game again using the previously selected settings, or the `Main Menu` button can be pressed to return to the main menu.

## Project Development

The development of this project was very rocky. I had no prior experience working with the Juce framework and quickly realized that their documentation was not very beginner friendly. The only resources I could find to familarize myself with the framework were tutorials demonstrating a subsection of uses for their modules and the documentation for the classes of the modules together. In the tutorials, I could not find any examples where an application made use of their animation module and their audio processing modules. This made it difficult to decide on an effective way to organize the functionality of the program, an lead to a lot of restructiring through out the developmet process. What resulted was core classes of the program getting nested in each other as member objects. This design made it difficult to make changes within classes (specifically the `RhythmProcessor` class) as they would need to be probagated throughout every other class. The design also made it difficult to test the code, as the classes were so dependant on each other. However, the class structure wasn't the only reason tests were not written for this program. When attempting to tests important features of the codebase, various errors occured because objects sent signals to trigger various events in other sections of the program. Ideally, I would have mocked these objects to fix this issue, but JUCE's UnitTest module did not have the capability to mock objects. I attempted to add a GoogleTest framework to this project, but I had issues getting the project to build. As a result, no unit tests were written for the application.

Implementing the audio processing algorithms for the program, went slightly more smoothly, but I am still not 100% happy with my results. The clap detection wasn't as accurate as I would have liked, which severly impacted the play ability of the game. I realized that the amplitude and duration of a noise was not a unique enough feature to accuratly detect if it was a single clap. Since the game requires that exactly one clap be detected per beat, many beats were falsly marked as off-beats when testing the game. It was difficult to find a good threshhold that would identify most claps but ignore most other noises. However, one thing I was happy with was the FIR filter created to remove unwanted input from the microphone. I used the website [T-Filter](http://t-filter.engineerjs.com/) to generate the filter's coefficents. After testing out the effects of a few different filters, I settled on using a band-pass filter that filtered out frequencies in the range of 0-1000hz and 13000-24000hz. The addition of this filter vastly improved the accuracy of the clap detection.

Overall, this project has a lot of room left for improvment. In the future, I would like to restructure my classes in a more modular fashion to allow for testing. I would also like to continue researching how to incorperate the GoogleTest framework into this project. Since the purpose of the project was focused towards audio processing, I did not spend much time learning about the different GUI components provided by JUCE. I would love to spend time redesigning the GUI layout to have a more consistant style and be more user-friendly. Finally, I would like to add a lot more error handeling to the program. Currently, the program doesn't alert users if an error has occured. For example, if the file containing coefficents for the audio filter failed to be loaded by the program, the game would continue to run, but would not filter the audio input before checking for claps. This would result in an abundancy of false positives when counting claps, making almost every beat be counted as an off-beat. The application even remains scilent when it fails to get permission to access the microphone and speakers of a device. This behavior is not user-friendly and makes it difficult to diagnose problems if the program is not behaving properly.

## License

Off-Beat is licensed under the GNU General Public License v3.0. More information on this license can be found in the [LICENSE](https://github.com/TaylorAtkins/OffBeat/blob/main/LICENSE) file.

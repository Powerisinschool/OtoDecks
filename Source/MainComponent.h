#pragma once

#include <JuceHeader.h>
#include <functional>

#include "DeckGUI.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

    // void play();
    // void pause();
    // void togglePlayPause();
    // void seekTo(double timeInSeconds);

    // double getTrackDuration();
    // double getPlaybackTime();
    // bool isPlaying();

private:
    //==============================================================================
    // Your private member variables go here...
    MixerAudioSource mixerSource;

    DJAudioPlayer player1{"Player 1"};
    DeckGUI deckGUI1{&player1};

    DJAudioPlayer player2{"Player 2"};
    DeckGUI deckGUI2{&player2};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

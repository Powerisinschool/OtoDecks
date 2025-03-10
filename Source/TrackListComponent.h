/*
  ==============================================================================

    TrackListComponent.h
    Created: 10 Mar 2025 4:42:16am
    Author:  Tolulope Olagunju

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class TrackListComponent  : public juce::Component,
                           public Button::Listener,
                           public Timer
{
public:
    TrackListComponent(DJAudioPlayer* player, WaveformDisplay *_waveformDisplay);
    ~TrackListComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button*) override;

    void timerCallback() override;

    std::vector<juce::String> trackTitles;
    std::vector<std::string> trackPaths;

    juce::String previousTrack = "No Track";
    juce::String nextTrack = "No Track";
    juce::String currentTrack = "No Track";

    // Variable to keep track of song list
    int trackCounter = 0;

    // Updates the song names after a track change
    void labelUpdate();

    // Loads the playlist into the deck and make the appropriate changes
    void loadPlaylist(std::vector<juce::String> trackList, std::vector<std::string> pathList);

    // Update Track Time
    void updateTimer(double time);

    // Takes a double time and returns it as a formatted string for timer
    static juce::String convertSecondsToTimer(double time);

    // Actions after an end of track
    void proceedEndOfTrack();

private:
    // Buttons
    juce::TextButton nextButton{ "NEXT" };
    juce::TextButton previousButton{ "PREVIOUS" };
    juce::TextButton shuffleButton{ "SHUFFLE" };
    juce::TextButton repeatButton{ "REPEAT" };

    // Labels
    juce::Label previousLabel;
    juce::Label previousLabel2;
    juce::Label nextLabel;
    juce::Label nextLabel2;
    juce::Label currentTrackNameLabel = currentTrack;
    juce::Label timerLabel;

    DJAudioPlayer* player;
    WaveformDisplay* waveformDisplay;

    // Functions to change songs
    void changeToNextTrack();
    void changeToPreviousTrack();

    // Shuffles the tracklist
    void shuffleList();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackListComponent)
};

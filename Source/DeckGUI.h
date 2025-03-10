/*
  ==============================================================================

    DeckGUI.h
    Created: 28 Feb 2025 4:45:26pm
    Author:  Tolulope Olagunju

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "DJAudioPlayer.h"
#include "PlaylistComponent.h"
#include "TrackListComponent.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                    public Button::Listener,
                    public Slider::Listener,
                    public FileDragAndDropTarget,
                    public Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
           AudioFormatManager & 	formatManagerToUse,
           AudioThumbnailCache & 	cacheToUse );
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** Implementation of Button::Listener */
    void buttonClicked (Button* button) override;

    /** Implementation of Slider::Listener */
    void sliderValueChanged (Slider* slider) override;

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    void updateTrackPosition();
    void loadTrack();
    void timerCallback() override;

private:
    FileChooser fChooser{"Select a file..."};

    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    TextButton loadButton{"LOAD"};
    TextButton playSelectedButton{"PLAY SELECTED TRACK"};
    TextButton muteButton{"MUTE"};
    TextButton skipButton{"SKIP"};

    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;

    DJAudioPlayer *player;
    WaveformDisplay waveformDisplay;
    PlaylistComponent *playlist;
    // std::atomic_bool cancelToken{false};
    // IntervalThread progressThread{&cancelToken, [this]()
    // {
    //     if (player->isPlaying())
    //         updateTrackPosition();
    // }};

    TrackListComponent trackListComponent{ player, &waveformDisplay };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};

/*
  ==============================================================================

    DeckGUI.cpp
    Created: 28 Feb 2025 4:45:26pm
    Author:  Tolulope Olagunju

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player),
               waveformDisplay(formatManagerToUse, cacheToUse)
{
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(playSelectedButton);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(skipButton);
    addAndMakeVisible(loadButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(trackListComponent);
    addAndMakeVisible(waveformDisplay);

    volSlider.setRange(0.0, 2.0); // Enables extra volume to 200% (like VLC)
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);

    volSlider.setValue(player->getGain());
    speedSlider.setValue(player->getSpeed());
    posSlider.setValue(player->getPosition());

    playButton.addListener(this);
    stopButton.addListener(this);
    playSelectedButton.addListener(this);
    muteButton.addListener(this);
    skipButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
};

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (FontOptions (14.0f));
    // g.drawText ("DeckGUI", getLocalBounds(),
    //             Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    const double rowH = getHeight() / 20;
    const double colW = getWidth() / 16;
    playButton.setBounds(colW * 2, rowH,  colW * 5, rowH * 2);
    stopButton.setBounds(colW * 9, rowH,  colW * 5, rowH * 2);
    playSelectedButton.setBounds(colW * 2, rowH * 3, colW * 12, rowH * 2);

    loadButton.setBounds(colW, rowH * 18,  colW * 7, rowH * 1.4);

    muteButton.setBounds(colW, rowH * 6, colW * 4, rowH * 1.5);
    skipButton.setBounds(colW * 11, rowH * 6, colW * 4, rowH * 1.5);

    volSlider.setBounds(colW * 1, rowH * 14, colW * 4, rowH * 4);
    volSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    volSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, colW * 4, rowH * 0.75);

    speedSlider.setBounds(colW * 11, rowH * 14, colW * 4, rowH * 4);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    speedSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, colW * 4, rowH * 0.75);

    // Position Slider (More centered)
    posSlider.setBounds(colW, rowH * 12, colW * 14, rowH);
    posSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, colW * 4, rowH * 0.75);

    trackListComponent.setBounds(colW, rowH * 8, colW * 14, rowH * 3);
    waveformDisplay.setBounds(colW, rowH * 10.5, colW * 14, rowH * 3);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        player->start();
        std::cout << "Play button was clicked!" << std::endl;
    }
    if (button == &stopButton)
    {
        player->stop();
        std::cout << "Stop button was clicked!" << std::endl;
    }
    if (button == &loadButton)
    {
        constexpr auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this] (const FileChooser &_chooser)
        {
            if (const File chosenFile = _chooser.getResult(); chosenFile.exists()){
                player->loadURL(URL{_chooser.getResult()});
                waveformDisplay.loadURL(URL{_chooser.getResult()});

                // Update the Labels
                trackListComponent.currentTrack = _chooser.getResult().getFileName();
                trackListComponent.nextTrack = "No Track";
                trackListComponent.previousTrack = "No Track";
                trackListComponent.labelUpdate();

                // Clean the playlist from the deck
                trackListComponent.trackTitles.clear();
                trackListComponent.trackPaths.clear();
            }
            // updateTrackPosition();
        });
        std::cout << "Load button was clicked!" << std::endl;
    }
    if (button == &playSelectedButton) {
        if (playlist->selectedTrack != "Nothing Selected")
        {
            // Plays the selected track
            player->loadURL(URL{ File{playlist->selectedTrackPath} });
            waveformDisplay.loadURL(URL{File{playlist->selectedTrackPath} });

            // Update the Labels
            trackListComponent.currentTrack = playlist->selectedTrack;
            trackListComponent.nextTrack = "No Track";
            trackListComponent.previousTrack = "No Track";
            trackListComponent.labelUpdate();

            // Clean the playlist from the deck
            trackListComponent.trackTitles.clear();
            trackListComponent.trackPaths.clear();
        }
    }
}

void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &volSlider)
        player->setGain(volSlider.getValue());

    if (slider == &speedSlider)
        player->setSpeed(speedSlider.getValue());

    if (slider == &posSlider)
        player->setPositionRelative(posSlider.getValue());
}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag " << files.size() << std::endl;
    return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped " << files.size() << std::endl;
    if (files.size() > 0)
    {
        const auto file = File{files[0]};
        if (file.getFileExtension() != ".mp3")
        {
            std::cerr << "DeckGUI::filesDropped " << file.getFileName() << " invalid type of " << file.getFileExtension() << std::endl;
        }
        player->loadURL(URL{file});
    }
}

void DeckGUI::updateTrackPosition()
{
    posSlider.setValue(player->getPositionRelative());
}

void DeckGUI::timerCallback()
{
    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());
}


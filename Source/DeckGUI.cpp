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
DeckGUI::DeckGUI(DJAudioPlayer *_player) : player(_player)
{
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    volSlider.setRange(0.0, 2.0); // Enables extra volume to 200% (like VLC)
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);

    volSlider.setValue(player->getGain());
    speedSlider.setValue(player->getSpeed());
    posSlider.setValue(player->getPosition());

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
}

DeckGUI::~DeckGUI()
{
    cancelToken.store (false, std::memory_order_relaxed);
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
    const int rowH = getHeight()/5;
    playButton.setBounds(0, 0,  getWidth()/2, rowH);
    stopButton.setBounds(getWidth()/2, 0,  getWidth()/2, rowH);
    volSlider.setBounds(0, rowH, getWidth(), rowH);
    speedSlider.setBounds(0, rowH * 2, getWidth(), rowH);
    posSlider.setBounds(0, rowH * 3, getWidth(), rowH);
    loadButton.setBounds(0, rowH * 4,  getWidth(), rowH);
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
            player->loadURL(_chooser.getURLResult());
            updateTrackPosition();
        });
        std::cout << "Load button was clicked!" << std::endl;
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


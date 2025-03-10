/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 4 Mar 2025 2:50:11pm
    Author:  Tolulope Olagunju

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(tableComponent);

    tableComponent.getHeader().addColumn("Track title", 1, 666);
    tableComponent.getHeader().addColumn("", 2, 333);
    tableComponent.setModel(this);

    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 3");
    trackTitles.push_back("Track 4");
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::black);
    Rectangle<float> prepareBorder(getWidth() * 0.295, getHeight() * 0.65, getWidth() * 0.41, getHeight() * 0.35);
    g.fillRect(prepareBorder);

    g.setColour(Colours::darkslategrey);
    Rectangle<float> prepareArea(getWidth() * 0.3, getHeight() * 0.68, getWidth() * 0.4, getHeight() * 0.31);
    g.fillRect(prepareArea);

    prepareLabel1.setColour(1, juce::Colours::white);
    prepareLabel1.setFont(20.0f);
    prepareLabel1.setText(selectedTrack, juce::dontSendNotification);
    prepareLabel1.setJustificationType(juce::Justification::centred);
    prepareLabel1.setBounds(getWidth() * 0.25, getHeight() * 0.80, getWidth() * 0.5, getHeight() * 0.1);

    prepareLabel2.setColour(1, juce::Colours::white);
    prepareLabel2.setFont(14.0f);
    prepareLabel2.setText("PREPARED TRACK", juce::dontSendNotification);
    prepareLabel2.setJustificationType(juce::Justification::centred);
    prepareLabel2.setBounds(getWidth() * 0.3, getHeight() * 0.72, getWidth() * 0.4, getHeight() * 0.1);

    // Add - Remove Track Buttons
    addTrackButton.setBounds(getWidth() * 0.72, getHeight() * 0.68, getWidth() * 0.25, getHeight() * 0.1);
    removeTrackButton.setBounds(getWidth() * 0.72, getHeight() * 0.80, getWidth() * 0.25, getHeight() * 0.1);

    // SearchBox and Search Button
    searchBox.setBounds(getWidth() * 0.02, getHeight() * 0.70, getWidth() * 0.25, getHeight() * 0.1);
    searchButton.setBounds(getWidth() * 0.15, getHeight() * 0.82, getWidth() * 0.12, getHeight() * 0.1);

    searchButton.addListener(this);
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    tableComponent.setBounds(getLocalBounds());
}

int PlaylistComponent::getNumRows() {
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics &g, int rowNumber, int width, int height, bool rowIsSelected) {
    // just highlight selected rows
    if (rowIsSelected)
    {
        g.fillAll(Colours::orange);
    }
    else{
        g.fillAll(Colours::darkgrey);
    }
}


void PlaylistComponent::paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    g.drawText (trackTitles[rowNumber], // get the current track title as a cell
    2, 0,
    width - 4, height,
    Justification::centredLeft,
    true);
}

void PlaylistComponent::cellClicked(int rowNumber, int columnId, const MouseEvent &) {
    DBG("A cell was clicked");
    DBG(rowNumber);
    DBG(columnId);
}

Component *PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) {
    if (columnId == 2)
    {
        if (existingComponentToUpdate == nullptr) {
            const auto btn = new TextButton("play");
            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button *button) {
    int id = std::stoi(button->getComponentID().toStdString());
    DBG("PlaylistComponent::buttonClicked " << trackTitles[id]);
}



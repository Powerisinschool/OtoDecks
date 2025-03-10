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
    addAndMakeVisible(addTrackBtn);

    tableComponent.getHeader().addColumn("Track title", 1, 800);
    tableComponent.getHeader().addColumn("", 4, 200);
    tableComponent.setModel(this);

    // tracks.emplace_back("Track 1", nullptr);
    // tracks.emplace_back("Track 2", nullptr);
    // tracks.emplace_back("Track 3", nullptr);
    // tracks.emplace_back("Track 4", nullptr);
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

    // g.setColour (juce::Colours::black);
    // juce::Rectangle<float> prepareBorder(getWidth() * 0.295, getHeight() * 0.65, getWidth() * 0.41, getHeight() * 0.35);
    // g.fillRect(prepareBorder);

    g.setColour(juce::Colours::darkslategrey);
    juce::Rectangle<float> prepareArea(getWidth(), getHeight() * 0.75, getWidth(), getHeight() * 0.25);
    g.fillRect(prepareArea);
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    tableComponent.setBounds(getLocalBounds().withHeight(getHeight() * 0.75));
    addTrackBtn.setBounds(getLocalBounds().withHeight(getHeight() * 0.25).withY(getHeight() * 0.75));
}

int PlaylistComponent::getNumRows() {
    return tracks.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics &g, int rowNumber, int width, int height, bool rowIsSelected) {
    // just highlight selected rows
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::orange);
    }
    else{
        g.fillAll(juce::Colours::darkgrey);
    }
}


void PlaylistComponent::paintCell(juce::Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    if (columnId == 1)
    {
        g.drawText (tracks[rowNumber].first, // get the current track title as a cell
        2, 0,
        width - 4, height,
        juce::Justification::centredLeft,
        true);
    }
}

void PlaylistComponent::cellClicked(int rowNumber, int columnId, const juce::MouseEvent &) {
    selectedTrack = std::make_pair(tracks[rowNumber].first, tracks[rowNumber].second);
}

juce::Component *PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component *existingComponentToUpdate) {
    if (columnId == 2)
    {
        if (existingComponentToUpdate == nullptr) {
            juce::String id{std::to_string(rowNumber)};
            const auto loadDeck1Btn = new juce::TextButton("Load into Deck 1");
            loadDeck1Btn->setComponentID(id);
            loadDeck1Btn->addListener(this);
            existingComponentToUpdate = loadDeck1Btn;
        }
    }
    if (columnId == 3)
    {
        juce::String id{std::to_string(rowNumber)};
        const auto loadDeck2Btn = new juce::TextButton("Load into Deck 2");
        loadDeck2Btn->setComponentID(id);
        loadDeck2Btn->addListener(this);
        existingComponentToUpdate = loadDeck2Btn;
    }
    if (columnId == 4)
    {
        juce::String id{std::to_string(rowNumber)};
        const auto deleteBtn = new juce::TextButton("Delete");
        deleteBtn->setComponentID(id);
        deleteBtn->addListener(this);
        existingComponentToUpdate = deleteBtn;
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button *button) {
    int id = std::stoi(button->getComponentID().toStdString());
    DBG("PlaylistComponent::buttonClicked " << tracks[id].first);
    if (button == &addTrackBtn)
    {
        constexpr auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this] (const FileChooser &_chooser)
        {
            if (const File chosenFile = _chooser.getResult(); chosenFile.exists()){
                tracks.emplace_back(chosenFile.getFileName().toStdString(), URL{_chooser.getResult()});
            }
            // updateTrackPosition()
        });
    }
    if (button->getButtonText().contains("Delete"))
    {
        tracks.erase(tracks.begin() + id);
        tableComponent.updateContent();
        tableComponent.repaint(getLocalBounds());
        return;
    }
    selectedTrack = nullopt;
}

std::optional<std::pair<std::string, URL>> PlaylistComponent::getSelectedTrack() const
{
    return selectedTrack;
}

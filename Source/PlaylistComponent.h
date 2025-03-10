/*
  ==============================================================================

    PlaylistComponent.h
    Created: 4 Mar 2025 2:50:11pm
    Author:  Tolulope Olagunju

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
public juce::TableListBoxModel,
public juce::Button::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows () override;
    void paintRowBackground (juce::Graphics &,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected) override;
    void paintCell (juce::Graphics &,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected) override;
    void cellClicked (int rowNumber, int columnId, const juce::MouseEvent &) override;
    Component* refreshComponentForCell (int rowNumber,
        int columnId,
        bool isRowSelected,
        Component *existingComponentToUpdate) override;

    void buttonClicked(juce::Button* button) override;
    std::optional<std::pair<std::string, URL>> getSelectedTrack() const;

private:
    FileChooser fChooser{"Select a file..."};
    std::vector<std::pair<std::string, URL>> tracks;
    std::optional<std::pair<std::string, URL>> selectedTrack;
    // std::vector<std::string> trackTitles;
    juce::TableListBox tableComponent;
    TextButton addTrackBtn{"Add Track"};

    juce::Label prepareLabel1; // Shows selected Track
    juce::Label prepareLabel2;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};

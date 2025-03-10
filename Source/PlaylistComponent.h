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
                            public TableListBoxModel,
                            public Button::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows () override;
    void paintRowBackground (Graphics &,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected) override;
    void paintCell (Graphics &,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected) override;
    void cellClicked (int rowNumber, int columnId, const MouseEvent &) override;
    Component* refreshComponentForCell (int rowNumber,
        int columnId,
        bool isRowSelected,
        Component *existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;

    juce::String selectedTrackPath;
    juce::String selectedTrack = "Nothing Selected";

private:
    std::vector<std::string> trackTitles;
    TableListBox tableComponent;

    Label prepareLabel1; // Shows selected Track
    Label prepareLabel2;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};

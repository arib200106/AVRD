// PlaylistComponent.h - SIMPLIFIED
#pragma once
#include <JuceHeader.h>

namespace te = tracktion::engine;

class PlaylistComponent : public juce::Component,
                          public juce::ListBoxModel
{
public:
    PlaylistComponent(te::Edit& ed)
        : edit(ed)
    {
        trackListBox = std::make_unique<juce::ListBox>("Track List", this);
        trackListBox->setRowHeight(40);
        addAndMakeVisible(trackListBox.get());
        
        // Add some test tracks
        trackNames.add("Audio Track 1");
        trackNames.add("Audio Track 2");
        trackNames.add("Master Track");
        
        trackListBox->updateContent();
    }
    
    int getNumRows() override
    {
        return trackNames.size();
    }
    
    void paintListBoxItem(int rowNumber, juce::Graphics& g,
                          int width, int height, bool rowIsSelected) override
    {
        if (rowIsSelected)
            g.fillAll(juce::Colours::lightblue);
        
        g.setColour(juce::Colours::white);
        g.setFont(14.0f);
        
        if (rowNumber < trackNames.size())
        {
            g.drawText(trackNames[rowNumber], 
                       10, 0, width - 10, height, 
                       juce::Justification::centredLeft);
        }
    }
    
    void selectedRowsChanged(int lastRowSelected) override
    {
        DBG("Selected track: " + juce::String(lastRowSelected));
    }
    
    void resized() override
    {
        trackListBox->setBounds(getLocalBounds());
    }
    
    void refreshTrackList()
    {
        // Simplified for now
        trackListBox->updateContent();
        repaint();
    }
    
    std::function<void(te::Track*)> onTrackSelected;
    
private:
    te::Edit& edit;
    std::unique_ptr<juce::ListBox> trackListBox;
    juce::StringArray trackNames;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
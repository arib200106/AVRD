// MainComponent.h - COMPLETE WORKING VERSION
#pragma once
#include <JuceHeader.h>
#include "MainMenuBar.h"
#include "TransportComponent.h"
#include "PlaylistComponent.h"
#include "TimelineComponent.h"
#include "DragAndDropComponent.h"

namespace te = tracktion::engine;

class MainComponent : public juce::Component
{
public:
    MainComponent(te::Engine& eng, te::Edit& ed)
        : engine(eng)
        , edit(ed)
        , menuBarComponent(ed, eng)
        , timelineComponent(ed)
        , transportComponent(ed, timelineComponent)
        , playlistComponent(ed)
        , dragDropComponent(ed, eng)
    {
        addAndMakeVisible(menuBarComponent);
        addAndMakeVisible(transportComponent);
        addAndMakeVisible(playlistComponent);
        addAndMakeVisible(timelineComponent);
        addAndMakeVisible(dragDropComponent);
        
        menuBarComponent.onAudioImported = [this]() {
            playlistComponent.refreshTrackList();
        };
        
        dragDropComponent.setOnTrackAdded([this]() {
            playlistComponent.refreshTrackList();
        });
    }
    
    void resized() override
    {
        auto area = getLocalBounds();
        
        menuBarComponent.setBounds(area.removeFromTop(30));
        transportComponent.setBounds(area.removeFromTop(60));
        
        auto leftPanel = area.removeFromLeft(250);
        playlistComponent.setBounds(leftPanel);
        
        timelineComponent.setBounds(area);
        dragDropComponent.setBounds(area);
    }
    
    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0xff1e1e1e));
    }
    
private:
    te::Engine& engine;
    te::Edit& edit;
    
    MainMenuBar menuBarComponent;
    TimelineComponent timelineComponent;
    TransportComponent transportComponent;
    PlaylistComponent playlistComponent;
    DragAndDropComponent dragDropComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
#pragma once
#include <JuceHeader.h>
#include "TrackLane.h"
#include "TimelineHeader.h" // Make sure you created this file!

class PlaylistPanel : public juce::Component, 
                      public juce::Timer // Added for Playhead movement
{
public:
    PlaylistPanel() 
    {
        // 1. Setup UI Hierarchy
        addAndMakeVisible(timelineHeader);
        addAndMakeVisible(viewport);
        viewport.setInterceptsMouseClicks(false, true);
        
        // 2. Create 50 tracks
        for (int i = 0; i < 50; ++i) {
            auto* lane = new TrackLane(i);
            playlistContent.addAndMakeVisible(lane);
            lanes.add(lane);
        }

        viewport.setViewedComponent(&playlistContent, false);
        setWantsKeyboardFocus(true);
        
        // 3. Start the Playhead "Engine" (60 frames per second)
        startTimerHz(60);
    }

    // This runs 60 times a second to move the white line
    void timerCallback() override
    {
        playheadPos += 0.5f;
        if (playheadPos > 3000) playheadPos = 140.0f;
        timelineHeader.setScrollOffset(viewport.getViewPosition().getX());
        repaint();
    }

    void resized() override 
    {
        auto area = getLocalBounds();
        
        // Position Timeline at top
        timelineHeader.setBounds(area.removeFromTop(30)); 

        // Viewport takes rest
        viewport.setBounds(area);

        int baseLaneHeight = 60;
        int currentLaneHeight = static_cast<int>(baseLaneHeight * zoomY);
        
        // Extra 40px height so Track 50 isn't hidden by scrollbar
        int totalHeight = (lanes.size() * currentLaneHeight) + 40; 
        int currentWidth = static_cast<int>(2000 * zoomX);

        playlistContent.setBounds(0, 0, currentWidth, totalHeight);

        for (int i = 0; i < lanes.size(); ++i) {
            lanes[i]->setBounds(0, i * currentLaneHeight, currentWidth, currentLaneHeight);
            lanes[i]->setZoom(zoomX);
        }
        
        timelineHeader.setZoom(zoomX);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black);
        g.setColour(juce::Colours::yellow);
        float viewX = playheadPos - viewport.getViewPosition().getX();
        if (viewX > 140.0f){
            g.drawLine(viewX, 30.0f, viewX, (float)getHeight(), 2.0f); // 2.0f thickness
            juce::Path p;
            p.addTriangle(viewX - 8, 30, viewX + 8, 30, viewX, 45);
            g.fillPath(p);
        }
    }
    
    void paintOverChildren(juce::Graphics& g) override
    {
        // 1. Calculate relative position
        float viewX = playheadPos - viewport.getViewPosition().getX();
        
        // 2. Only draw if it's in the 'Arrangement' area (past 140px header)
        if (viewX > 140.0f && viewX < getWidth()) 
        {
            // Bright Yellow, thicker line
            g.setColour(juce::Colours::yellow);
            g.drawLine(viewX, 0.0f, viewX, (float)getHeight(), 2.5f); 

            // Larger Triangle at the top (on the timeline)
            juce::Path p;
            p.addTriangle(viewX - 10, 0, viewX + 10, 0, viewX, 15);
            g.fillPath(p);
        }
    }

private:

    juce::Viewport viewport;
    juce::Component playlistContent; // The 'Canvas'
    juce::OwnedArray<TrackLane> lanes;
    TimelineHeader timelineHeader;
    
    float zoomX = 1.0f;
    float zoomY = 1.0f;
    float playheadPos = 140.0f;
    float minZoom = 0.2f;
    float maxZoom = 5.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistPanel)
};
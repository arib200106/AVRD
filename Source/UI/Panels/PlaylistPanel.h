#pragma once
#include <JuceHeader.h>
#include "TrackLane.h"
#include "TimelineHeader.h"
#include "Playhead.h"

struct RectangularScrollBarLF : public juce::LookAndFeel_V4
{
    void drawScrollbar (juce::Graphics& g, juce::ScrollBar& sb,
                        int x, int y, int width, int height,
                        bool isVertical, int thumbPosition, int thumbSize,
                        bool isMouseOver, bool isButtonDown) override
    {
        g.setColour(juce::Colours::black.withAlpha(0.2f));
        g.fillRect(x, y, width, height);

        g.setColour(juce::Colours::grey.withAlpha(0.8f));
        if (isVertical)
            g.fillRect(x + 2, thumbPosition, width - 4, thumbSize);
        else
            // Fill full height of the bar to meet the playhead
            g.fillRect(thumbPosition, y, thumbSize, height); 
    }

    void drawScrollbarButton (juce::Graphics& g, juce::ScrollBar& sb,
                              int width, int height, int direction,
                              bool isMouseOver, bool isButtonDown)
    {
    }
};

class PlaylistPanel : public juce::Component, public juce::Timer 
{
public:
    PlaylistPanel() 
    {
        addAndMakeVisible(timelineHeader);
        addAndMakeVisible(viewport);
        addAndMakeVisible(playhead); 
        
        viewport.getHorizontalScrollBar().setLookAndFeel(&customLF);
        viewport.getVerticalScrollBar().setLookAndFeel(&customLF);
        viewport.setInterceptsMouseClicks(false, true);
        
        for (int i = 0; i < 50; ++i) {
            auto* lane = new TrackLane(i);
            playlistContent.addAndMakeVisible(lane);
            lanes.add(lane);
        }

        viewport.setViewedComponent(&playlistContent, false);
        
        // Ensure the component can receive key presses
        setWantsKeyboardFocus(true);
        startTimerHz(60);
    }

    ~PlaylistPanel()
    {
        viewport.getHorizontalScrollBar().setLookAndFeel(nullptr);
        viewport.getVerticalScrollBar().setLookAndFeel(nullptr);
    }

    // --- Play/Stop Logic ---
    void togglePlay() { isPlaying = !isPlaying; }
    
    void stop() { 
        isPlaying = false; 
        playheadPos = 140.0f; 
    }

    bool keyPressed(const juce::KeyPress& key) override
    {
        if (key.getKeyCode() == juce::KeyPress::spaceKey)
        {
            togglePlay();
            return true;
        }
        return false;
    }

    void mouseDown(const juce::MouseEvent& event) override
    {
        grabKeyboardFocus(); // Regain focus so Spacebar works after clicking
    }

    void timerCallback() override
    {
        if (isPlaying) 
        {
            float barWidth = 160.0f * zoomX;
            float loopEnd = 140.0f + (barWidth * 4.0f);

            playheadPos += (1.5f * zoomX);
            
            if (playheadPos >= loopEnd) 
                playheadPos = 140.0f; 
        }

        int scrollX = viewport.getViewPosition().getX();
        timelineHeader.setScrollOffset(scrollX);
        
        // Pass the viewport height so playhead knows where to stop
        playhead.updatePosition(playheadPos, scrollX, viewport.getHeight());
    }

    void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override
    {
        if (event.mods.isCtrlDown()) 
        {
            float dynamicMinX = (float)viewport.getWidth() / 2000.0f;
            zoomX = juce::jlimit(dynamicMinX, 5.0f, zoomX + (wheel.deltaY * 0.1f));
            resized();
        }
        else if (event.mods.isAltDown()) 
        {
            float baseHeight = lanes.size() * 60.0f;
            float dynamicMinY = (float)viewport.getHeight() / baseHeight;
            zoomY = juce::jlimit(dynamicMinY, 5.0f, zoomY + (wheel.deltaY * 0.1f));
            resized();
        }
        else
        {
            viewport.mouseWheelMove(event, wheel);
        }
    }

    void resized() override 
    {
        auto area = getLocalBounds();
        playhead.setBounds(area); 

        timelineHeader.setBounds(area.removeFromTop(30)); 
        viewport.setBounds(area);

        int baseLaneHeight = 60;
        int currentLaneHeight = static_cast<int>(baseLaneHeight * zoomY);
        int totalHeight = (lanes.size() * currentLaneHeight) + 40; 
        int currentWidth = static_cast<int>(2000 * zoomX);

        playlistContent.setBounds(0, 0, currentWidth, totalHeight);

        for (int i = 0; i < lanes.size(); ++i) {
            lanes[i]->setBounds(0, i * currentLaneHeight, currentWidth, currentLaneHeight);
            lanes[i]->setZoom(zoomX);
        }
        timelineHeader.setZoom(zoomX);
    }

    void paint(juce::Graphics& g) override { g.fillAll(juce::Colours::black); }

private:
    juce::Viewport viewport;
    juce::Component playlistContent; 
    juce::OwnedArray<TrackLane> lanes;
    TimelineHeader timelineHeader;
    Playhead playhead;
    
    RectangularScrollBarLF customLF;

    float zoomX = 1.0f;
    float zoomY = 1.0f;
    float playheadPos = 140.0f;
    bool isPlaying = false; // Initial state
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistPanel)
};
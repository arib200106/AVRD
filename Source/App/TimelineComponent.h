// TimelineComponent.h - UPDATED with moving playhead
#pragma once
#include <JuceHeader.h>
#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class TimelineComponent : public juce::Component,
                          private juce::ScrollBar::Listener,
                          private juce::Timer  // ADD THIS for animation
{
public:
    TimelineComponent(te::Edit& ed)
        : edit(ed),
          horizontalScrollbar(true),
          verticalScrollbar(false)
    {
        // Grid settings
        snapToGrid = true;
        gridSize = 16;
        pixelsPerBeat = 50.0f;
        currentPosition = 0.0;  // Start at bar 1
        
        // Set up scrollbars
        horizontalScrollbar.setRangeLimits(0.0, 1000.0);
        horizontalScrollbar.setCurrentRange(0.0, 100.0);
        horizontalScrollbar.addListener(this);
        addAndMakeVisible(horizontalScrollbar);
        
        verticalScrollbar.setRangeLimits(0.0, 1000.0);
        verticalScrollbar.setCurrentRange(0.0, 100.0);
        verticalScrollbar.addListener(this);
        addAndMakeVisible(verticalScrollbar);
        
        // START TIMER for playhead animation (60 FPS)
        startTimerHz(60);
    }
    
    ~TimelineComponent()
    {
        stopTimer();
    }
    
    void paint(juce::Graphics& g) override
    {
        // Fill background
        g.fillAll(juce::Colour(0xff333333));
        
        auto area = getLocalBounds();
        area.removeFromBottom(horizontalScrollbar.getHeight());
        area.removeFromRight(verticalScrollbar.getWidth());
        
        // Draw grid lines
        drawGrid(g, area);
        
        // Draw timeline ruler (top bar)
        drawTimeRuler(g, area);
        
        // Draw playhead (MOVING!)
        drawPlayhead(g, area);
    }
    
    void resized() override
    {
        auto area = getLocalBounds();
        
        // Horizontal scrollbar at bottom
        horizontalScrollbar.setBounds(area.removeFromBottom(16));
        
        // Vertical scrollbar at right
        verticalScrollbar.setBounds(area.removeFromRight(16));
        
        // Timeline area is the rest
        timelineArea = area;
        
        repaint();
    }
    
    // Scrollbar listener
    void scrollBarMoved(juce::ScrollBar* scrollBar, double newRangeStart)
    {
        repaint();
    }
    
    // Timer callback - updates playhead position
    void timerCallback() override
    {
        // Update playhead position if playing
        if (isPlaying)
        {
            // Move playhead forward (120 BPM = 2 beats per second)
            // 4 beats per bar, so 0.5 bars per second at 120 BPM
            currentPosition += 0.5 / 60.0;  // Update based on timer rate (60Hz)
            
            // Loop back to beginning if past 16 bars
            if (currentPosition >= 16.0)
                currentPosition = 0.0;
                
            repaint();
        }
    }
    
    // Playhead control methods
    void play() { isPlaying = true; }
    void stop() { isPlaying = false; currentPosition = 0.0; repaint(); }
    void pause() { isPlaying = false; }
    
    void setPosition(double bars) { currentPosition = bars; repaint(); }
    double getPosition() const { return currentPosition; }
    
    // Zoom controls
    void zoomIn() { pixelsPerBeat *= 1.2f; repaint(); }
    void zoomOut() { pixelsPerBeat /= 1.2f; repaint(); }
    void setSnapToGrid(bool snap) { snapToGrid = snap; repaint(); }
    
private:
    te::Edit& edit;
    juce::ScrollBar horizontalScrollbar;
    juce::ScrollBar verticalScrollbar;
    juce::Rectangle<int> timelineArea;
    
    // Grid settings
    bool snapToGrid;
    int gridSize;
    float pixelsPerBeat;
    
    // Playhead state
    bool isPlaying = false;
    double currentPosition;  // Position in bars
    
    void drawGrid(juce::Graphics& g, juce::Rectangle<int> area)
    {
        // Draw bars (heavier lines)
        g.setColour(juce::Colour(0xff555555));
        for (int bar = 0; bar < 100; ++bar) {
            float x = bar * 4 * pixelsPerBeat - (float)horizontalScrollbar.getCurrentRangeStart();
            if (x >= 0 && x < area.getWidth()) {
                g.drawLine(x, 0, x, area.getHeight(), 2.0f);
                
                // Bar number
                g.setColour(juce::Colours::lightgrey);
                g.setFont(12.0f);
                g.drawText(juce::String(bar + 1), 
                          (int)x + 2, area.getY() + 2, 30, 20, 
                          juce::Justification::centredLeft);
                g.setColour(juce::Colour(0xff555555));
            }
        }
        
        // Draw beats (medium lines)
        g.setColour(juce::Colour(0xff444444));
        for (int beat = 0; beat < 400; ++beat) {
            float x = beat * pixelsPerBeat - (float)horizontalScrollbar.getCurrentRangeStart();
            if (x >= 0 && x < area.getWidth() && (beat % 4 != 0)) {
                g.drawLine(x, 30, x, area.getHeight(), 1.0f);
            }
        }
        
        // Draw grid divisions (thin lines)
        g.setColour(juce::Colour(0xff3a3a3a));
        for (int division = 0; division < 1600; ++division) {
            float x = division * (pixelsPerBeat / 4.0f) - (float)horizontalScrollbar.getCurrentRangeStart();
            if (x >= 0 && x < area.getWidth() && (division % 4 != 0)) {
                g.drawLine(x, 60, x, area.getHeight(), 0.5f);
            }
        }
    }
    
    void drawTimeRuler(juce::Graphics& g, juce::Rectangle<int> area)
    {
        // Ruler background
        g.setColour(juce::Colour(0xff222222));
        g.fillRect(area.getX(), area.getY(), area.getWidth(), 30);
        
        // Ruler border
        g.setColour(juce::Colours::grey);
        g.drawLine(area.getX(), area.getY() + 30, area.getRight(), area.getY() + 30, 1.0f);
        
        // Time markers
        g.setColour(juce::Colours::white);
        g.setFont(11.0f);
        
        for (int bar = 0; bar < 100; ++bar) {
            float x = bar * 4 * pixelsPerBeat - (float)horizontalScrollbar.getCurrentRangeStart();
            if (x >= area.getX() && x < area.getRight()) {
                // Draw bar:beat time
                juce::String timeText = juce::String(bar + 1) + ":1";
                g.drawText(timeText, 
                          (int)x + 5, area.getY() + 5, 40, 20, 
                          juce::Justification::centredLeft);
            }
        }
    }
    
    void drawPlayhead(juce::Graphics& g, juce::Rectangle<int> area)
    {
        // Convert bars to pixels
        float x = currentPosition * 4 * pixelsPerBeat - (float)horizontalScrollbar.getCurrentRangeStart();
        
        if (x >= area.getX() && x < area.getRight()) {
            // Playhead line (red when playing, yellow when stopped)
            g.setColour(isPlaying ? juce::Colours::red : juce::Colours::yellow);
            g.drawLine(x, area.getY(), x, area.getBottom(), 2.0f);
            
            // Playhead triangle
            juce::Path triangle;
            triangle.addTriangle(x - 5, area.getY(), 
                                x + 5, area.getY(), 
                                x, area.getY() + 10);
            g.fillPath(triangle);
            
            // Position label
            g.setColour(juce::Colours::white);
            g.setFont(10.0f);
            juce::String posText = juce::String(currentPosition, 2) + " bars";
            g.drawText(posText, x + 10, area.getY() + 5, 60, 15, juce::Justification::centredLeft);
        }
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineComponent)
};
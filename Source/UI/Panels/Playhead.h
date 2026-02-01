#pragma once
#include <JuceHeader.h>

class Playhead : public juce::Component
{
public:
    Playhead() { setInterceptsMouseClicks(false, false); }

    void updatePosition(float newX, int scrollOffset, int vPortHeight)
    {
        playheadPos = newX;
        currentScroll = scrollOffset;
        viewportHeight = vPortHeight; // Track how tall the viewport is
        repaint();
    }

    void paint(juce::Graphics& g) override
    {
        float viewX = playheadPos - (float)currentScroll;

        if (viewX > 140.0f)
        {
            g.setColour(juce::Colours::yellow);
            
            // Draw from y=0 down to the top of the horizontal scrollbar (y=30 + vPortHeight)
            // Since playhead starts at y=0 of the WHOLE panel, and timeline is 30px:
            float lineTop = 30.0f; 
            float lineBottom = 30.0f + (float)viewportHeight;
            
            g.drawLine(viewX, lineTop, viewX, lineBottom, 3.0f);

            juce::Path p;
            p.addTriangle(viewX - 10.0f, lineTop, viewX + 10.0f, lineTop, viewX, lineTop + 15.0f);
            g.fillPath(p);
        }
    }

private:
    float playheadPos = 140.0f;
    int currentScroll = 0;
    int viewportHeight = 0;
};
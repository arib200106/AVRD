// TransportComponent.h - UPDATED
#pragma once
#include <JuceHeader.h>
#include <tracktion_engine/tracktion_engine.h>
#include "TimelineComponent.h"  // Need this to control playhead

namespace te = tracktion_engine;

class TransportComponent : public juce::Component
{
public:
    TransportComponent(te::Edit& ed, TimelineComponent& timeline)
        : edit(ed),
          timelineComp(timeline)  // Store reference to timeline
    {
        playButton = std::make_unique<juce::TextButton>("Play");
        playButton->onClick = [this] { 
            edit.getTransport().play(false); 
            timelineComp.play();  // Start timeline playhead
        };
        addAndMakeVisible(playButton.get());
        
        stopButton = std::make_unique<juce::TextButton>("Stop");
        stopButton->onClick = [this] { 
            edit.getTransport().stop(false, false); 
            timelineComp.stop();  // Stop timeline playhead
        };
        addAndMakeVisible(stopButton.get());
        
        recordButton = std::make_unique<juce::TextButton>("Record");
        recordButton->onClick = [this] { 
            edit.getTransport().record(false); 
            // We could add recording visualization later
        };
        addAndMakeVisible(recordButton.get());
        
        // Add loop button
        loopButton = std::make_unique<juce::TextButton>("Loop");
        loopButton->onClick = [this] { 
            isLooping = !isLooping;
            loopButton->setToggleState(isLooping, juce::dontSendNotification);
            // We'll implement looping later
        };
        loopButton->setClickingTogglesState(true);
        addAndMakeVisible(loopButton.get());
    }
    
    void resized() override
    {
        auto area = getLocalBounds();
        playButton->setBounds(area.removeFromLeft(80).reduced(5));
        stopButton->setBounds(area.removeFromLeft(80).reduced(5));
        recordButton->setBounds(area.removeFromLeft(80).reduced(5));
        loopButton->setBounds(area.removeFromLeft(80).reduced(5));
    }
    
private:
    te::Edit& edit;
    TimelineComponent& timelineComp;  // Reference to timeline
    
    std::unique_ptr<juce::TextButton> playButton;
    std::unique_ptr<juce::TextButton> stopButton;
    std::unique_ptr<juce::TextButton> recordButton;
    std::unique_ptr<juce::TextButton> loopButton;
    
    bool isLooping = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TransportComponent)
};
#pragma once
#include <JuceHeader.h>
#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class DragAndDropComponent : public juce::Component,
                             public juce::FileDragAndDropTarget
{
public:
    DragAndDropComponent(te::Edit& ed, te::Engine& eng)
        : edit(ed), engine(eng)
    {
        // Make sure this component is transparent to mouse clicks 
        // if you want to click the timeline underneath
        setInterceptsMouseClicks(false, true);
    }

    // FileDragAndDropTarget implementation
    bool isInterestedInFileDrag(const juce::StringArray& files) override
    {
        return true; 
    }

    void filesDropped(const juce::StringArray& files, int x, int y) override
    {
        for (auto file : files)
        {
            auto f = juce::File(file);
            if (f.existsAsFile())
            {
                // Logic to add track to Tracktion Engine
                if (onTrackAdded)
                    onTrackAdded();
            }
        }
    }

    void setOnTrackAdded(std::function<void()> callback) { onTrackAdded = callback; }

private:
    te::Edit& edit;
    te::Engine& engine;
    std::function<void()> onTrackAdded;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DragAndDropComponent)
};
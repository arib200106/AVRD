// AudioSettingsComponent.h - FIXED
#pragma once
#include <JuceHeader.h>
#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class AudioSettingsComponent : public juce::Component
{
public:
    AudioSettingsComponent(te::Engine& eng)
        : engine(eng)
    {
        // Create audio device selector
        auto& deviceManager = engine.getDeviceManager().deviceManager;
        audioDeviceSelector = std::make_unique<juce::AudioDeviceSelectorComponent>(
            deviceManager,
            0, 256,    // min/max audio inputs
            0, 256,    // min/max audio outputs
            true,      // show MIDI inputs
            true,      // show MIDI outputs
            true,      // show channels
            false      // show advanced options
        );
        
        addAndMakeVisible(audioDeviceSelector.get());
        
        // Add a title label
        titleLabel = std::make_unique<juce::Label>("title", "Audio & MIDI Settings");
        titleLabel->setFont(juce::Font(16.0f, juce::Font::bold));
        titleLabel->setColour(juce::Label::textColourId, juce::Colours::white);
        addAndMakeVisible(titleLabel.get());
    }
    
    void resized() override
    {
        auto area = getLocalBounds();
        
        // Title at top (30px)
        titleLabel->setBounds(area.removeFromTop(30).reduced(5));
        
        // Audio device selector takes the rest
        audioDeviceSelector->setBounds(area);
    }
    
    // REMOVE the refreshDeviceList() method - it doesn't exist in JUCE
    // void refreshDeviceList() { ... }
    
private:
    te::Engine& engine;
    std::unique_ptr<juce::AudioDeviceSelectorComponent> audioDeviceSelector;
    std::unique_ptr<juce::Label> titleLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioSettingsComponent)
};
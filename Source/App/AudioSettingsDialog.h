// AudioSettingsDialog.h - FIXED
#pragma once
#include <JuceHeader.h>
#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class AudioSettingsDialog : public juce::DialogWindow
{
public:
    AudioSettingsDialog(te::Engine& eng)
        : DialogWindow("Audio & MIDI Settings", 
                      juce::Colours::darkgrey,
                      true, // add title bar
                      true) // add close button
    {
        setUsingNativeTitleBar(true);
        setResizable(true, true);
        setContentOwned(new AudioSettingsContent(eng), true);
        
        centreWithSize(600, 500);
        setVisible(true);
    }
    
    void closeButtonPressed() override
    {
        exitModalState(0);
    }
    
private:
    class AudioSettingsContent : public juce::Component
    {
    public:
        AudioSettingsContent(te::Engine& eng)
            : engine(eng)
        {
            // Audio device selector
            auto& deviceManager = engine.getDeviceManager().deviceManager;
            audioDeviceSelector = std::make_unique<juce::AudioDeviceSelectorComponent>(
                deviceManager,
                0, 256,
                0, 256,
                true,  // MIDI inputs
                true,  // MIDI outputs
                true,  // channels
                false  // advanced
            );
            addAndMakeVisible(audioDeviceSelector.get());
            
            // Title
            titleLabel = std::make_unique<juce::Label>("title", "Audio & MIDI Configuration");
            titleLabel->setFont(juce::Font(18.0f, juce::Font::bold));
            titleLabel->setColour(juce::Label::textColourId, juce::Colours::white);
            addAndMakeVisible(titleLabel.get());
            
            // Apply button
            applyButton = std::make_unique<juce::TextButton>("Apply");
            applyButton->onClick = [this] { applySettings(); };
            addAndMakeVisible(applyButton.get());
            
            // Close button
            closeButton = std::make_unique<juce::TextButton>("Close");
            closeButton->onClick = [this] { 
                juce::DialogWindow* dw = findParentComponentOfClass<juce::DialogWindow>();
                if (dw) dw->exitModalState(0);
            };
            addAndMakeVisible(closeButton.get());
        }
        
        void resized() override
        {
            auto area = getLocalBounds().reduced(10);
            
            // Title at top
            titleLabel->setBounds(area.removeFromTop(30));
            area.removeFromTop(10);
            
            // Audio selector takes most space
            audioDeviceSelector->setBounds(area.removeFromTop(area.getHeight() - 50));
            
            // Buttons at bottom
            auto buttonArea = area.removeFromTop(40);
            applyButton->setBounds(buttonArea.removeFromLeft(80).reduced(5));
            closeButton->setBounds(buttonArea.removeFromLeft(80).reduced(5));
        }
        
    private:
        te::Engine& engine;
        std::unique_ptr<juce::AudioDeviceSelectorComponent> audioDeviceSelector;
        std::unique_ptr<juce::Label> titleLabel;
        std::unique_ptr<juce::TextButton> applyButton;
        std::unique_ptr<juce::TextButton> closeButton;
        
        void applySettings()
        {
            // Settings are automatically applied by the AudioDeviceSelectorComponent
            juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::InfoIcon, 
                                                  "Settings Applied", 
                                                  "Audio settings have been applied.");
        }
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioSettingsContent)
    };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioSettingsDialog)
};
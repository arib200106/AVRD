// MainMenuBar.h - CLEAN VERSION
#pragma once
#include <JuceHeader.h>
#include "AudioImportManager.h"

namespace te = tracktion::engine;

class MainMenuBar : public juce::Component,
                    public juce::MenuBarModel
{
public:
    MainMenuBar(te::Edit& ed, te::Engine& eng)
        : edit(ed)
        , engine(eng)
        , audioImportManager(ed, eng)
    {
        fileMenu.addItem("File", []() {});
        fileMenu.addItem("Import Audio...", [this]() { importAudio(); });
        fileMenu.addSeparator();
        fileMenu.addItem("Exit", []() { juce::JUCEApplication::getInstance()->systemRequestedQuit(); });
        
        addAndMakeVisible(menuBar);
        menuBar.setModel(this);
    }
    
    ~MainMenuBar()
    {
        menuBar.setModel(nullptr);
    }
    
    juce::StringArray getMenuBarNames() override
    {
        return { "File" };
    }
    
    juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String&) override
    {
        if (topLevelMenuIndex == 0)
            return fileMenu;
        return juce::PopupMenu();
    }
    
    void menuItemSelected(int, int) override {}
    
    void importAudio()
    {
        audioImportManager.importAudioFile();
        if (onAudioImported)
            onAudioImported();
    }
    
    void resized() override
    {
        menuBar.setBounds(getLocalBounds());
    }
    
    std::function<void()> onAudioImported;
    AudioImportManager& getAudioImportManager() { return audioImportManager; }
    
private:
    te::Edit& edit;
    te::Engine& engine;
    juce::MenuBarComponent menuBar;
    AudioImportManager audioImportManager;
    juce::PopupMenu fileMenu;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainMenuBar)
};
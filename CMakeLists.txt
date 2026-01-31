cmake_minimum_required(VERSION 3.22)

project(AVRD VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tracktion Engine Setup
set(TRACKTION_ENGINE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(TRACKTION_ENGINE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(TRACKTION_ENGINE_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(TRACKTION_ENGINE_BUILD_DEMOS OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/tracktion_engine)

juce_add_gui_app(AVRD
    PRODUCT_NAME "AVRD"
)

target_sources(AVRD PRIVATE
    Source/App/main.cpp
    Source/App/MainWindow.h
    Source/App/MainComponent.h
    Source/App/MenuBarComponent.h      
    Source/App/AudioImportManager.h  
    Source/App/DragAndDropComponent.h
    Source/App/TransportComponent.h
    Source/App/PlaylistComponent.h
    Source/App/TimelineComponent.h
)

target_include_directories(AVRD PRIVATE 
    Source 
    Source/App
)

# Link ALL required libraries
target_link_libraries(AVRD PRIVATE
    tracktion_engine
    tracktion_graph
    tracktion_core
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
)

juce_generate_juce_header(AVRD)
cmake_minimum_required(VERSION 3.22)

project(AVRD VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
# 1. FORCE DISABLE TRACKTION EXTRAS (The fix for your error)
# -----------------------------------------------------------------------------
set(TRACKTION_ENGINE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(TRACKTION_ENGINE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(TRACKTION_ENGINE_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(TRACKTION_ENGINE_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(TRACKTION_ENGINE_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)

# Add the library - it will now only provide modules, not build its own apps
add_subdirectory(libs/tracktion_engine)

# -----------------------------------------------------------------------------
# 2. AVRD Application Setup
# -----------------------------------------------------------------------------
juce_add_gui_app(AVRD
    PRODUCT_NAME "AVRD"
    VERSION "0.0.1"
)

# -----------------------------------------------------------------------------
# 3. Pure UI Source Files
# -----------------------------------------------------------------------------
target_include_directories(AVRD PRIVATE 
    Source
    Source/App
    Source/UI
    Source/Controller
    Source/Utils
    Source/UI/Panels
)

target_sources(AVRD PRIVATE
    Source/App/main.cpp
)

# -----------------------------------------------------------------------------
# 4. Minimal Linking (Only what's needed for UI)
# -----------------------------------------------------------------------------
target_link_libraries(AVRD PRIVATE
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_graphics
    juce::juce_events
    juce::juce_core
    juce::juce_data_structures
)

juce_generate_juce_header(AVRD)
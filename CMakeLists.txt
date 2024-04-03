
cmake_minimum_required(VERSION 3.15)
project(tap_webview_plugin_template VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)



# ------------------------------------------------------------------------------
# Fetch CPM.cmake
# ------------------------------------------------------------------------------
set(CPM_DOWNLOAD_VERSION 0.34.0)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

# Import CPM for package management
if (NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
    message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
    file(DOWNLOAD https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif ()

include(${CPM_DOWNLOAD_LOCATION})

# Directories
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")

CPMAddPackage(
        NAME juce
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG 7.0.9
        GIT_SHALLOW ON
)

CPMAddPackage(
        NAME choc
        GIT_REPOSITORY https://github.com/Tracktion/choc.git
        GIT_TAG 62d0e0cfef837f8cd6b162c252fbb2f79ddba2e5
)

juce_add_binary_data(Data
        HEADER_NAME "Data.h"
        NAMESPACE "data"
        SOURCES
            WebView/gain_view.html
)

juce_add_plugin(${PROJECT_NAME}
        VERSION 0.0.1
        COMPANY_NAME TheAudioProgrammer
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS TRUE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Tap1
        PLUGIN_CODE Tap1
        FORMATS AU VST3 Standalone
        PRODUCT_NAME "TAP Webview Plugin Template"
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
        JUCE_VST3_CAN_REPLACE_VST2=0
)

file(GLOB_RECURSE SourceFiles CONFIGURE_DEPENDS "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.h")
target_sources("${PROJECT_NAME}" PRIVATE ${SourceFiles})

# Choc is header only with no CMakeLists, so this is how we include it in our project
target_include_directories(${PROJECT_NAME} PRIVATE ${choc_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        juce::juce_analytics
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_core
        juce::juce_data_structures
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        Data
)

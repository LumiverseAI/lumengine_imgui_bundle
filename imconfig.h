//-----------------------------------------------------------------------------
// DEAR IMGUI COMPILE-TIME OPTIONS
// Runtime options (clipboard callbacks, enabling various features, etc.) can
// generally be set via the ImGuiIO structure.
//-----------------------------------------------------------------------------
// You need to make sure that configuration settings are defined consistently
// _everywhere_ Dear ImGui is used, which include the imgui*.cpp files but also
// _any_ of your code that uses Dear ImGui. This is because some compile-time
// options have an affect on data structures.
//-----------------------------------------------------------------------------

#pragma once

//---- Define attributes of all API symbols declarations for DLL
// Lumengine ImGui Bundle: Built as a single shared library (DLL)
// Using Dear ImGui via a shared library is not recommended by ImGui authors,
// but necessary for sharing context across modules in Lumengine.

#if defined(_WIN32) || defined(_WIN64)
    // Windows DLL export/import
    #ifdef imgui_bundle_EXPORTS
        // Building the DLL - export symbols
        #define IMGUI_API __declspec(dllexport)
    #else
        // Using the DLL - import symbols
        #define IMGUI_API __declspec(dllimport)
    #endif
#elif defined(__GNUC__) && __GNUC__ >= 4
    // GCC/Clang on Linux/macOS with visibility support
    #ifdef imgui_bundle_EXPORTS
        #define IMGUI_API __attribute__((visibility("default")))
    #else
        #define IMGUI_API
    #endif
#else
    // Other platforms - no special decoration
    #define IMGUI_API
#endif

// ImPlot uses the same API macro as ImGui
#ifndef IMPLOT_API
#define IMPLOT_API IMGUI_API
#endif

// ImPlot3D uses the same API macro as ImGui
#ifndef IMPLOT3D_API
#define IMPLOT3D_API IMGUI_API
#endif

// imgui-node_editor uses the same API macro as ImGui
#ifndef IMGUI_NODE_EDITOR_API
#define IMGUI_NODE_EDITOR_API IMGUI_API
#endif

//---- Use 32-bit vertex indices (default is 16-bit) to allow large meshes with more than 64K vertices.
// Your renderer backend will need to support it (most example renderer backends support both 16/32-bit indices).
//#define ImDrawIdx unsigned int

//---- Define IMGUI_DEFINE_MATH_OPERATORS to use Dear ImGui's own very basic math operators.
#define IMGUI_DEFINE_MATH_OPERATORS

//---- Don't define obsolete functions/enums/behaviors. Consider enabling from time to time after updating.
//#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS

//---- Disable demo windows and debug tools (not recommended during development)
//#define IMGUI_DISABLE_DEMO_WINDOWS
//#define IMGUI_DISABLE_DEBUG_TOOLS

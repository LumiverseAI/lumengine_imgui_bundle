#pragma once
// Relay between Dear ImGui's test-engine hooks and a test engine loaded later, from
// its own module.
//
// IMGUI_ENABLE_TEST_ENGINE is defined by imconfig.h, so ImGui calls four hook functions.
// This library defines them all as forwards to a table of function pointers that is empty
// by default: with no engine attached every relay does nothing. The engine fills the table
// when its module loads and clears it when the module unloads.
//
// The table is process-wide and is read and written from the thread that drives ImGui.
#include "imgui.h"
#include "imgui_internal.h"

struct LmImGuiTestEngineHooks
{
    void (*ItemAdd)(ImGuiContext* ctx, ImGuiID id, const ImRect& bb, const ImGuiLastItemData* item_data);
    void (*ItemInfo)(ImGuiContext* ctx, ImGuiID id, const char* label, ImGuiItemStatusFlags flags);
    // Receives the message already formatted: the relay owns the varargs.
    void (*Log)(ImGuiContext* ctx, const char* message);
    const char* (*FindItemDebugLabel)(ImGuiContext* ctx, ImGuiID id);
};

/// Install the engine's hooks (the table is copied); nullptr detaches them.
IMGUI_API void LmImGui_SetTestEngineHooks(const LmImGuiTestEngineHooks* hooks);

/// Wrapper for the one ImGui internal the engine calls that ImGui does not export, so
/// the engine module forwards to ImGui's single implementation instead of copying it.
IMGUI_API ImGuiKeyChord LmImGui_FixupKeyChord(ImGuiKeyChord key_chord);

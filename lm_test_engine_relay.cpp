// Defines the four functions Dear ImGui calls when IMGUI_ENABLE_TEST_ENGINE is defined
// (see lm_test_engine_relay.h). No engine code lives here.
#include "lm_test_engine_relay.h"

#include <cstdarg>
#include <cstdio>

namespace
{
    LmImGuiTestEngineHooks g_hooks{};
}

void LmImGui_SetTestEngineHooks(const LmImGuiTestEngineHooks* hooks)
{
    g_hooks = hooks ? *hooks : LmImGuiTestEngineHooks{};
}

ImGuiKeyChord LmImGui_FixupKeyChord(ImGuiKeyChord key_chord)
{
    return ImGui::FixupKeyChord(key_chord);
}

// Exported, although imgui_internal.h declares them plain, so that a test can call each relay
// directly and check that it does nothing while no engine is attached.
IMGUI_API void ImGuiTestEngineHook_ItemAdd(ImGuiContext* ctx, ImGuiID id, const ImRect& bb, const ImGuiLastItemData* item_data)
{
    if (g_hooks.ItemAdd)
        g_hooks.ItemAdd(ctx, id, bb, item_data);
}

IMGUI_API void ImGuiTestEngineHook_ItemInfo(ImGuiContext* ctx, ImGuiID id, const char* label, ImGuiItemStatusFlags flags)
{
    if (g_hooks.ItemInfo)
        g_hooks.ItemInfo(ctx, id, label, flags);
}

IMGUI_API void ImGuiTestEngineHook_Log(ImGuiContext* ctx, const char* fmt, ...)
{
    if (!g_hooks.Log)
        return;
    char message[2048];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);
    g_hooks.Log(ctx, message);
}

IMGUI_API const char* ImGuiTestEngine_FindItemDebugLabel(ImGuiContext* ctx, ImGuiID id)
{
    return g_hooks.FindItemDebugLabel ? g_hooks.FindItemDebugLabel(ctx, id) : nullptr;
}

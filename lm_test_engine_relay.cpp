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
// directly and check that it does nothing while no engine is attached. MSVC refuses dllexport on
// a definition whose declaration lacks it, so there the linker exports the function by its
// decorated name, from inside its body.
#if defined(_MSC_VER)
#define LM_RELAY
#define LM_RELAY_EXPORT __pragma(comment(linker, "/EXPORT:" __FUNCDNAME__))
#else
#define LM_RELAY IMGUI_API
#define LM_RELAY_EXPORT
#endif

LM_RELAY void ImGuiTestEngineHook_ItemAdd(ImGuiContext* ctx, ImGuiID id, const ImRect& bb, const ImGuiLastItemData* item_data)
{
    LM_RELAY_EXPORT
    if (g_hooks.ItemAdd)
        g_hooks.ItemAdd(ctx, id, bb, item_data);
}

LM_RELAY void ImGuiTestEngineHook_ItemInfo(ImGuiContext* ctx, ImGuiID id, const char* label, ImGuiItemStatusFlags flags)
{
    LM_RELAY_EXPORT
    if (g_hooks.ItemInfo)
        g_hooks.ItemInfo(ctx, id, label, flags);
}

LM_RELAY void ImGuiTestEngineHook_Log(ImGuiContext* ctx, const char* fmt, ...)
{
    LM_RELAY_EXPORT
    if (!g_hooks.Log)
        return;
    char message[2048];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);
    g_hooks.Log(ctx, message);
}

LM_RELAY const char* ImGuiTestEngine_FindItemDebugLabel(ImGuiContext* ctx, ImGuiID id)
{
    LM_RELAY_EXPORT
    return g_hooks.FindItemDebugLabel ? g_hooks.FindItemDebugLabel(ctx, id) : nullptr;
}

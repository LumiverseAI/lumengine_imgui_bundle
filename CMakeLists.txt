cmake_minimum_required(VERSION 3.20)
project(lumengine_imgui_bundle VERSION 1.0.0 LANGUAGES C CXX)

# Set policies
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)  # Use BoostConfig.cmake
endif()
if(POLICY CMP0169)
    cmake_policy(SET CMP0169 OLD)  # Allow FetchContent_Populate for now
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# Library Git Tags/Commits
# ============================================================================
# CMake keeps cache entries across incremental configurations, so a build tree
# configured before a bump silently keeps fetching the previous commits. Stamp
# the pin set with a generation id: when it changes, every pin below is forced
# back to its new default exactly once. Bump the id in the same commit as the
# pins. An explicit -DLM_*_GIT_TAG=... on the command line still wins, it is
# simply re-applied on top of a refreshed generation.
set(_LM_PIN_GENERATION "2026.08-imgui-1.92.8")
set(_lm_pin_force "")
if(NOT DEFINED LM_IMGUI_BUNDLE_PIN_GENERATION OR
   NOT LM_IMGUI_BUNDLE_PIN_GENERATION STREQUAL _LM_PIN_GENERATION)
    set(_lm_pin_force FORCE)
    message(STATUS "ImGui bundle: applying pin generation ${_LM_PIN_GENERATION}")
endif()
set(LM_IMGUI_BUNDLE_PIN_GENERATION "${_LM_PIN_GENERATION}"
    CACHE INTERNAL "Generation id of the dependency pin set")

set(LM_IMGUI_GIT_TAG "de2079f09" CACHE STRING "ImGui git commit/tag" ${_lm_pin_force})
set(LM_IMPLOT_GIT_TAG "a3226777f" CACHE STRING "ImPlot git commit/tag" ${_lm_pin_force})
set(LM_IMPLOT3D_GIT_TAG "be541bc3f" CACHE STRING "ImPlot3D git commit/tag" ${_lm_pin_force})
set(LM_IMGUI_NODE_EDITOR_GIT_TAG "afa40649f" CACHE STRING "ImGui Node Editor git commit/tag" ${_lm_pin_force})
set(LM_IMGUIZMO_GIT_TAG "a25157a6e" CACHE STRING "ImGuizmo git commit/tag" ${_lm_pin_force})
set(LM_IMGUI_COLOR_TEXT_EDIT_GIT_TAG "20bc1ec8f" CACHE STRING "ImGui Color Text Edit git commit/tag" ${_lm_pin_force})
set(LM_IMGUI_MD_GIT_TAG "1f131466a" CACHE STRING "ImGui Markdown git commit/tag" ${_lm_pin_force})
set(LM_MD4C_GIT_TAG "481fbfbdf" CACHE STRING "md4c git commit/tag" ${_lm_pin_force})
set(LM_IMGUI_TOGGLE_GIT_TAG "a9d363b57" CACHE STRING "ImGui Toggle git commit/tag" ${_lm_pin_force})
set(LM_IMGUI_KNOBS_GIT_TAG "68af3d5c9" CACHE STRING "ImGui Knobs git commit/tag" ${_lm_pin_force})
set(LM_IMGUI_COMMAND_PALETTE_GIT_TAG "6f09a39" CACHE STRING "ImGui Command Palette git commit/tag" ${_lm_pin_force})
set(LM_IMGUI_COOL_BAR_GIT_TAG "ee3b07d0f" CACHE STRING "ImGui Cool Bar git commit/tag" ${_lm_pin_force})
unset(_lm_pin_force)

# Options to enable/disable each library component
option(LM_IMGUI_BUILD_IMPLOT "Include ImPlot" ON)
option(LM_IMGUI_BUILD_IMPLOT3D "Include ImPlot3D" ON)
option(LM_IMGUI_BUILD_IMGUI_NODE_EDITOR "Include ImGui Node Editor" ON)
option(LM_IMGUI_BUILD_IMGUIZMO "Include ImGuizmo" ON)
option(LM_IMGUI_BUILD_IMGUI_COLOR_TEXT_EDIT "Include ImGui Color Text Edit" ON)
option(LM_IMGUI_BUILD_IMGUI_MD "Include ImGui Markdown" ON)
option(LM_IMGUI_BUILD_IMGUI_TOGGLE "Include ImGui Toggle" ON)
option(LM_IMGUI_BUILD_IMGUI_KNOBS "Include ImGui Knobs" ON)
option(LM_IMGUI_BUILD_IMGUI_COMMAND_PALETTE "Include ImGui Command Palette" ON)
option(LM_IMGUI_BUILD_IMGUI_COOL_BAR "Include ImGui Cool Bar" ON)

# Options for ImGui backends
option(LM_IMGUI_BACKEND_WIN32 "Include Win32 backend" ON)
option(LM_IMGUI_BACKEND_DX11 "Include DirectX 11 backend" OFF)
option(LM_IMGUI_BACKEND_DX12 "Include DirectX 12 backend" OFF)
option(LM_IMGUI_BACKEND_OPENGL3 "Include OpenGL3 backend" OFF)

# FetchContent setup
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
# Honor the caller's FetchContent connectivity setting. Existing build trees
# must be able to fetch a newly pinned dependency revision.

# ============================================================================
# Fetch all libraries
# ============================================================================

message(STATUS "Fetching ImGui libraries...")

# ImGui core (always required)
FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/pthom/imgui.git
    GIT_TAG ${LM_IMGUI_GIT_TAG}
    GIT_SHALLOW FALSE
)

# ImPlot
if(LM_IMGUI_BUILD_IMPLOT)
    FetchContent_Declare(implot
        GIT_REPOSITORY https://github.com/pthom/implot.git
        GIT_TAG ${LM_IMPLOT_GIT_TAG}
        GIT_SHALLOW FALSE
    )
endif()

# ImPlot3D
if(LM_IMGUI_BUILD_IMPLOT3D)
    FetchContent_Declare(implot3d
        GIT_REPOSITORY https://github.com/pthom/implot3d.git
        GIT_TAG ${LM_IMPLOT3D_GIT_TAG}
        GIT_SHALLOW FALSE
    )
endif()

# ImGui Node Editor
if(LM_IMGUI_BUILD_IMGUI_NODE_EDITOR)
    FetchContent_Declare(imgui_node_editor
        GIT_REPOSITORY https://github.com/Lumengine/imgui-node-editor.git
        GIT_TAG ${LM_IMGUI_NODE_EDITOR_GIT_TAG}
        GIT_SHALLOW FALSE
    )
endif()

# ImGuizmo
if(LM_IMGUI_BUILD_IMGUIZMO)
    # Upstream now ships its own CMakeLists that builds a separate target with
    # no knowledge of our ImGui; point SOURCE_SUBDIR at a directory that has
    # none so MakeAvailable only populates the sources, which we compile into
    # imgui_bundle ourselves.
    FetchContent_Declare(imguizmo
        GIT_REPOSITORY https://github.com/pthom/ImGuizmo.git
        GIT_TAG ${LM_IMGUIZMO_GIT_TAG}
        GIT_SHALLOW FALSE
        SOURCE_SUBDIR lm-populate-only
    )
endif()

# ImGui Color Text Edit
if(LM_IMGUI_BUILD_IMGUI_COLOR_TEXT_EDIT)
    FetchContent_Declare(imgui_color_text_edit
        GIT_REPOSITORY https://github.com/Lumengine/ImGuiColorTextEdit.git
        GIT_TAG ${LM_IMGUI_COLOR_TEXT_EDIT_GIT_TAG}
        GIT_SHALLOW FALSE
    )
endif()

# md4c (for ImGui Markdown)
if(LM_IMGUI_BUILD_IMGUI_MD)
    FetchContent_Declare(md4c
        GIT_REPOSITORY https://github.com/mity/md4c.git
        GIT_TAG ${LM_MD4C_GIT_TAG}
        GIT_SHALLOW FALSE
    )

    FetchContent_Declare(imgui_md
        GIT_REPOSITORY https://github.com/Lumengine/imgui_md.git
        GIT_TAG ${LM_IMGUI_MD_GIT_TAG}
        GIT_SHALLOW FALSE
    )
endif()

# ImGui Toggle
if(LM_IMGUI_BUILD_IMGUI_TOGGLE)
    FetchContent_Declare(imgui_toggle
        GIT_REPOSITORY https://github.com/Lumengine/imgui_toggle.git
        GIT_TAG ${LM_IMGUI_TOGGLE_GIT_TAG}
        GIT_SHALLOW FALSE
    )
endif()

# ImGui Knobs
if(LM_IMGUI_BUILD_IMGUI_KNOBS)
    FetchContent_Declare(imgui_knobs
        GIT_REPOSITORY https://github.com/Lumengine/imgui-knobs.git
        GIT_TAG ${LM_IMGUI_KNOBS_GIT_TAG}
        GIT_SHALLOW FALSE
    )
endif()

# ImGui Command Palette
if(LM_IMGUI_BUILD_IMGUI_COMMAND_PALETTE)
    FetchContent_Declare(imgui_command_palette
        GIT_REPOSITORY https://github.com/Lumengine/imgui-command-palette.git
        GIT_TAG ${LM_IMGUI_COMMAND_PALETTE_GIT_TAG}
        GIT_SHALLOW FALSE
    )
endif()

# ImGui Cool Bar
if(LM_IMGUI_BUILD_IMGUI_COOL_BAR)
    FetchContent_Declare(imgui_cool_bar
        GIT_REPOSITORY https://github.com/pthom/ImCoolBar.git
        GIT_TAG ${LM_IMGUI_COOL_BAR_GIT_TAG}
        GIT_SHALLOW FALSE
    )
endif()

# Make all content available
FetchContent_MakeAvailable(imgui)
if(LM_IMGUI_BUILD_IMPLOT)
    FetchContent_MakeAvailable(implot)
endif()
if(LM_IMGUI_BUILD_IMPLOT3D)
    FetchContent_MakeAvailable(implot3d)
endif()
if(LM_IMGUI_BUILD_IMGUI_NODE_EDITOR)
    FetchContent_MakeAvailable(imgui_node_editor)
endif()
if(LM_IMGUI_BUILD_IMGUIZMO)
    FetchContent_MakeAvailable(imguizmo)
endif()
if(LM_IMGUI_BUILD_IMGUI_COLOR_TEXT_EDIT)
    FetchContent_MakeAvailable(imgui_color_text_edit)
endif()
if(LM_IMGUI_BUILD_IMGUI_MD)
    FetchContent_GetProperties(md4c)
    if(NOT md4c_POPULATED)
        FetchContent_Populate(md4c)
    endif()
    FetchContent_MakeAvailable(imgui_md)
endif()
if(LM_IMGUI_BUILD_IMGUI_TOGGLE)
    FetchContent_MakeAvailable(imgui_toggle)
endif()
if(LM_IMGUI_BUILD_IMGUI_KNOBS)
    FetchContent_MakeAvailable(imgui_knobs)
endif()
if(LM_IMGUI_BUILD_IMGUI_COMMAND_PALETTE)
    FetchContent_MakeAvailable(imgui_command_palette)
endif()
if(LM_IMGUI_BUILD_IMGUI_COOL_BAR)
    FetchContent_GetProperties(imgui_cool_bar)
    if(NOT imgui_cool_bar_POPULATED)
        FetchContent_Populate(imgui_cool_bar)
    endif()
endif()

# ============================================================================
# Collect all source files into a single library
# ============================================================================

set(ALL_SOURCES)
set(ALL_HEADERS)
set(ALL_INCLUDE_DIRS)

# ImGui core
file(GLOB IMGUI_SOURCES ${imgui_SOURCE_DIR}/*.cpp)
file(GLOB IMGUI_HEADERS ${imgui_SOURCE_DIR}/*.h)
list(APPEND ALL_SOURCES ${IMGUI_SOURCES})
list(APPEND ALL_HEADERS ${IMGUI_HEADERS})
list(APPEND ALL_INCLUDE_DIRS ${imgui_SOURCE_DIR})

# ImGui misc/cpp (optional C++ helpers)
file(GLOB IMGUI_MISC_CPP_SOURCES ${imgui_SOURCE_DIR}/misc/cpp/*.cpp)
file(GLOB IMGUI_MISC_CPP_HEADERS ${imgui_SOURCE_DIR}/misc/cpp/*.h)
if(IMGUI_MISC_CPP_SOURCES)
    list(APPEND ALL_SOURCES ${IMGUI_MISC_CPP_SOURCES})
    list(APPEND ALL_HEADERS ${IMGUI_MISC_CPP_HEADERS})
    list(APPEND ALL_INCLUDE_DIRS ${imgui_SOURCE_DIR}/misc/cpp)
    message(STATUS "  Including misc/cpp helpers")
endif()

# ImGui backends (platform/renderer implementations)
list(APPEND ALL_INCLUDE_DIRS ${imgui_SOURCE_DIR}/backends)

# Win32 backend
if(WIN32 AND LM_IMGUI_BACKEND_WIN32)
    if(EXISTS ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp)
        list(APPEND ALL_SOURCES ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp)
        list(APPEND ALL_HEADERS ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.h)
        message(STATUS "  Including Win32 backend")
    endif()
endif()

# DirectX 11 backend
if(WIN32 AND LM_IMGUI_BACKEND_DX11)
    if(EXISTS ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp)
        list(APPEND ALL_SOURCES ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp)
        list(APPEND ALL_HEADERS ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.h)
        message(STATUS "  Including DirectX 11 backend")
    endif()
endif()

# DirectX 12 backend
if(WIN32 AND LM_IMGUI_BACKEND_DX12)
    if(EXISTS ${imgui_SOURCE_DIR}/backends/imgui_impl_dx12.cpp)
        list(APPEND ALL_SOURCES ${imgui_SOURCE_DIR}/backends/imgui_impl_dx12.cpp)
        list(APPEND ALL_HEADERS ${imgui_SOURCE_DIR}/backends/imgui_impl_dx12.h)
        message(STATUS "  Including DirectX 12 backend")
    endif()
endif()

# OpenGL3 backend (cross-platform)
if(LM_IMGUI_BACKEND_OPENGL3)
    if(EXISTS ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
        list(APPEND ALL_SOURCES ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
        list(APPEND ALL_HEADERS ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h)
        message(STATUS "  Including OpenGL3 backend")
    endif()
endif()

# Note: Vulkan, GLFW, SDL2, SDL3 backends are NOT included by default
# as they require external dependencies. Users can include them manually
# if they have these dependencies installed by copying the backend files.

# ImPlot
if(LM_IMGUI_BUILD_IMPLOT)
    file(GLOB IMPLOT_SOURCES ${implot_SOURCE_DIR}/*.cpp)
    file(GLOB IMPLOT_HEADERS ${implot_SOURCE_DIR}/*.h)
    list(APPEND ALL_SOURCES ${IMPLOT_SOURCES})
    list(APPEND ALL_HEADERS ${IMPLOT_HEADERS})
    list(APPEND ALL_INCLUDE_DIRS ${implot_SOURCE_DIR})
endif()

# ImPlot3D
if(LM_IMGUI_BUILD_IMPLOT3D)
    file(GLOB IMPLOT3D_SOURCES ${implot3d_SOURCE_DIR}/*.cpp)
    file(GLOB IMPLOT3D_HEADERS ${implot3d_SOURCE_DIR}/*.h)
    list(APPEND ALL_SOURCES ${IMPLOT3D_SOURCES})
    list(APPEND ALL_HEADERS ${IMPLOT3D_HEADERS})
    list(APPEND ALL_INCLUDE_DIRS ${implot3d_SOURCE_DIR})
endif()

# ImGui Node Editor
if(LM_IMGUI_BUILD_IMGUI_NODE_EDITOR)
    file(GLOB NODE_EDITOR_SOURCES ${imgui_node_editor_SOURCE_DIR}/*.cpp)
    file(GLOB NODE_EDITOR_HEADERS ${imgui_node_editor_SOURCE_DIR}/*.h ${imgui_node_editor_SOURCE_DIR}/*.inl)
    list(APPEND ALL_SOURCES ${NODE_EDITOR_SOURCES})
    list(APPEND ALL_HEADERS ${NODE_EDITOR_HEADERS})
    list(APPEND ALL_INCLUDE_DIRS ${imgui_node_editor_SOURCE_DIR})
endif()

# ImGuizmo
# Upstream moved the sources under src/ in 2026; keep working with either
# layout so the pin can move back and forth without editing this block.
if(LM_IMGUI_BUILD_IMGUIZMO)
    if(EXISTS ${imguizmo_SOURCE_DIR}/src/ImGuizmo.cpp)
        set(IMGUIZMO_SRC_DIR ${imguizmo_SOURCE_DIR}/src)
    else()
        set(IMGUIZMO_SRC_DIR ${imguizmo_SOURCE_DIR})
    endif()
    file(GLOB IMGUIZMO_SOURCES ${IMGUIZMO_SRC_DIR}/*.cpp)
    file(GLOB IMGUIZMO_HEADERS ${IMGUIZMO_SRC_DIR}/*.h)
    list(APPEND ALL_SOURCES ${IMGUIZMO_SOURCES})
    list(APPEND ALL_HEADERS ${IMGUIZMO_HEADERS})
    list(APPEND ALL_INCLUDE_DIRS ${IMGUIZMO_SRC_DIR})
endif()

# ImGui Color Text Edit
if(LM_IMGUI_BUILD_IMGUI_COLOR_TEXT_EDIT)
    list(APPEND ALL_SOURCES
        ${imgui_color_text_edit_SOURCE_DIR}/TextEditor.cpp
        ${imgui_color_text_edit_SOURCE_DIR}/TextDiff.cpp
    )
    file(GLOB TEXT_EDIT_HEADERS ${imgui_color_text_edit_SOURCE_DIR}/*.h)
    list(APPEND ALL_HEADERS ${TEXT_EDIT_HEADERS})
    list(APPEND ALL_INCLUDE_DIRS
        ${imgui_color_text_edit_SOURCE_DIR}
    )
endif()

# md4c + ImGui Markdown
if(LM_IMGUI_BUILD_IMGUI_MD)
    list(APPEND ALL_SOURCES
        ${md4c_SOURCE_DIR}/src/md4c.c
        ${md4c_SOURCE_DIR}/src/md4c-html.c
        ${md4c_SOURCE_DIR}/src/entity.c
    )
    file(GLOB MD4C_HEADERS ${md4c_SOURCE_DIR}/src/*.h)
    list(APPEND ALL_HEADERS ${MD4C_HEADERS})

    file(GLOB IMGUI_MD_SOURCES ${imgui_md_SOURCE_DIR}/*.cpp)
    file(GLOB IMGUI_MD_HEADERS ${imgui_md_SOURCE_DIR}/*.h)
    list(APPEND ALL_SOURCES ${IMGUI_MD_SOURCES})
    list(APPEND ALL_HEADERS ${IMGUI_MD_HEADERS})
    list(APPEND ALL_INCLUDE_DIRS ${md4c_SOURCE_DIR}/src ${imgui_md_SOURCE_DIR})
endif()

# ImGui Toggle
if(LM_IMGUI_BUILD_IMGUI_TOGGLE)
    file(GLOB TOGGLE_SOURCES ${imgui_toggle_SOURCE_DIR}/*.cpp)
    file(GLOB TOGGLE_HEADERS ${imgui_toggle_SOURCE_DIR}/*.h)
    list(APPEND ALL_SOURCES ${TOGGLE_SOURCES})
    list(APPEND ALL_HEADERS ${TOGGLE_HEADERS})
    list(APPEND ALL_INCLUDE_DIRS ${imgui_toggle_SOURCE_DIR})
endif()

# ImGui Knobs
if(LM_IMGUI_BUILD_IMGUI_KNOBS)
    file(GLOB KNOBS_SOURCES ${imgui_knobs_SOURCE_DIR}/*.cpp)
    file(GLOB KNOBS_HEADERS ${imgui_knobs_SOURCE_DIR}/*.h)
    list(APPEND ALL_SOURCES ${KNOBS_SOURCES})
    list(APPEND ALL_HEADERS ${KNOBS_HEADERS})
    list(APPEND ALL_INCLUDE_DIRS ${imgui_knobs_SOURCE_DIR})
endif()

# ImGui Command Palette
if(LM_IMGUI_BUILD_IMGUI_COMMAND_PALETTE)
    file(GLOB CMD_PALETTE_SOURCES ${imgui_command_palette_SOURCE_DIR}/*.cpp)
    file(GLOB CMD_PALETTE_HEADERS ${imgui_command_palette_SOURCE_DIR}/*.h)
    list(APPEND ALL_SOURCES ${CMD_PALETTE_SOURCES})
    list(APPEND ALL_HEADERS ${CMD_PALETTE_HEADERS})
    list(APPEND ALL_INCLUDE_DIRS ${imgui_command_palette_SOURCE_DIR})
endif()

# ImGui Cool Bar
if(LM_IMGUI_BUILD_IMGUI_COOL_BAR)
    file(GLOB COOL_BAR_SOURCES ${imgui_cool_bar_SOURCE_DIR}/*.cpp)
    file(GLOB COOL_BAR_HEADERS ${imgui_cool_bar_SOURCE_DIR}/*.h)
    list(APPEND ALL_SOURCES ${COOL_BAR_SOURCES})
    list(APPEND ALL_HEADERS ${COOL_BAR_HEADERS})
    list(APPEND ALL_INCLUDE_DIRS ${imgui_cool_bar_SOURCE_DIR})
endif()

# ============================================================================
# Create single shared library: imgui_bundle
# ============================================================================

add_library(imgui_bundle SHARED ${ALL_SOURCES} ${ALL_HEADERS})

# Remove duplicates from include dirs and headers
list(REMOVE_DUPLICATES ALL_INCLUDE_DIRS)
list(REMOVE_DUPLICATES ALL_HEADERS)

# Set PUBLIC_HEADER property so parent projects can install headers via the target
set_target_properties(imgui_bundle PROPERTIES
    PUBLIC_HEADER "${ALL_HEADERS}"
)

# Set include directories
target_include_directories(imgui_bundle PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

foreach(inc_dir ${ALL_INCLUDE_DIRS})
    target_include_directories(imgui_bundle PUBLIC
        $<BUILD_INTERFACE:${inc_dir}>
    )
endforeach()

# Set compile definitions
# Allow parent projects to override the imconfig.h location
if(NOT DEFINED IMGUI_USER_CONFIG_PATH)
    set(IMGUI_USER_CONFIG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/imconfig.h")
endif()

target_compile_definitions(imgui_bundle
    PUBLIC
        IMGUI_USER_CONFIG="${IMGUI_USER_CONFIG_PATH}"
        IMGUI_BUNDLE_PYTHON_API
        # ImCoolBar decorates its API with IMCOOLBAR_API, which defaults to
        # nothing, so its symbols never left the shared library. Route it
        # through IMGUI_API so it exports and imports like everything else.
        IMCOOLBAR_API=IMGUI_API
    PRIVATE
        imgui_bundle_EXPORTS  # Define export macro for DLL building
)

if(LM_IMGUI_BUILD_IMGUI_MD)
    target_compile_definitions(imgui_bundle PRIVATE MD4C_USE_UTF8)
endif()

# On Linux/macOS, set visibility to hidden by default, then export only marked symbols
if(NOT WIN32)
    target_compile_options(imgui_bundle PRIVATE -fvisibility=hidden)
endif()

# Link required system libraries for backends
if(WIN32)
    set(BACKEND_LIBS "")

    if(LM_IMGUI_BACKEND_DX11)
        list(APPEND BACKEND_LIBS d3d11.lib)
    endif()

    if(LM_IMGUI_BACKEND_DX12)
        list(APPEND BACKEND_LIBS d3d12.lib dxgi.lib d3dcompiler.lib)
    endif()

    if(BACKEND_LIBS)
        target_link_libraries(imgui_bundle PRIVATE ${BACKEND_LIBS})
    endif()
endif()

# Print configuration summary
message(STATUS "=== Lumengine ImGui Bundle Configuration ===")
message(STATUS "Single shared library: imgui_bundle.dll")
message(STATUS "")
message(STATUS "Libraries:")
message(STATUS "  ImPlot: ${LM_IMGUI_BUILD_IMPLOT}")
message(STATUS "  ImPlot3D: ${LM_IMGUI_BUILD_IMPLOT3D}")
message(STATUS "  ImGui Node Editor: ${LM_IMGUI_BUILD_IMGUI_NODE_EDITOR}")
message(STATUS "  ImGuizmo: ${LM_IMGUI_BUILD_IMGUIZMO}")
message(STATUS "  ImGui Color Text Edit: ${LM_IMGUI_BUILD_IMGUI_COLOR_TEXT_EDIT}")
message(STATUS "  ImGui Markdown: ${LM_IMGUI_BUILD_IMGUI_MD}")
message(STATUS "  ImGui Toggle: ${LM_IMGUI_BUILD_IMGUI_TOGGLE}")
message(STATUS "  ImGui Knobs: ${LM_IMGUI_BUILD_IMGUI_KNOBS}")
message(STATUS "  ImGui Command Palette: ${LM_IMGUI_BUILD_IMGUI_COMMAND_PALETTE}")
message(STATUS "  ImGui Cool Bar: ${LM_IMGUI_BUILD_IMGUI_COOL_BAR}")
message(STATUS "")
message(STATUS "Backends:")
message(STATUS "  Win32: ${LM_IMGUI_BACKEND_WIN32}")
message(STATUS "  DirectX 11: ${LM_IMGUI_BACKEND_DX11}")
message(STATUS "  DirectX 12: ${LM_IMGUI_BACKEND_DX12}")
message(STATUS "  OpenGL3: ${LM_IMGUI_BACKEND_OPENGL3}")
message(STATUS "==========================================")

# ============================================================================
# Installation
# ============================================================================

# Install the shared library
install(TARGETS imgui_bundle
    EXPORT imgui_bundle_targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install headers to include/imgui_bundle (flat structure for easy includes)
# Users can then do: #include <imgui.h> or #include <implot.h>

# Core ImGui headers
install(FILES
    ${imgui_SOURCE_DIR}/imgui.h
    ${imgui_SOURCE_DIR}/imgui_internal.h
    ${imgui_SOURCE_DIR}/imconfig.h
    ${imgui_SOURCE_DIR}/imstb_rectpack.h
    ${imgui_SOURCE_DIR}/imstb_textedit.h
    ${imgui_SOURCE_DIR}/imstb_truetype.h
    DESTINATION include/imgui_bundle
)

# ImGui misc/cpp headers
if(EXISTS ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h)
    install(FILES ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h
        DESTINATION include/imgui_bundle
    )
endif()

# Backend headers
if(WIN32 AND LM_IMGUI_BACKEND_WIN32 AND EXISTS ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.h)
    install(FILES ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.h
        DESTINATION include/imgui_bundle
    )
endif()

# ImPlot headers
if(LM_IMGUI_BUILD_IMPLOT)
    install(FILES
        ${implot_SOURCE_DIR}/implot.h
        ${implot_SOURCE_DIR}/implot_internal.h
        DESTINATION include/imgui_bundle
    )
endif()

# ImPlot3D headers
if(LM_IMGUI_BUILD_IMPLOT3D)
    install(FILES
        ${implot3d_SOURCE_DIR}/implot3d.h
        ${implot3d_SOURCE_DIR}/implot3d_internal.h
        DESTINATION include/imgui_bundle
    )
endif()

# ImGui Node Editor headers
if(LM_IMGUI_BUILD_IMGUI_NODE_EDITOR)
    install(FILES
        ${imgui_node_editor_SOURCE_DIR}/imgui_node_editor.h
        DESTINATION include/imgui_bundle
    )
endif()

# ImGuizmo headers
if(LM_IMGUI_BUILD_IMGUIZMO)
    install(FILES
        ${IMGUIZMO_SRC_DIR}/ImGuizmo.h
        ${IMGUIZMO_SRC_DIR}/ImCurveEdit.h
        ${IMGUIZMO_SRC_DIR}/ImGradient.h
        ${IMGUIZMO_SRC_DIR}/ImSequencer.h
        ${IMGUIZMO_SRC_DIR}/ImZoomSlider.h
        ${IMGUIZMO_SRC_DIR}/GraphEditor.h
        DESTINATION include/imgui_bundle
    )
endif()

# ImGui Color Text Edit headers
if(LM_IMGUI_BUILD_IMGUI_COLOR_TEXT_EDIT)
    install(FILES
        ${imgui_color_text_edit_SOURCE_DIR}/TextEditor.h
        ${imgui_color_text_edit_SOURCE_DIR}/TextDiff.h
        DESTINATION include/imgui_bundle
    )
endif()

# ImGui Markdown headers
if(LM_IMGUI_BUILD_IMGUI_MD)
    install(FILES
        ${imgui_md_SOURCE_DIR}/imgui_md.h
        DESTINATION include/imgui_bundle
    )
endif()

# ImGui Toggle headers
if(LM_IMGUI_BUILD_IMGUI_TOGGLE)
    install(FILES
        ${imgui_toggle_SOURCE_DIR}/imgui_toggle.h
        ${imgui_toggle_SOURCE_DIR}/imgui_toggle_palette.h
        ${imgui_toggle_SOURCE_DIR}/imgui_toggle_presets.h
        ${imgui_toggle_SOURCE_DIR}/imgui_toggle_renderer.h
        DESTINATION include/imgui_bundle
    )
endif()

# ImGui Knobs headers
if(LM_IMGUI_BUILD_IMGUI_KNOBS)
    install(FILES
        ${imgui_knobs_SOURCE_DIR}/imgui-knobs.h
        DESTINATION include/imgui_bundle
    )
endif()

# ImGui Command Palette headers
if(LM_IMGUI_BUILD_IMGUI_COMMAND_PALETTE)
    install(FILES
        ${imgui_command_palette_SOURCE_DIR}/imcmd_command_palette.h
        DESTINATION include/imgui_bundle
    )
endif()

# ImGui Cool Bar headers
if(LM_IMGUI_BUILD_IMGUI_COOL_BAR)
    install(FILES
        ${imgui_cool_bar_SOURCE_DIR}/ImCoolbar.h
        DESTINATION include/imgui_bundle
    )
endif()

# Install custom imconfig.h if it exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/imconfig.h)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/imconfig.h
        DESTINATION include/imgui_bundle
        RENAME imconfig_bundle.h
    )
endif()

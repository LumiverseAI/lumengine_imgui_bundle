# Lumengine ImGui Bundle

This CMake bundle builds Dear ImGui and the extensions used by Lumengine into
one shared library, `imgui_bundle`. It requires CMake 3.20 and C++17.

## Integration

```cmake
include(FetchContent)
FetchContent_Declare(lumengine_imgui_bundle
    GIT_REPOSITORY https://github.com/LumiverseAI/lumengine_imgui_bundle.git
    GIT_TAG <exact-reviewed-commit>
)
FetchContent_MakeAvailable(lumengine_imgui_bundle)
target_link_libraries(your_target PRIVATE imgui_bundle)
```

Use an exact commit. Each dependency is also pinned to a full commit SHA in
`CMakeLists.txt`. The pin generation refreshes older CMake cache values when
this bundle is updated. Lumengine creates its own `Lumengine::ImGui_Bundle` alias.

## Components

Dear ImGui is always included. Each option below defaults to ON:

| Option | Library |
| --- | --- |
| LM_IMGUI_BUILD_IMPLOT | ImPlot |
| LM_IMGUI_BUILD_IMPLOT3D | ImPlot3D |
| LM_IMGUI_BUILD_IMGUI_NODE_EDITOR | ImGui Node Editor |
| LM_IMGUI_BUILD_IMGUIZMO | ImGuizmo |
| LM_IMGUI_BUILD_IMGUI_COLOR_TEXT_EDIT | ImGuiColorTextEdit |
| LM_IMGUI_BUILD_IMGUI_MD | imgui_md and md4c |
| LM_IMGUI_BUILD_IMGUI_TOGGLE | imgui_toggle |
| LM_IMGUI_BUILD_IMGUI_KNOBS | imgui-knobs |
| LM_IMGUI_BUILD_IMGUI_COMMAND_PALETTE | imgui-command-palette |
| LM_IMGUI_BUILD_IMGUI_COOL_BAR | ImCoolBar |

Win32 is enabled on Windows. DX11, DX12, and OpenGL3 backends are opt-in via
`LM_IMGUI_BACKEND_DX11`, `LM_IMGUI_BACKEND_DX12`, and `LM_IMGUI_BACKEND_OPENGL3`.
ImSpinner, ImFileDialog, and texture inspection are not supplied by this bundle.

## October 2026 dependency update

The compatibility reference is pthom/imgui_bundle v1.92.900. Dear ImGui remains
at 1.92.9b (`de2079f098dad2c461f8d0f40f3284291595288b`), which already contains
fixes beyond that release's submodule pin. ImPlot moves to
`fa5b4e2f843db64a9a9ccc4357d3a74e4ca0b389`.

ImGuiColorTextEdit merges upstream `bc5bb95db077aea273b365a5b41a97a4c83e2f0f`
while retaining Lumengine's DLL exports, hover accessors, autocomplete accessors
and popup behavior. `Render` now returns whether the document changed. The
other extension pins already contain the stable changes plus necessary DLL
export patches. Their `lumengine` branches may be newer than their default
branches; never update those forks from their default branch blindly.

The bundle defines `IMGUI_BUNDLE_PYTHON_API`. Python bindings belong to the
Lumengine consumer and must be regenerated from these exact fetched headers.
The 1.93 WIP branch and the replacement Markdown library are not part of this
stable update.

## Standalone build

```sh
cmake -S . -B build
cmake --build build --config Release
```

The consumer chooses runtime and SDK install components. Headers are available
through the target's `PUBLIC_HEADER` property; the shared library must be
shipped alongside applications that use it.

## Licenses

Every dependency retains its upstream license. Consult the source repository
and its license files for each component.

## Installed consumer check

After building and installing the bundle, configure this independent consumer
against the installation prefix. It checks exported Node Editor state APIs and
TextEditor symbols without access to dependency source directories.

```sh
cmake -S tests/consumer -B build-consumer -DCMAKE_PREFIX_PATH=/path/to/install
cmake --build build-consumer
ctest --test-dir build-consumer --output-on-failure
```

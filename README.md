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

The bundle pins Dear ImGui 1.93.0 with docking and stack layout, ImPlot 1.1,
ImPlot3D 0.5, and Node Editor 0.10 WIP at upstream revision
`27bcacec350a37db43e3afb8d37223237912577c`. Every dependency is pinned to an
exact commit in CMake. Node Editor's blueprints runtime is not included.

The Lumengine forks preserve DLL exports and Python convenience APIs while
merging their upstream changes. TextEditor also retains its hover accessors.
Some forks use a `lumengine` integration branch rather than their default branch.
ImGui Knobs was already current and keeps its existing revision.

The bundle defines `IMGUI_BUNDLE_PYTHON_API`. Python bindings belong to the
Lumengine consumer and must be regenerated from these exact fetched headers.
Node Editor 0.10 replaces save callbacks with explicit state and persistence
APIs; consumers must adapt rather than patch downloaded sources during configure.

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

For an installation built by Lumengine, also pass `-DBUNDLE_USE_WCHAR32=ON`,
matching its public `IMGUI_USE_WCHAR32` build setting.

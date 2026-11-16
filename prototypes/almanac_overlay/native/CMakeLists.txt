cmake_minimum_required(VERSION 3.24)
project(AlmanacNativeCheck LANGUAGES CXX)

# Deliberately separate from build.bat and normal plugins. No fetching/deployment.
if(NOT WIN32 OR NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Use an x64 MSVC Windows toolchain; this is not a host/GNU build")
endif()
include("${CMAKE_CURRENT_LIST_DIR}/verify_imgui.cmake")
# Match loader/plugin CRT isolation: no CRT ownership crosses the C API.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Configure-time checks alone miss dependency changes before an incremental build.
add_custom_target(almanac_verify_imgui
    COMMAND "${CMAKE_COMMAND}" "-DALMANAC_IMGUI_DIR=${ALMANAC_IMGUI_DIR}"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/verify_imgui.cmake"
    VERBATIM)

add_library(almanac_imgui STATIC
    "${ALMANAC_IMGUI_DIR}/imgui.cpp" "${ALMANAC_IMGUI_DIR}/imgui_draw.cpp"
    "${ALMANAC_IMGUI_DIR}/imgui_tables.cpp" "${ALMANAC_IMGUI_DIR}/imgui_widgets.cpp"
    "${ALMANAC_IMGUI_DIR}/backends/imgui_impl_dx11.cpp")
add_dependencies(almanac_imgui almanac_verify_imgui)
target_include_directories(almanac_imgui SYSTEM PUBLIC "${ALMANAC_IMGUI_DIR}")
target_compile_features(almanac_imgui PUBLIC cxx_std_17)
target_link_libraries(almanac_imgui PUBLIC d3d11 dxgi d3dcompiler)

add_library(almanac_native_candidate STATIC d3d11_panel.cpp graphics_adapter.cpp
    ../almanac_overlay_view.cpp ../earthworks/panel_style.cpp)
target_include_directories(almanac_native_candidate PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../../src")
target_compile_features(almanac_native_candidate PUBLIC cxx_std_17)
target_compile_options(almanac_native_candidate PRIVATE /W4 /WX /EHsc /utf-8)
target_compile_definitions(almanac_native_candidate PUBLIC NOMINMAX WIN32_LEAN_AND_MEAN)
target_link_libraries(almanac_native_candidate PUBLIC almanac_imgui user32)

add_executable(almanac_panel_window WIN32 panel_window.cpp)
target_compile_options(almanac_panel_window PRIVATE /W4 /WX /EHsc /utf-8)
target_link_libraries(almanac_panel_window PRIVATE almanac_native_candidate)
add_dependencies(almanac_native_candidate almanac_verify_imgui)
add_dependencies(almanac_panel_window almanac_verify_imgui)
option(ALMANAC_BUILD_DEV_PLUGIN "Compile experimental game DLL (no deployment)" ON)
if(ALMANAC_BUILD_DEV_PLUGIN)
    add_library(almanac_overlay_dev SHARED dev_plugin.cpp)
    target_compile_features(almanac_overlay_dev PRIVATE cxx_std_17)
    target_compile_options(almanac_overlay_dev PRIVATE /W4 /WX /EHsc /utf-8)
    target_link_libraries(almanac_overlay_dev PRIVATE almanac_native_candidate)
    add_dependencies(almanac_overlay_dev almanac_verify_imgui)
endif()
# No test auto-execution, install rules, game detection, or DLL copying.

cmake_minimum_required(VERSION 3.21)
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" EARTHWORKS_VERSION_STRING LIMIT_COUNT 1)
if(NOT EARTHWORKS_VERSION_STRING MATCHES "^([0-9]+\\.[0-9]+\\.[0-9]+)([-+][A-Za-z0-9.-]+)?$")
    message(FATAL_ERROR "VERSION must contain a semantic version and optional suffix.")
endif()
set(EARTHWORKS_CMAKE_VERSION "${CMAKE_MATCH_1}")
project(EarthworksPlanning VERSION "${EARTHWORKS_CMAKE_VERSION}" LANGUAGES CXX)
option(EARTHWORKS_TRACE_FAULT "Enable temporary native fault tracing during investigation" OFF)
option(EARTHWORKS_DEVELOPER_UI "Include the technical workforce inspector" OFF)

if(NOT WIN32 OR NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Use the Microsoft x64 C++ toolchain on Windows.")
endif()

set(TESMIO_SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/TesmioLoader" CACHE PATH "Pinned TesmioLoader checkout")
if(NOT EXISTS "${TESMIO_SDK_DIR}/src/tesmio_api.h")
    message(FATAL_ERROR "SDK missing. Run scripts/Fetch-Dependencies.ps1 first.")
endif()

file(SHA256 "${TESMIO_SDK_DIR}/src/tesmio_api.h" sdk_header_hash)
if(NOT sdk_header_hash STREQUAL "33c9fae4acb1041708c7b1b4675b0eb4740f0af737e7a1968c0acb0c325fff3c")
    message(FATAL_ERROR "SDK header differs from the pinned API 4 header. Review dependencies.lock.json.")
endif()

set(EARTHWORKS_IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/DearImGui" CACHE PATH "Pinned upstream Dear ImGui checkout")
if(NOT EXISTS "${EARTHWORKS_IMGUI_DIR}/imgui.cpp")
    message(FATAL_ERROR "Pinned Dear ImGui checkout missing. Run scripts/Fetch-Dependencies.ps1.")
endif()
set(EARTHWORKS_IMGUI_VERIFY "${CMAKE_CURRENT_SOURCE_DIR}/cmake/verify_imgui.cmake")
execute_process(COMMAND "${CMAKE_COMMAND}" "-DEARTHWORKS_IMGUI_DIR=${EARTHWORKS_IMGUI_DIR}"
    -P "${EARTHWORKS_IMGUI_VERIFY}" RESULT_VARIABLE imgui_verify_result)
if(NOT imgui_verify_result EQUAL 0)
    message(FATAL_ERROR "Pinned Dear ImGui verification failed.")
endif()
add_custom_target(earthworks_verify_imgui
    COMMAND "${CMAKE_COMMAND}" "-DEARTHWORKS_IMGUI_DIR=${EARTHWORKS_IMGUI_DIR}"
        -P "${EARTHWORKS_IMGUI_VERIFY}"
    VERBATIM)
add_library(earthworks_imgui STATIC
    "${EARTHWORKS_IMGUI_DIR}/imgui.cpp" "${EARTHWORKS_IMGUI_DIR}/imgui_draw.cpp"
    "${EARTHWORKS_IMGUI_DIR}/imgui_tables.cpp" "${EARTHWORKS_IMGUI_DIR}/imgui_widgets.cpp"
    "${EARTHWORKS_IMGUI_DIR}/backends/imgui_impl_win32.cpp"
    "${EARTHWORKS_IMGUI_DIR}/backends/imgui_impl_dx11.cpp")
add_dependencies(earthworks_imgui earthworks_verify_imgui)
target_include_directories(earthworks_imgui SYSTEM PUBLIC "${EARTHWORKS_IMGUI_DIR}" "${EARTHWORKS_IMGUI_DIR}/backends")
target_compile_definitions(earthworks_imgui PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
set_property(TARGET earthworks_imgui PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated")
configure_file(src/plugin/version.h.in "${CMAKE_CURRENT_BINARY_DIR}/generated/earthworks_version.h" @ONLY)
add_library(EarthworksPlanning SHARED src/plugin/plugin.cpp src/core/planning.cpp src/core/productivity.cpp src/core/state_store.cpp src/core/user_settings.cpp src/runtime/runtime.cpp src/runtime/save_store.cpp src/runtime/native_terrain_edit_hook.cpp src/runtime/workforce_probe.cpp src/runtime/vehicle_filter.cpp src/ui/overlay.cpp src/ui/game_underlay.cpp)
target_link_libraries(EarthworksPlanning PRIVATE earthworks_imgui d3d11 dxgi d3dcompiler dwmapi bcrypt windowscodecs ole32)
target_include_directories(EarthworksPlanning SYSTEM PRIVATE "${TESMIO_SDK_DIR}/src")
target_include_directories(EarthworksPlanning PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated")
target_compile_features(EarthworksPlanning PRIVATE cxx_std_17)
target_compile_definitions(EarthworksPlanning PRIVATE
    WIN32_LEAN_AND_MEAN NOMINMAX
)
if(EARTHWORKS_DEVELOPER_UI)
    target_compile_definitions(EarthworksPlanning PRIVATE EARTHWORKS_DEVELOPER_UI=1)
endif()
if(EARTHWORKS_TRACE_FAULT)
    target_compile_definitions(EarthworksPlanning PRIVATE EARTHWORKS_TRACE_FAULT)
endif()
target_compile_options(EarthworksPlanning PRIVATE /W4 /WX /permissive- /utf-8)
target_link_options(EarthworksPlanning PRIVATE "/MAP:${CMAKE_BINARY_DIR}/bin/EarthworksPlanning.map")
set_property(TARGET EarthworksPlanning PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_target_properties(EarthworksPlanning PROPERTIES
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

enable_testing()
add_executable(planning_checks tests/planning_checks.cpp src/core/planning.cpp)
target_compile_features(planning_checks PRIVATE cxx_std_17)
target_compile_options(planning_checks PRIVATE /W4 /WX /utf-8)
set_property(TARGET planning_checks PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
add_test(NAME geometry_and_draft_history COMMAND planning_checks)
add_executable(terrain_save_checks tests/terrain_save_checks.cpp)
target_compile_features(terrain_save_checks PRIVATE cxx_std_17)
target_compile_options(terrain_save_checks PRIVATE /W4 /WX /utf-8)
set_property(TARGET terrain_save_checks PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
add_executable(workforce_checks tests/workforce_checks.cpp src/runtime/workforce_probe.cpp src/runtime/vehicle_filter.cpp src/core/productivity.cpp)
target_include_directories(workforce_checks SYSTEM PRIVATE "${TESMIO_SDK_DIR}/src")
target_compile_features(workforce_checks PRIVATE cxx_std_17)
target_compile_options(workforce_checks PRIVATE /W4 /WX /utf-8)
target_compile_definitions(workforce_checks PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
set_property(TARGET workforce_checks PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
add_test(NAME synthetic_workforce_readings COMMAND workforce_checks)

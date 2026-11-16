# Offline dependency validation; also runnable with cmake -D... -P this-file.
set(ALMANAC_IMGUI_DIR "" CACHE PATH "Unmodified local Dear ImGui v1.91.9b checkout")
if(NOT IS_DIRECTORY "${ALMANAC_IMGUI_DIR}")
    message(FATAL_ERROR "Supply -DALMANAC_IMGUI_DIR=<local pinned ImGui checkout>")
endif()
find_package(Git REQUIRED)
set(IMGUI_PIN f5befd2d29e66809cd1110a152e375a7f1981f06)
execute_process(COMMAND "${GIT_EXECUTABLE}" -C "${ALMANAC_IMGUI_DIR}" rev-parse HEAD
    RESULT_VARIABLE git_result OUTPUT_VARIABLE actual OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT git_result EQUAL 0 OR NOT actual STREQUAL IMGUI_PIN)
    message(FATAL_ERROR "ImGui HEAD must be ${IMGUI_PIN}; got ${actual}")
endif()
set(IMGUI_INPUTS imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp
    imgui.h imconfig.h imgui_internal.h imstb_rectpack.h imstb_textedit.h
    imstb_truetype.h LICENSE.txt backends/imgui_impl_dx11.cpp backends/imgui_impl_dx11.h)
foreach(input IN LISTS IMGUI_INPUTS)
    if(NOT EXISTS "${ALMANAC_IMGUI_DIR}/${input}")
        message(FATAL_ERROR "Missing pinned ImGui input: ${input}")
    endif()
    # Compare raw worktree bytes, without trusting clean filters/autocrlf.
    execute_process(COMMAND "${GIT_EXECUTABLE}" -C "${ALMANAC_IMGUI_DIR}" rev-parse "${IMGUI_PIN}:${input}"
        RESULT_VARIABLE expected_result OUTPUT_VARIABLE expected OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND "${GIT_EXECUTABLE}" hash-object --no-filters "${ALMANAC_IMGUI_DIR}/${input}"
        RESULT_VARIABLE hash_result OUTPUT_VARIABLE actual_hash OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(NOT expected_result EQUAL 0 OR NOT hash_result EQUAL 0 OR NOT actual_hash STREQUAL expected)
        message(FATAL_ERROR "Modified ImGui input: ${input}")
    endif()
endforeach()

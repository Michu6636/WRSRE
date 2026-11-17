// Single source of the panel version. Bump it, then rebuild by name:
//   build.bat almanac_overlay_dev
// The value goes into TsmPluginInfo::version, the loader log line and the panel
// footer, so a screenshot identifies the build that produced it.
#pragma once

#define ALMANAC_PANEL_VERSION "1.0"

// Dear ImGui pin. The build fetches this tag into .cache unless
// ALMANAC_IMGUI_DIR names an existing checkout, and refuses to build if the
// checkout does not contain imgui.cpp. Keep this string and the tag in
// build.bat in step.
#define ALMANAC_IMGUI_TAG "v1.91.9b"

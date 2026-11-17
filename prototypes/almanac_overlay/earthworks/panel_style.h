// SPDX-License-Identifier: GPL-3.0-only
// Earthworks-derived style adapter; see NOTICE.md and GPL-3.0.txt.
#pragma once
namespace almanac {
// Call once on an owned current ImGui context, before frames and DPI scaling.
// Sets absolute style values (not cumulative scaling). No fonts/assets loaded.
// Never call on another overlay's context. Does not create a context or backend.
void ApplyEarthworksPanelStyle();
}

// Isolated drawing prototype, deliberately outside plugins/ and build.bat.
#pragma once
#include "almanac_finance_view.h"
namespace almanac {
struct OverlayPrototypeActions { bool close = false; };
// Caller owns context, NewFrame/Render, GPU resources, scaling and input routing.
// This draws only. It cannot open itself, capture game input or access providers.
// Interactive legacy prototype; NOT approved for native integration.
OverlayPrototypeActions DrawOverlayPrototype(const FinanceView& view);
// Draw-only candidate: no controls, navigation, focus or input handling.
// Visibility/readiness is entirely the caller's responsibility.
void DrawOverlayReadOnlyPrototype(const FinanceView& view);
FinanceView SyntheticOverlayView();
} // namespace almanac

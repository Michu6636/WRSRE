// Isolated drawing prototype, deliberately outside plugins/ and build.bat.
#pragma once
#include "almanac_finance_view.h"
namespace almanac {
struct OverlayPrototypeActions { bool close = false; };
// Caller owns context, NewFrame/Render, GPU resources, scaling and input routing.
// This draws only. It cannot open itself, capture game input or access providers.
OverlayPrototypeActions DrawOverlayPrototype(const FinanceView& view);
FinanceView SyntheticOverlayView();
} // namespace almanac

#include "almanac_pages.h"

#include <cstddef>

namespace {
const char* kTitles[ALMANAC_PAGE_COUNT] = {
    "Overview",
    "Fiscal Policy",
    "Money Flow",
    "Private Economy",
    "Prices & Citizens",
    "Logistics & Production",
    "Causation"
};
const char* kEmpty[ALMANAC_PAGE_COUNT] = {
    "No data source",
    "Fiscal controls unavailable",
    "No money-flow data",
    "No private-economy data",
    "No price or citizen data",
    "No logistics or production data",
    "No causation data"
};
}

extern "C" const char* almanac_page_title(AlmanacPageId page) {
    if (page < 0 || page >= ALMANAC_PAGE_COUNT) return "";
    return kTitles[page];
}

extern "C" const char* almanac_page_default_empty_state(AlmanacPageId page) {
    if (page < 0 || page >= ALMANAC_PAGE_COUNT) return "Unavailable";
    return kEmpty[page];
}

extern "C" const char* almanac_value_class_name(AlmanacValueClass valueClass) {
    switch (valueClass) {
        case ALMANAC_VALUE_ACTUAL: return "ACTUAL";
        case ALMANAC_VALUE_DRY_RUN: return "DRY_RUN";
        case ALMANAC_VALUE_ESTIMATED: return "ESTIMATED";
        default: return "UNKNOWN";
    }
}

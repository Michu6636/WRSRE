#pragma once

#include "almanac_ui_api.h"

#ifdef __cplusplus
extern "C" {
#endif

const char* almanac_page_title(AlmanacPageId page);
const char* almanac_page_default_empty_state(AlmanacPageId page);
const char* almanac_value_class_name(AlmanacValueClass valueClass);

#ifdef __cplusplus
}
#endif

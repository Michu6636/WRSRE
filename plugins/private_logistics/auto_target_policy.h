#pragma once

#include <cstddef>

// v0.18.19: road-only TOTAL target policy. This is not an engine maximum.
// The reviewed creator/receiver chain initializes 100 DWORD controls; the road
// renderer uses total-count indexing. Road has native /49 AND /20 display modes;
// stay within BOTH without reading an unverified live mode. Neither the 100-slot
// capacity nor a per-selection batch count is the policy. Rail remains unchanged.
enum {
    AUTO_DO_ROAD_TYPE = 43,
    AUTO_DO_RAIL_TYPE = 52,
    AUTO_DO_TARGET_STATE_CAPACITY = 100,
    AUTO_DO_ROAD_TARGET_LIMIT = 20,
    AUTO_DO_RAIL_TARGET_LIMIT = 5
};
static_assert(AUTO_DO_ROAD_TARGET_LIMIT <= AUTO_DO_TARGET_STATE_CAPACITY,
              "road policy exceeds reviewed per-target control storage");
static_assert(AUTO_DO_RAIL_TARGET_LIMIT <= AUTO_DO_TARGET_STATE_CAPACITY,
              "rail policy exceeds reviewed per-target control storage");

inline std::size_t AutoTargetLimitForType(int type)
{
    return type == AUTO_DO_ROAD_TYPE ? AUTO_DO_ROAD_TARGET_LIMIT :
           type == AUTO_DO_RAIL_TYPE ? AUTO_DO_RAIL_TARGET_LIMIT : 0;
}

// Count ALL native entries, including manual/customs/unreadable rows. Never
// delete existing entries to fit policy. Subtraction avoids count overflow;
// projected counts include successful appends (or dry-run plans), not failures.
inline bool AutoCanAppendTarget(int type, std::size_t existing,
                                std::size_t projected = 0)
{
    const std::size_t limit = AutoTargetLimitForType(type);
    return existing < limit && projected < limit - existing;
}

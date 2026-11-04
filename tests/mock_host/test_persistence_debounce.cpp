#include "save_path_match.h"
#include <cassert>
#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { std::printf("FAIL: %s (%s:%d)\n", #cond, __FILE__, __LINE__); ++failures; } } while(0)

int main() {
    // M3 positive
    {
        char out[512];
        CHECK(SaveDirOf("C:/x/save/slot0/stats.ini", "stats.ini", out, sizeof(out)));
        CHECK(std::strcmp(out, "C:/x/save/slot0") == 0);
    }
    {
        char out[512];
        CHECK(SaveDirOf("C:\\x\\save\\slot0\\stats.ini", "stats.ini", out, sizeof(out)));
    }
    {
        char out[512];
        CHECK(SaveDirOf("C:/x/SAVE/slot0/stats.ini", "stats.ini", out, sizeof(out)));
    }
    // M3 negative
    {
        char out[512];
        CHECK(!SaveDirOf("C:/x/autosave_backup/slot0/stats.ini", "stats.ini", out, sizeof(out)));
    }
    {
        char out[512];
        CHECK(!SaveDirOf("C:/x/my_save/slot0/stats.ini", "stats.ini", out, sizeof(out)));
    }
    {
        char out[512];
        CHECK(!SaveDirOf("C:/x/save_backup/slot0/stats.ini", "stats.ini", out, sizeof(out)));
    }
    {
        char out[512];
        CHECK(!SaveDirOf("C:/x/save", "stats.ini", out, sizeof(out)));
    }

    // M4 debounce
    {
        SaveDebounceState st;
        char out[512];
        CHECK(SaveDirOf("C:/x/save/slot0/header.bin", "header.bin", out, sizeof(out)));
        // first load
        CHECK(!DebounceLoad(st, out, 1000, 3000));
        // duplicate within 50ms -> skip
        CHECK(DebounceLoad(st, out, 1050, 3000));
        // invalidate on write
        InvalidateLoadDebounce(st);
        // load after write within <3s -> NOT skipped
        CHECK(!DebounceLoad(st, out, 1100, 3000));
    }

    // Load -> Save -> Load rapid
    {
        SaveDebounceState st;
        char out[512];
        CHECK(SaveDirOf("C:/x/save/slot0/header.bin", "header.bin", out, sizeof(out)));
        CHECK(!DebounceLoad(st, out, 1000, 3000)); // load
        InvalidateLoadDebounce(st); // save
        CHECK(!DebounceLoad(st, out, 1050, 3000)); // load again not suppressed
    }

    if (failures) {
        std::printf("test_persistence_debounce: %d failures\n", failures);
        return 1;
    }
    std::printf("test_persistence_debounce: OK\n");
    return 0;
}

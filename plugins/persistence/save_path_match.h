// save_path_match.h - pure helpers for persistence save-directory
// resolution and load-debounce state. Header-only so the unit tests
// exercise the exact production code.

#ifndef WRSRE_PERSISTENCE_SAVE_PATH_MATCH_H
#define WRSRE_PERSISTENCE_SAVE_PATH_MATCH_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cctype>

namespace wrsre {
namespace persistence {

inline int spm_tolower(int c) {
    return (c >= 'A' && c <= 'Z') ? (c + ('a' - 'A')) : c;
}

inline int spm_stricmp(const char* a, const char* b) {
    if (a == b) return 0;
    if (!a) return (b && *b) ? -1 : 0;
    if (!b) return (*a) ? 1 : 0;
    while (*a && *b) {
        int ca = spm_tolower((unsigned char)*a);
        int cb = spm_tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        ++a; ++b;
    }
    return spm_tolower((unsigned char)*a) - spm_tolower((unsigned char)*b);
}

inline int spm_strnicmp(const char* a, const char* b, std::size_t n) {
    if (n == 0) return 0;
    if (a == b) return 0;
    if (!a) a = "";
    if (!b) b = "";
    for (std::size_t i = 0; i < n; ++i) {
        int ca = spm_tolower((unsigned char)a[i]);
        int cb = spm_tolower((unsigned char)b[i]);
        if (ca != cb) return ca - cb;
        if (a[i] == '\0' || b[i] == '\0') {
            // both terminated; equality so far
            if (a[i] == b[i]) return 0;
        }
    }
    return 0;
}

// Resolve the save directory from a full file path.
//
// `path` must end with `filename` (case-insensitive) preceded by a
// separator. The directory containing that file is then checked to
// ensure its *parent* component is exactly "save" (case-insensitive),
// i.e. the layout ".../save/<slot>/<trigger>". Returns true and writes
// the slot directory (no trailing separator) to `out` on success.
inline bool SaveDirOf(const char* path, const char* filename,
                      char* out, std::size_t n) {
    if (!path || !filename || !out || n == 0) return false;

    std::size_t pathLen = std::strlen(path);
    std::size_t fileLen = std::strlen(filename);
    if (pathLen < fileLen + 2 || pathLen >= n) return false;

    if (spm_stricmp(path + pathLen - fileLen, filename) != 0) return false;
    char beforeSep = path[pathLen - fileLen - 1];
    if (beforeSep != '\\' && beforeSep != '/') return false;

    std::size_t dirLen = pathLen - fileLen - 1;
    if (dirLen == 0 || dirLen >= n) return false;
    std::memcpy(out, path, dirLen);
    out[dirLen] = '\0';

    // Find the last path separator within the slot directory.
    std::size_t i = dirLen;
    while (i > 0 && out[i - 1] != '\\' && out[i - 1] != '/') --i;
    if (i == 0) return false;

    std::size_t sepPos = i - 1; // separator preceding the slot component
    if (sepPos < 4) return false;

    // The four characters immediately before that separator must be
    // "save" (case-insensitive) and must form a complete component.
    if (spm_strnicmp(out + sepPos - 4, "save", 4) != 0) return false;

    std::size_t saveStart = sepPos - 4;
    if (saveStart != 0) {
        char before = out[saveStart - 1];
        if (before != '\\' && before != '/') return false;
    }

    // The slot component itself must be non-empty.
    if (sepPos + 1 >= dirLen) return false;

    return true;
}

// Debounce state for load events.
struct SaveDebounceState {
    unsigned long lastLoadTick = 0;
    char lastLoadDir[1024] = {0};
};

inline void InvalidateLoadDebounce(SaveDebounceState& st) {
    st.lastLoadDir[0] = '\0';
    st.lastLoadTick = 0;
}

// Returns true if this load should be suppressed (debounced).
inline bool DebounceLoad(SaveDebounceState& st, const char* dir,
                         unsigned long nowMs, unsigned long windowMs) {
    if (!dir) return false;
    if (st.lastLoadDir[0] != '\0' &&
        (std::uint32_t)(nowMs - st.lastLoadTick) < (std::uint32_t)windowMs &&
        spm_stricmp(st.lastLoadDir, dir) == 0) {
        return true;
    }
    st.lastLoadTick = nowMs;
    std::size_t len = std::strlen(dir);
    if (len >= sizeof(st.lastLoadDir)) len = sizeof(st.lastLoadDir) - 1;
    std::memcpy(st.lastLoadDir, dir, len);
    st.lastLoadDir[len] = '\0';
    return false;
}

} // namespace persistence
} // namespace wrsre

// Backwards-compatible free-function names expected by persistence.cpp
// and tests.
using wrsre::persistence::SaveDirOf;
using wrsre::persistence::SaveDebounceState;
using wrsre::persistence::InvalidateLoadDebounce;
using wrsre::persistence::DebounceLoad;

#endif // WRSRE_PERSISTENCE_SAVE_PATH_MATCH_H



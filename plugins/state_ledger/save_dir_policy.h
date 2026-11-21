// save_dir_policy.h - pure policy helpers for state_ledger save directory pinning.
//
// Background autosaves (e.g. "media_soviet/save/autosave3") must not hijack
// an already-established save directory when TSM_SAVE_WRITING fires; otherwise
// a single session's monthly CSV is split across the loaded save and whatever
// autosave slot rotated in mid-session.
#pragma once

#include <cctype>
#include <cstddef>

namespace state_ledger {
namespace policy {

inline int ToLowerAscii(unsigned char c)
{
    return (c >= 'A' && c <= 'Z') ? (c + ('a' - 'A')) : c;
}

inline bool StartsWithCaseInsensitive(const char* str, const char* prefix)
{
    if (!str || !prefix) return false;
    while (*prefix) {
        if (ToLowerAscii((unsigned char)*str) !=
            ToLowerAscii((unsigned char)*prefix)) {
            return false;
        }
        ++str;
        ++prefix;
    }
    return true;
}

// Returns true if the final non-empty path component of `dir` is an engine
// autosave slot (e.g. "autosave", "autosave1", "autosave3").
inline bool IsAutoSaveSlot(const char* dir)
{
    if (!dir || !dir[0]) return false;

    std::size_t end = 0;
    while (dir[end] != '\0') ++end;
    while (end > 0 && (dir[end - 1] == '/' || dir[end - 1] == '\\')) --end;
    if (end == 0) return false;

    std::size_t start = end;
    while (start > 0 && dir[start - 1] != '/' && dir[start - 1] != '\\') {
        --start;
    }

    const char* slot = dir + start;
    const std::size_t slotLen = end - start;
    if (slotLen < 8) return false; // strlen("autosave")
    if (!StartsWithCaseInsensitive(slot, "autosave")) return false;

    // Require either exact "autosave" or trailing digits/suffix like
    // "autosave3" or "autosave_01".
    const char next = slot[8];
    return next == '\0' || std::isdigit((unsigned char)next) ||
           next == '_' || next == '-' || next == ' ';
}

// Decides whether `g_saveDir` should be updated to `eventDir`.
//   - On load (`isLoad == true`): always follow the loaded save directory.
//   - On write (`isLoad == false`): follow manual saves, and initialize if
//     no save directory is known yet, but ignore autosaves once a directory
//     is already pinned.
inline bool ShouldUpdateSaveDir(const char* currentDir,
                                const char* eventDir,
                                bool isLoad)
{
    if (!eventDir || !eventDir[0]) return false;
    if (isLoad) return true;
    if (!currentDir || !currentDir[0]) return true;
    return !IsAutoSaveSlot(eventDir);
}

} // namespace policy
} // namespace state_ledger

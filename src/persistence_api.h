// persistence_api.h - the save/load event contract.
//
// persistence is a Level 0 service: it depends on nothing, and it exists
// so that no other plugin has to hook fopen.
//
// The game opens stats.ini for write when a save is being written, and
// header.bin for read when a world is being loaded. That is the whole of
// the trigger. persistence watches both through the four CRT openers the
// game actually uses (fopen, fopen_s, _wfopen, _wfopen_s), resolves the
// save directory from the path it sees, and fires a callback for every
// registered handler.
//
// A handler does one of two things:
//
//   SAVE_WRITING   write its data into <dir>/<filename>
//   SAVE_LOADING   read its data back from <dir>/<filename>
//
// and the two atomic-write helpers make the first one a three-line job.
// The alternative - each plugin hooking fopen itself - is four copies of
// the same save-directory logic, four chances to get the atomic-rename
// wrong, and four places that must agree on what "a save" means. It is
// not a mechanism anyone benefits from duplicating.
//
//
// WHY A SAVE EVENT AND NOT "A FILE OPEN"
//
// A plugin should not have to know that the game writes stats.ini at all.
// It should not care which file the trigger comes from, whether the game
// writes one save file or twenty, or whether the save format changed in
// a patch. It should say "when the game saves, call me with the
// directory" and get on with its actual job.
//
// This is the same split the loader itself uses for plugins: a Level 0
// service owns the low-level mechanism, and every plugin above it sees a
// clean, domain-shaped event.

#ifndef TSM_PERSISTENCE_API_H
#define TSM_PERSISTENCE_API_H

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TSM_SERVICE_PERSISTENCE "persistence"
#define TSM_PERSISTENCE_VERSION 1u

typedef enum TsmSaveEvent {
    // The game is writing a save into `dir`. Handle this by writing
    // your own file into that directory with writeAtomic().
    TSM_SAVE_WRITING = 0,

    // A world is being loaded from `dir`. Handle this by reading your
    // own file back with readFile() and restoring whatever state you
    // saved.
    TSM_SAVE_LOADING = 1
} TsmSaveEvent;

// Called from the game's own save/load thread, inside an fopen. It must
// not block for long and it must not itself open files through the game's
// CRT (that would re-enter this hook). Use the helpers below instead.
//
// `dir` is a NUL-terminated ANSI path to the save directory, with no
// trailing separator. It is valid only for the duration of the call -
// copy it if you need to keep it.
typedef void (*TsmSaveCallback)(TsmSaveEvent event,
                                const char* dir,
                                void* userData);

typedef struct TsmPersistenceApi {
    unsigned structSize;

    // --- subscriptions -----------------------------------------------------
    //
    // `pluginName` is only used for logging; it is not the handler's
    // identity. Returns a non-zero handle, or 0 on failure (table full,
    // null callback, null name).

    int  (*registerHandler)(const char* pluginName,
                            TsmSaveCallback cb, void* userData);
    void (*unregisterHandler)(int handle);

    // --- atomic file helpers ----------------------------------------------
    //
    // These are the only safe way to touch the filesystem from inside a
    // save handler. Doing it with fopen would re-enter the hook this
    // service installed; doing it with CreateFileA directly is fine, but
    // the atomic-rename dance has to be exactly right and there is no
    // reason for every plugin to re-derive it.

    // Write <dir>/<filename>.tmp, flush, then atomically rename over
    // <dir>/<filename>. Creates the file if it does not exist. Returns
    // 1 on success, 0 on any failure (logged).
    int  (*writeAtomic)(const char* dir, const char* filename,
                        const void* data, size_t len);

    // Read <dir>/<filename> whole into a buffer owned by persistence.dll.
    // Returns NULL on failure. Release with TsmPersistenceFree(api, buf)
    // (or api->freeBuffer when present) so the allocation is freed on the
    // provider's own /MT CRT heap. `*outLen` is set on success and left
    // untouched on failure.
    void* (*readFile)(const char* dir, const char* filename,
                      size_t* outLen);

    // --- v1, appended (check structSize before calling) --------------------
    //
    // Free a buffer returned by readFile() on persistence.dll's own CRT heap.
    // Safe to call with NULL.
    void  (*freeBuffer)(void* buf);
} TsmPersistenceApi;

// ABI-safe helper: frees a buffer returned by readFile() using the provider's
// freeBuffer when available (structSize check), falling back to free() only
// against older persistence.dll builds.
static inline void TsmPersistenceFree(const TsmPersistenceApi* api, void* buf)
{
    if (!buf) return;
    if (api &&
        api->structSize >= offsetof(TsmPersistenceApi, freeBuffer) +
                               sizeof(api->freeBuffer) &&
        api->freeBuffer)
    {
        api->freeBuffer(buf);
        return;
    }
    free(buf);
}

#ifdef __cplusplus
}
#endif

#endif // TSM_PERSISTENCE_API_H

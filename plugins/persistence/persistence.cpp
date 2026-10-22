// persistence - the save/load event service.
//
// See persistence_api.h for the contract.

#include "../../src/tesmio_plugin.h"
#include "../../src/persistence_api.h"

// ---------------------------------------------------------------- config

#define MAX_HANDLERS     32
#define HANDLER_NAME_LEN 32
#define TRIGGER_NAME_LEN 64
#define MAX_FILE_SIZE    (16u * 1024u * 1024u)

static int  g_enabled      = 1;
static int  g_probe        = 1;
static char g_writeTrigger[TRIGGER_NAME_LEN] = "stats.ini";
static char g_readTrigger [TRIGGER_NAME_LEN] = "workers.bin";

// ---------------------------------------------------------------- handlers

struct Handler {
    char             name[HANDLER_NAME_LEN];
    TsmSaveCallback  cb;
    void*            userData;
    int              active;
};

static Handler g_handlers[MAX_HANDLERS];
static int     g_handlerCount;

static int RegisterHandler(const char* pluginName,
                           TsmSaveCallback cb, void* userData)
{
    if (!cb || !pluginName) return 0;

    EnterCriticalSection(&g_lock);
    if (g_handlerCount >= MAX_HANDLERS) {
        LeaveCriticalSection(&g_lock);
        Logf("persistence  handler table full (%d) - refusing %s",
             MAX_HANDLERS, pluginName);
        return 0;
    }

    int i = g_handlerCount++;
    Handler* h = &g_handlers[i];
    strncpy_s(h->name, sizeof(h->name), pluginName, _TRUNCATE);
    h->cb       = cb;
    h->userData = userData;
    h->active   = 1;
    LeaveCriticalSection(&g_lock);

    Logf("persistence  registered handler \"%s\" (#%d)", h->name, i + 1);
    return i + 1;
}

static void UnregisterHandler(int handle)
{
    if (handle < 1 || handle > g_handlerCount) return;
    EnterCriticalSection(&g_lock);
    g_handlers[handle - 1].active = 0;
    LeaveCriticalSection(&g_lock);
}

// ---------------------------------------------------------------- save dir

static bool SaveDirOf(const char* path, const char* filename,
                      char* out, size_t n)
{
    if (!path || !filename || !out || n == 0) return false;

    size_t pathLen = strlen(path);
    size_t fileLen = strlen(filename);
    if (pathLen < fileLen + 2 || pathLen >= n) return false;

    if (_stricmp(path + pathLen - fileLen, filename) != 0) return false;
    char beforeSep = path[pathLen - fileLen - 1];
    if (beforeSep != '\\' && beforeSep != '/') return false;

    size_t dirLen = pathLen - fileLen - 1;
    if (dirLen == 0 || dirLen >= n) return false;
    memcpy(out, path, dirLen);
    out[dirLen] = 0;

    size_t i = dirLen;
    while (i > 0 && out[i - 1] != '\\' && out[i - 1] != '/') i--;
    if (i == 0) return false;

    size_t sepPos = i - 1;
    if (sepPos < 4) return false;
    if (_strnicmp(out + sepPos - 4, "save", 4) != 0) return false;

    size_t saveStart = sepPos - 4;
    if (saveStart == 0) return true;
    char before = out[saveStart - 1];
    return before == '\\' || before == '/';
}

static bool SaveDirOfW(const wchar_t* path, const char* filename,
                       char* out, size_t n)
{
    if (!path) return false;
    char ansi[MAX_PATH * 2];
    if (!WideCharToMultiByte(CP_ACP, 0, path, -1, ansi, sizeof(ansi),
                             NULL, NULL))
        return false;
    return SaveDirOf(ansi, filename, out, n);
}

// ---------------------------------------------------------------- file helpers

static int WriteAtomicImpl(const char* dir, const char* filename,
                           const void* data, size_t len)
{
    if (!dir || !filename || !data || len == 0 || len > MAX_FILE_SIZE)
        return 0;

    char tmp[MAX_PATH * 2];
    char dst[MAX_PATH * 2];
    if (_snprintf_s(tmp, sizeof(tmp), _TRUNCATE, "%s\\%s.tmp",
                    dir, filename) < 0) return 0;
    if (_snprintf_s(dst, sizeof(dst), _TRUNCATE, "%s\\%s",
                    dir, filename) < 0) return 0;

    HANDLE h = CreateFileA(tmp, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        Logf("persistence  could not create %s (%lu)", tmp, GetLastError());
        return 0;
    }

    DWORD wrote = 0;
    if (!WriteFile(h, data, (DWORD)len, &wrote, NULL) || wrote != (DWORD)len) {
        Logf("persistence  short write on %s (%lu, %lu of %zu bytes)",
             tmp, GetLastError(), wrote, len);
        CloseHandle(h);
        DeleteFileA(tmp);
        return 0;
    }

    FlushFileBuffers(h);
    CloseHandle(h);

    if (!MoveFileExA(tmp, dst,
                     MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        Logf("persistence  could not rename %s -> %s (%lu)",
             tmp, dst, GetLastError());
        DeleteFileA(tmp);
        return 0;
    }
    return 1;
}

static void* ReadFileImpl(const char* dir, const char* filename,
                          size_t* outLen)
{
    if (!dir || !filename || !outLen) return NULL;

    char path[MAX_PATH * 2];
    if (_snprintf_s(path, sizeof(path), _TRUNCATE, "%s\\%s",
                    dir, filename) < 0) return NULL;

    HANDLE h = CreateFileA(path, GENERIC_READ,
                           FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                           NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return NULL;

    LARGE_INTEGER sz = { 0 };
    if (!GetFileSizeEx(h, &sz) || sz.QuadPart <= 0 ||
        sz.QuadPart > (LONGLONG)MAX_FILE_SIZE)
    {
        CloseHandle(h);
        return NULL;
    }

    size_t want = (size_t)sz.QuadPart;
    void* buf = malloc(want);
    if (!buf) { CloseHandle(h); return NULL; }

    DWORD got = 0;
    if (!ReadFile(h, buf, (DWORD)want, &got, NULL) || got != want) {
        Logf("persistence  short read on %s (%lu, %lu of %zu bytes)",
             path, GetLastError(), got, want);
        free(buf);
        CloseHandle(h);
        return NULL;
    }
    CloseHandle(h);

    *outLen = want;
    return buf;
}

// ---------------------------------------------------------------- dispatch

static void Dispatch(TsmSaveEvent event, const char* dir)
{
    static const char* kEventName[2] = { "writing", "loading" };

    if (g_probe)
        Logf("persistence  %s -> %s", kEventName[event], dir);

    EnterCriticalSection(&g_lock);
    for (int i = 0; i < g_handlerCount; i++) {
        Handler* h = &g_handlers[i];
        if (!h->active) continue;
        __try { h->cb(event, dir, h->userData); }
        __except (FaultFilter("persistence handler",
                              GetExceptionInformation()))
        {
            Logf("persistence  handler \"%s\" faulted on %s; disabling it for this session",
                 h->name, kEventName[event]);
            h->active = 0;
        }
    }
    LeaveCriticalSection(&g_lock);
}

// ---------------------------------------------------------------- hooks

static __declspec(thread) int t_inHook = 0;
static DWORD g_lastLoadTick = 0;
static char  g_lastLoadDir[MAX_PATH * 2] = {0};

static void MaybeDispatch(const char* path, const char* mode)
{
    if (!g_enabled || t_inHook || !path || !mode) return;
    if (mode[0] != 'w' && mode[0] != 'a' && mode[0] != 'r') return;

    const bool writing = (mode[0] == 'w' || mode[0] == 'a');
    const char* trigger = writing ? g_writeTrigger : g_readTrigger;

    char dir[MAX_PATH * 2];
    if (!SaveDirOf(path, trigger, dir, sizeof(dir))) return;

    // Debounce under lock: prevent duplicate LOADING triggers for the same directory within 3 seconds
    if (!writing) {
        DWORD now = GetTickCount();
        bool skip = false;
        EnterCriticalSection(&g_lock);
        if ((int)(now - g_lastLoadTick) < 3000 && _stricmp(g_lastLoadDir, dir) == 0) {
            skip = true;
        } else {
            g_lastLoadTick = now;
            strncpy_s(g_lastLoadDir, sizeof(g_lastLoadDir), dir, _TRUNCATE);
        }
        LeaveCriticalSection(&g_lock);
        if (skip) return;
    }

    t_inHook = 1;
    __try { Dispatch(writing ? TSM_SAVE_WRITING : TSM_SAVE_LOADING, dir); }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        Logf("persistence  fault in save/load dispatch - ignored");
    }
    t_inHook = 0;
}

static void MaybeDispatchW(const wchar_t* path, const wchar_t* mode)
{
    if (!g_enabled || t_inHook || !path || !mode) return;
    if (mode[0] != L'w' && mode[0] != L'a' && mode[0] != L'r') return;

    const bool writing = (mode[0] == L'w' || mode[0] == L'a');
    const char* trigger = writing ? g_writeTrigger : g_readTrigger;

    char dir[MAX_PATH * 2];
    if (!SaveDirOfW(path, trigger, dir, sizeof(dir))) return;

    // Debounce under lock: prevent duplicate LOADING triggers for the same directory within 3 seconds
    if (!writing) {
        DWORD now = GetTickCount();
        bool skip = false;
        EnterCriticalSection(&g_lock);
        if ((int)(now - g_lastLoadTick) < 3000 && _stricmp(g_lastLoadDir, dir) == 0) {
            skip = true;
        } else {
            g_lastLoadTick = now;
            strncpy_s(g_lastLoadDir, sizeof(g_lastLoadDir), dir, _TRUNCATE);
        }
        LeaveCriticalSection(&g_lock);
        if (skip) return;
    }

    t_inHook = 1;
    __try { Dispatch(writing ? TSM_SAVE_WRITING : TSM_SAVE_LOADING, dir); }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        Logf("persistence  fault in save/load dispatch - ignored");
    }
    t_inHook = 0;
}

typedef FILE*   (__cdecl* t_fopen)   (const char*, const char*);
typedef errno_t (__cdecl* t_fopen_s) (FILE**, const char*, const char*);
typedef FILE*   (__cdecl* t_wfopen)  (const wchar_t*, const wchar_t*);
typedef errno_t (__cdecl* t_wfopen_s)(FILE**, const wchar_t*, const wchar_t*);

static t_fopen    o_fopen    = nullptr;
static t_fopen_s  o_fopen_s  = nullptr;
static t_wfopen   o_wfopen   = nullptr;
static t_wfopen_s o_wfopen_s = nullptr;

static FILE* __cdecl h_fopen(const char* path, const char* mode)
{
    MaybeDispatch(path, mode);
    return o_fopen(path, mode);
}

static errno_t __cdecl h_fopen_s(FILE** f, const char* path, const char* mode)
{
    MaybeDispatch(path, mode);
    return o_fopen_s(f, path, mode);
}

static FILE* __cdecl h_wfopen(const wchar_t* path, const wchar_t* mode)
{
    MaybeDispatchW(path, mode);
    return o_wfopen(path, mode);
}

static errno_t __cdecl h_wfopen_s(FILE** f, const wchar_t* path,
                                  const wchar_t* mode)
{
    MaybeDispatchW(path, mode);
    return o_wfopen_s(f, path, mode);
}

// ---------------------------------------------------------------- the API

static const TsmPersistenceApi kApi = {
    sizeof(TsmPersistenceApi),
    RegisterHandler,
    UnregisterHandler,
    WriteAtomicImpl,
    ReadFileImpl,
};

// ---------------------------------------------------------------- settings

static void ReadSettings()
{
    const char* ini = "plugins\\persistence.ini";
    char v[64];

    g_enabled = H->configInt(ini, "persistence", "enabled", 1);
    g_probe   = H->configInt(ini, "persistence", "probe",   1);

    if (H->configString(ini, "persistence", "write_trigger",
                        v, sizeof(v), "") && v[0])
        strncpy_s(g_writeTrigger, sizeof(g_writeTrigger), v, _TRUNCATE);
    if (H->configString(ini, "persistence", "read_trigger",
                        v, sizeof(v), "") && v[0])
        strncpy_s(g_readTrigger, sizeof(g_readTrigger), v, _TRUNCATE);

    if (!g_writeTrigger[0]) strncpy_s(g_writeTrigger, sizeof(g_writeTrigger),
                                      "stats.ini", _TRUNCATE);
    if (!g_readTrigger[0])  strncpy_s(g_readTrigger, sizeof(g_readTrigger),
                                      "workers.bin", _TRUNCATE);
}

// ---------------------------------------------------------------- lifecycle

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host,
                                                   TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "persistence";
    info->version = "1.0";

    ReadSettings();
    if (!g_enabled) {
        Logf("persistence  disabled");
        return 1;
    }

    H->provide(TSM_SERVICE_PERSISTENCE, TSM_PERSISTENCE_VERSION, &kApi);
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;

    bool ok = true;

    ok = PatchIat(g_exe, DLL_STDIO, "fopen",
                  (void*)h_fopen, (void**)&o_fopen, "fopen") && ok;
    ok = PatchIat(g_exe, DLL_STDIO, "fopen_s",
                  (void*)h_fopen_s, (void**)&o_fopen_s, "fopen_s") && ok;
    ok = PatchIat(g_exe, DLL_STDIO, "_wfopen",
                  (void*)h_wfopen, (void**)&o_wfopen, "_wfopen") && ok;
    ok = PatchIat(g_exe, DLL_STDIO, "_wfopen_s",
                  (void*)h_wfopen_s, (void**)&o_wfopen_s, "_wfopen_s") && ok;

    if (!ok) {
        Logf("persistence  one or more CRT openers could not be hooked");
    }

    Logf("persistence  live: %s (write) / %s (read)",
         g_writeTrigger, g_readTrigger);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }
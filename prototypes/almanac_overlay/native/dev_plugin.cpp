// Explicit development DLL entry points. Not discovered by normal build.bat.
#include "graphics_adapter.h"
#include "sha256.h"
#include <cstddef>
#include <cstring>

namespace {
const TsmHost* host = nullptr;
bool allowSynthetic = false;
bool started = false;
constexpr char ExeHash[] = "296644a9f207d609031fc2ae73fed2dcb34619a1d55a35d1c7b51965ce6841b8";
constexpr char EngineHash[] = "65fcb6845c3cb7c25a22121f25904a855ea88657e31a3b50a15adac26e231516";

// Synchronous bounded file I/O + pure hash only. No crypto-provider loading,
// thread waits, ImGui, window creation or D3D calls under the loader lock.
// Checks backing file identity, NOT hostile in-memory modification resistance.
bool MatchModuleFile(HMODULE module, const char* expected) {
    wchar_t path[MAX_PATH]{};
    const DWORD length = GetModuleFileNameW(module, path, MAX_PATH);
    if (!length || length >= MAX_PATH) return false;
    HANDLE file = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, nullptr,
                              OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
    if (file == INVALID_HANDLE_VALUE) return false;
    struct CloseFile { HANDLE file; ~CloseFile() { CloseHandle(file); } } close{file};
    LARGE_INTEGER size{};
    if (!GetFileSizeEx(file, &size) || size.QuadPart <= 0 || size.QuadPart > 256LL * 1024 * 1024)
        return false;
    const ULONGLONG began = GetTickCount64();
    almanac::BinarySha256 hash;
    unsigned char buffer[16384];
    LONGLONG total = 0;
    for (;;) {
        if (GetTickCount64() - began > 10000) return false;
        DWORD count = 0;
        if (!ReadFile(file, buffer, sizeof(buffer), &count, nullptr)) return false;
        if (!count) break;
        total += count;
        if (total > size.QuadPart) return false;
        hash.Update(buffer, count);
    }
    char digest[65]{};
    hash.Finish(digest);
    return total == size.QuadPart && std::strcmp(digest, expected) == 0;
}
}

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion() { return TSM_API_VERSION; }

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* api, TsmPluginInfo* info) {
    if (!api || !info || api->apiVersion != TSM_API_VERSION ||
        api->structSize < offsetof(TsmHost, configInt) + sizeof(api->configInt) ||
        !api->log || !api->configInt || !api->findIatSlot || !api->readablePtr) return 1;
    info->name = "almanac_overlay_dev";
    info->version = "0.1-experimental";
    if (!api->configInt("plugins\\almanac_overlay_dev.ini", "almanac_overlay_dev", "enabled", 1))
        return 1; // no hooks, services or persistent resources: safe to decline Init
    if (!api->exeModule || !api->engineModule ||
        api->exeModule != GetModuleHandleW(nullptr) ||
        api->engineModule != GetModuleHandleW(L"C3DDLL64.dll") ||
        !MatchModuleFile(static_cast<HMODULE>(api->exeModule), ExeHash) ||
        !MatchModuleFile(static_cast<HMODULE>(api->engineModule), EngineHash)) {
        api->log("almanac dev: missing/mismatched binary identity or file read failure; inactive");
        return 1;
    }
    // Also validate the loaded call instruction used by the return-RVA gate.
    const auto* call = static_cast<const unsigned char*>(api->engineModule) + 0x148BC;
    constexpr unsigned char expectedCall[] = {0xFF, 0x15, 0x8E, 0x6E, 0x15, 0x00};
    if (!api->readablePtr(call, sizeof(expectedCall)) ||
        std::memcmp(call, expectedCall, sizeof(expectedCall)) != 0) return 1;
    host = api;
    allowSynthetic = api->configInt("plugins\\almanac_overlay_dev.ini", "almanac_overlay_dev",
                                   "allow_synthetic_without_modal_tracking", 1) != 0;
    api->log("almanac dev: matching files/call site; synthetic drawing allowed=%d", allowSynthetic);
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart() {
    if (!host || started) return 1;
    started = true;
    try {
        // Observer install only: no HWND/graphics initialization here.
        if (!almanac::InstallGraphicsAdapter(host, true)) return 1;
        almanac::SetGraphicsAdapterPermission(allowSynthetic);
        host->log("almanac dev: observer installed; synthetic panel opens on first eligible frame; F8 hides/reopens; no live Finance/input capture");
        return 0;
    } catch (...) {
        almanac::SetGraphicsAdapterPermission(false);
        host->log("almanac dev: startup exception; keep DLL resident, drawing disabled");
        return 1;
    }
}
// No custom DllMain or detach cleanup. Installed code/references stay resident.

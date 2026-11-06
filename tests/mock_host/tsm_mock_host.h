// tsm_mock_host.h - portable headless mock host for WRSRE plugins.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#include "../../src/tesmio_plugin.h"
#pragma GCC diagnostic pop
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#include "../../src/game_world.h"
#pragma GCC diagnostic pop
#include "../../plugins/persistence/save_path_match.h"

namespace wrsre {
namespace mock {

class MockTsmEnvironment {
public:
    MockTsmEnvironment();
    ~MockTsmEnvironment();

    TsmHost* host() { return &host_; }
    const TsmHost* host() const { return &host_; }

    // Service registry
    int provide(const char* service, unsigned version, const void* iface);
    const void* consume(const char* service, unsigned version);

    // Config
    void setConfigInt(const char* ini, const char* section, const char* key, int value);
    void setConfigString(const char* ini, const char* section, const char* key, const char* value);
    int configInt(const char* iniName, const char* section, const char* key, int fallback);
    int configString(const char* iniName, const char* section, const char* key, char* out, int outSize, const char* fallback);

    int readablePtr(const void* p, std::size_t n);

    const char* baseDir() const { return baseDir_.c_str(); }
    const char* pluginDir() const { return pluginDir_.c_str(); }
    const char* vfsRoot() const { return vfsRoot_.c_str(); }

    // Persistence helpers in temp dir
    int writeAtomic(const char* dir, const char* filename, const void* data, std::size_t len);
    void* readFile(const char* dir, const char* filename, std::size_t* outLen);

    const char* tempDir() const { return tempDir_.c_str(); }

    // Synthetic PE / game object
    unsigned char* exeBase() { return exeBase_.data(); }
    std::size_t exeSize() const { return exeBase_.size(); }

    // Allocators
    void* allocateBuilding();
    void* allocatePerson();
    void* gameObject();

    // Sim clock
    struct SimClock {
        int day = 0;
        int month = 1;
        int quarter = 1;
        int year = 2020;
        int tick = 0;
        int daysPerMonth = 30;
    };
    SimClock& clock() { return clock_; }
    const SimClock& clock() const { return clock_; }
    void stepTick();
    void stepDay();
    void stepDays(int n);

    // Memory range tracking
    void trackRange(const void* p, std::size_t n);

    static MockTsmEnvironment* current();

private:
    void buildPeImage();
    void buildHostTable();
    static int provideThunk(const char* s, unsigned v, const void* i);
    static const void* consumeThunk(const char* s, unsigned v);
    static int configIntThunk(const char* i, const char* sec, const char* key, int fb);
    static int configStringThunk(const char* i, const char* sec, const char* key, char* out, int sz, const char* fb);
    static int readablePtrThunk(const void* p, std::size_t n);
    static void logThunk(const char* fmt, ...);

    TsmHost host_{};
    std::string baseDir_;
    std::string pluginDir_;
    std::string vfsRoot_;
    std::string tempDir_;

    struct Service { std::string name; unsigned version; const void* iface; };
    std::vector<Service> services_;

    struct ConfigKey { std::string ini; std::string section; std::string key; };
    std::map<std::string, std::map<std::string, std::map<std::string, std::string>>> config_;

    std::vector<unsigned char> exeBase_;
    std::vector<void*> buildings_;
    std::vector<void*> persons_;
    std::vector<std::pair<const void*, std::size_t>> ranges_;

    SimClock clock_;

    static MockTsmEnvironment* current_;
};

} // namespace mock
} // namespace wrsre

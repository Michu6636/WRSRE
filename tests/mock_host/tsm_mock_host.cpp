#include "tsm_mock_host.h"

#include <windows.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <stdexcept>

namespace wrsre {
namespace mock {

MockTsmEnvironment* MockTsmEnvironment::current_ = nullptr;

MockTsmEnvironment* MockTsmEnvironment::current() { return current_; }

MockTsmEnvironment::MockTsmEnvironment() {
    current_ = this;
    char tmp[MAX_PATH];
    GetTempPathA(MAX_PATH, tmp);
    tempDir_ = std::string(tmp) + "wrsre_mock_host";
    std::filesystem::create_directories(tempDir_);
    baseDir_ = tempDir_;
    pluginDir_ = tempDir_ + "\\plugins";
    vfsRoot_ = tempDir_ + "\\vfs";
    std::filesystem::create_directories(pluginDir_);
    std::filesystem::create_directories(vfsRoot_);

    buildPeImage();
    buildHostTable();

    // track exe range
    trackRange(exeBase_.data(), exeBase_.size());

    TsmAddrInit(host());
}

MockTsmEnvironment::~MockTsmEnvironment() {
    for (void* p : buildings_) std::free(p);
    for (void* p : persons_) std::free(p);
    current_ = nullptr;
}

void MockTsmEnvironment::trackRange(const void* p, std::size_t n) {
    ranges_.push_back({p, n});
}

int MockTsmEnvironment::readablePtr(const void* p, std::size_t n) {
    if (!p || n == 0) return 0;
    uintptr_t addr = (uintptr_t)p;
    for (auto& r : ranges_) {
        uintptr_t base = (uintptr_t)r.first;
        if (addr >= base && (addr + n) <= (base + r.second)) return 1;
    }
    return 0;
}

int MockTsmEnvironment::readablePtrThunk(const void* p, std::size_t n) {
    if (!current_) return 0;
    return current_->readablePtr(p, n);
}

void MockTsmEnvironment::buildPeImage() {
    const std::size_t size = 0x00A00000; // 10MB
    exeBase_.assign(size, 0);
    unsigned char* b = exeBase_.data();

    // DOS header
    b[0] = 'M'; b[1] = 'Z';
    uint32_t lfanew = 0x80;
    std::memcpy(b + 0x3C, &lfanew, 4);

    // PE signature
    std::memcpy(b + lfanew, "PE\0\0", 4);

    // COFF header
    uint16_t machine = 0x8664;
    uint16_t numSec = 1;
    uint32_t stamp = TSM_BASELINE_EXE_STAMP;
    uint16_t optSize = 0xF0;
    std::memcpy(b + lfanew + 4, &machine, 2);
    std::memcpy(b + lfanew + 6, &numSec, 2);
    std::memcpy(b + lfanew + 8, &stamp, 4);
    std::memcpy(b + lfanew + 20, &optSize, 2);

    // Optional header magic
    uint16_t magic = 0x020B;
    std::memcpy(b + lfanew + 24, &magic, 2);

    // Section header at lfanew + 24 + optSize
    uint32_t secOff = lfanew + 24 + optSize;
    // name
    std::memcpy(b + secOff, ".text", 6);
    uint32_t vsize = (uint32_t)(size - 0x1000);
    uint32_t vaddr = 0x1000;
    uint32_t flags = 0x60000020; // code + execute + read
    std::memcpy(b + secOff + 8, &vsize, 4);
    std::memcpy(b + secOff + 12, &vaddr, 4);
    std::memcpy(b + secOff + 36, &flags, 4);
}

void MockTsmEnvironment::buildHostTable() {
    std::memset(&host_, 0, sizeof(host_));
    host_.apiVersion = TSM_API_VERSION;
    host_.structSize = sizeof(TsmHost);
    host_.exeModule = nullptr;
    host_.exeBase = exeBase_.data();
    host_.exeSize = exeBase_.size();
    host_.engineModule = nullptr;
    host_.baseDir = baseDir_.c_str();
    host_.pluginDir = pluginDir_.c_str();
    host_.log = &MockTsmEnvironment::logThunk;
    host_.findIatSlot = nullptr;
    host_.patchIat = nullptr;
    host_.installInlineHook = nullptr;
    host_.allocNear = nullptr;
    host_.readablePtr = &MockTsmEnvironment::readablePtrThunk;
    host_.faultFilter = nullptr;
    host_.configInt = &MockTsmEnvironment::configIntThunk;
    host_.configString = &MockTsmEnvironment::configStringThunk;
    host_.provide = &MockTsmEnvironment::provideThunk;
    host_.consume = &MockTsmEnvironment::consumeThunk;
    host_.vfsRoot = vfsRoot_.c_str();
}

int MockTsmEnvironment::provide(const char* service, unsigned version, const void* iface) {
    if (!service || !iface) return 0;
    for (auto& s : services_) {
        if (wrsre::persistence::spm_stricmp(s.name.c_str(), service) == 0 && s.version == version) return 0;
    }
    services_.push_back({service, version, iface});
    return 1;
}

int MockTsmEnvironment::provideThunk(const char* s, unsigned v, const void* i) {
    if (!current_) return 0;
    return current_->provide(s, v, i);
}

const void* MockTsmEnvironment::consume(const char* service, unsigned version) {
    if (!service) return nullptr;
    for (auto& s : services_) {
        if (wrsre::persistence::spm_stricmp(s.name.c_str(), service) == 0 && s.version == version) return s.iface;
    }
    return nullptr;
}

const void* MockTsmEnvironment::consumeThunk(const char* s, unsigned v) {
    if (!current_) return nullptr;
    return current_->consume(s, v);
}

void MockTsmEnvironment::setConfigInt(const char* ini, const char* section, const char* key, int value) {
    config_[ini ? ini : ""][section ? section : ""][key ? key : ""] = std::to_string(value);
}

void MockTsmEnvironment::setConfigString(const char* ini, const char* section, const char* key, const char* value) {
    config_[ini ? ini : ""][section ? section : ""][key ? key : ""] = value ? value : "";
}

int MockTsmEnvironment::configInt(const char* iniName, const char* section, const char* key, int fallback) {
    auto it = config_.find(iniName ? iniName : "");
    if (it == config_.end()) return fallback;
    auto it2 = it->second.find(section ? section : "");
    if (it2 == it->second.end()) return fallback;
    auto it3 = it2->second.find(key ? key : "");
    if (it3 == it2->second.end()) return fallback;
    try { return std::stoi(it3->second); } catch (...) { return fallback; }
}

int MockTsmEnvironment::configIntThunk(const char* i, const char* sec, const char* key, int fb) {
    if (!current_) return fb;
    return current_->configInt(i, sec, key, fb);
}

int MockTsmEnvironment::configString(const char* iniName, const char* section, const char* key, char* out, int outSize, const char* fallback) {
    std::string tmp;
    auto it = config_.find(iniName ? iniName : "");
    if (it != config_.end()) {
        auto it2 = it->second.find(section ? section : "");
        if (it2 != it->second.end()) {
            auto it3 = it2->second.find(key ? key : "");
            if (it3 != it2->second.end()) tmp = it3->second;
        }
    }
    const char* src = tmp.empty() ? (fallback ? fallback : "") : tmp.c_str();
    if (out && outSize > 0) {
        std::strncpy(out, src, outSize - 1);
        out[outSize - 1] = '\0';
    }
    return (int)std::strlen(src);
}

int MockTsmEnvironment::configStringThunk(const char* i, const char* sec, const char* key, char* out, int sz, const char* fb) {
    if (!current_) return 0;
    return current_->configString(i, sec, key, out, sz, fb);
}

void MockTsmEnvironment::logThunk(const char* fmt, ...) {
    if (!fmt) return;
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    std::vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    std::cout << buf << "\n";
}

int MockTsmEnvironment::writeAtomic(const char* dir, const char* filename, const void* data, std::size_t len) {
    if (!dir || !filename || !data || len == 0) return 0;
    std::filesystem::path d(dir);
    std::filesystem::create_directories(d);
    std::filesystem::path tmp = d / (std::string(filename) + ".tmp");
    std::filesystem::path dst = d / filename;
    {
        std::ofstream f(tmp, std::ios::binary | std::ios::trunc);
        if (!f) return 0;
        f.write((const char*)data, (std::streamsize)len);
        if (!f) return 0;
        f.flush();
    }
    std::error_code ec;
    std::filesystem::rename(tmp, dst, ec);
    if (ec) {
        // try replace
        std::filesystem::remove(dst, ec);
        std::filesystem::rename(tmp, dst, ec);
        if (ec) return 0;
    }
    return 1;
}

void* MockTsmEnvironment::readFile(const char* dir, const char* filename, std::size_t* outLen) {
    if (!dir || !filename || !outLen) return nullptr;
    std::filesystem::path p = std::filesystem::path(dir) / filename;
    std::error_code ec;
    auto sz = std::filesystem::file_size(p, ec);
    if (ec) return nullptr;
    if (sz == 0 || sz > 16 * 1024 * 1024) return nullptr;
    void* buf = std::malloc(sz);
    if (!buf) return nullptr;
    std::ifstream f(p, std::ios::binary);
    if (!f) { std::free(buf); return nullptr; }
    f.read((char*)buf, (std::streamsize)sz);
    if (!f) { std::free(buf); return nullptr; }
    *outLen = (std::size_t)sz;
    return buf;
}

void* MockTsmEnvironment::gameObject() {
    void* p = exeBase_.data() + g_tsmAddr.game_object;
    trackRange(p, 0x1000);
    return p;
}

void* MockTsmEnvironment::allocateBuilding() {
    void* p = std::calloc(1, TSM_BLD_MIN_READABLE + 0x100);
    if (!p) return nullptr;
    buildings_.push_back(p);
    trackRange(p, TSM_BLD_MIN_READABLE + 0x100);
    // set type descriptor
    // allocate a descriptor
    void* td = std::calloc(1, TSM_TD_PHASES_END + 16);
    if (td) {
        int typeNum = 1;
        std::memcpy((char*)td + TSM_TD_TYPE_NUM, &typeNum, sizeof(int));
        void* tdptr = td;
        std::memcpy((char*)p + TSM_BLD_TYPEDESC, &tdptr, sizeof(void*));
        // track descriptor range
        trackRange(td, TSM_TD_PHASES_END + 16);
        // keep descriptor alive by storing in buildings_? Store as extra
        buildings_.push_back(td);
    }
    return p;
}

void* MockTsmEnvironment::allocatePerson() {
    void* p = std::calloc(1, TSM_PERSON_SIZE);
    if (!p) return nullptr;
    persons_.push_back(p);
    trackRange(p, TSM_PERSON_SIZE);
    return p;
}

void MockTsmEnvironment::stepTick() {
    clock_.tick++;
}

void MockTsmEnvironment::stepDay() {
    clock_.day++;
    if (clock_.day > 364) {
        clock_.day = 0;
        clock_.year++;
    }
    int m = clock_.day / clock_.daysPerMonth + 1;
    if (m < 1) m = 1;
    if (m > 12) m = 12;
    clock_.month = m;
    clock_.quarter = (clock_.month - 1) / 3 + 1;
}

void MockTsmEnvironment::stepDays(int n) {
    for (int i = 0; i < n; ++i) stepDay();
}

} // namespace mock
} // namespace wrsre

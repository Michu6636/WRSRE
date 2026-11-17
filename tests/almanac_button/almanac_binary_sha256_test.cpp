// Test-only stdin driver for the original streaming identity hash.
#include "../../prototypes/almanac_overlay/native/sha256.h"
#include <cstdio>
#include <cstdlib>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif
int main(int argc, char** argv) {
#ifdef _WIN32
    if (_setmode(_fileno(stdin), _O_BINARY) == -1) return 2;
#endif
    const unsigned long chunk = argc == 2 ? std::strtoul(argv[1], nullptr, 10) : 0;
    if (!chunk || chunk > 4096) return 2;
    almanac::BinarySha256 hash;
    unsigned char buffer[4096];
    for (;;) {
        const auto count = std::fread(buffer, 1, static_cast<std::size_t>(chunk), stdin);
        hash.Update(buffer, count);
        if (count < chunk) { if (std::ferror(stdin)) return 1; break; }
    }
    char hex[65]{};
    hash.Finish(hex);
    std::puts(hex);
}

#include "almanac_trampoline.h"
#include <cstdint>
#include <cstdio>
#include <cstring>

#define CHECK(expression) do { \
    if (!(expression)) { \
        std::fprintf(stderr, "CHECK failed at line %d: %s\n", __LINE__, #expression); \
        return 1; \
    } \
} while (0)

int main()
{
    constexpr std::uintptr_t trampoline = 0x000002D48FF20000ULL;
    constexpr std::uintptr_t resume = 0x00007FF6BDF27DC2ULL;
    constexpr std::uintptr_t observedBadTarget = 0x000002D4BDF27DC2ULL;

    unsigned char code[almanac_button::AbsoluteIndirectJumpSize]{};
    almanac_button::EncodeAbsoluteIndirectJump(code, resume);
    const unsigned char opcode[6] = {0xFF, 0x25, 0x00, 0x00, 0x00, 0x00};
    CHECK(std::memcmp(code, opcode, sizeof(opcode)) == 0);

    std::uint64_t encodedTarget = 0;
    std::memcpy(&encodedTarget, code + sizeof(opcode), sizeof(encodedTarget));
    CHECK(encodedTarget == resume);

    // Reproduce the old E9 rel32 truncation from the logged allocation. This
    // must land at the observed invalid RIP, not the intended resume address.
    const std::uintptr_t next = trampoline + 18 + 5;
    const std::uint32_t relBits = static_cast<std::uint32_t>(resume - next);
    const std::int64_t rel32 = (relBits & 0x80000000u)
        ? static_cast<std::int64_t>(relBits) - 0x100000000LL
        : static_cast<std::int64_t>(relBits);
    const std::uintptr_t oldTarget = static_cast<std::uintptr_t>(
        static_cast<std::int64_t>(next) + rel32);
    CHECK(oldTarget == observedBadTarget);
    CHECK(oldTarget != resume);

    std::puts("absolute trampoline jump avoids the logged rel32 crash target");
    return 0;
}

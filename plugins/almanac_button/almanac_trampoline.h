// Portable encoding helper for x64 absolute indirect trampoline jumps.
#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace almanac_button {
constexpr std::size_t AbsoluteIndirectJumpSize = 14;

inline void EncodeAbsoluteIndirectJump(unsigned char* output,
                                       std::uintptr_t destination)
{
    static_assert(sizeof(std::uintptr_t) == 8,
                  "Absolute indirect jumps require a 64-bit target");
    static constexpr unsigned char opcode[6] = {
        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00 // jmp qword ptr [rip+0]
    };
    const std::uint64_t target = static_cast<std::uint64_t>(destination);
    std::memcpy(output, opcode, sizeof(opcode));
    std::memcpy(output + sizeof(opcode), &target, sizeof(target));
}
} // namespace almanac_button

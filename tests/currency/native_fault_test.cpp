// Windows/MSVC-only: exercise real SEH and read-only native write failures.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include "../../src/currency_api.h"
#define PERSON_SPENT_OFFSET 0x734
static bool ReadablePtr(const void* p, size_t n) {
    MEMORY_BASIC_INFORMATION m{};
    if (!VirtualQuery(p, &m, sizeof(m))) return false;
    return m.State == MEM_COMMIT && !(m.Protect & (PAGE_NOACCESS | PAGE_GUARD)) &&
        (const BYTE*)p + n <= (const BYTE*)m.BaseAddress + m.RegionSize;
}
static bool ValidPerson(const void* p) { return ReadablePtr(p, 0x738); }
static LONG FaultFilter(const char*, EXCEPTION_POINTERS*) { return EXCEPTION_EXECUTE_HANDLER; }
#define Logf(...) ((void)0)
#include "../../plugins/currency/transaction_native.h"

int main() {
    BYTE* person = (BYTE*)VirtualAlloc(nullptr, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    double* treasury = (double*)VirtualAlloc(nullptr, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    assert(person && treasury);
    *(uint32_t*)(person + 8) = 1;
    float* spent = (float*)(person + PERSON_SPENT_OFFSET);
    *spent = 7; *treasury = 100;
    assert(ReadPersonId(person) != 0 && ReadTreasuryNative(treasury) == 100);
    DWORD oldProtect = 0;
    assert(VirtualProtect(treasury, 4096, PAGE_READONLY, &oldProtect));
    // Spent writes first, treasury faults second: native rollback restores spent.
    assert(!CommitNative(person, 3, treasury, 3));
    assert(*spent == 7 && *treasury == 100);
    assert(VirtualProtect(treasury, 4096, PAGE_READWRITE, &oldProtect));
    assert(CommitNative(person, 3, treasury, 3));
    assert(*spent == 10 && *treasury == 103);
    assert(VirtualProtect(person, 4096, PAGE_READONLY, &oldProtect));
    // First write faults: treasury must not be touched.
    assert(!CommitNative(person, 3, treasury, 3));
    assert(*spent == 10 && *treasury == 103);
    assert(VirtualProtect(person, 4096, PAGE_NOACCESS, &oldProtect));
    assert(ReadPersonId(person) == 0);
    assert(!CommitNative(person, 3, treasury, 3));
    assert(*treasury == 103);
    VirtualFree(person, 0, MEM_RELEASE);
    VirtualFree(treasury, 0, MEM_RELEASE);
    std::puts("currency native SEH tests: PASS");
}

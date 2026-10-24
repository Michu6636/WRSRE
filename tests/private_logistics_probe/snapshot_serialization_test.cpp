// Portable test of the production payload and serializer, not a replica.
#include "../../plugins/private_logistics_probe/snapshot_record.h"
#include <cassert>
#include <cstdarg>
#include <type_traits>

static void PrintLine(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    std::vprintf(format, args);
    va_end(args);
    std::putchar('\n');
}

static ProbeSnapshotPayload Produce()
{
    ProbeSnapshotPayload p{};
    char localCargo[128] = "fuel=9.876t ore=2.345t goods=1.234t";
    p.seq = 71; p.ts = 123.4567; p.thread = 1234;
    p.day = 42; p.month = 7; p.year = 2026; p.tod = 0.8333f;
    ProbeCopyText(p.record_type, sizeof(p.record_type), "measurement");
    ProbeCopyText(p.fail_reason, sizeof(p.fail_reason), "");
    ProbeCopyText(p.cargo_list, sizeof(p.cargo_list), localCargo);
    p.snapshot_ok = 1;
    p.vehicle = reinterpret_cast<const void*>(std::uintptr_t(0x1f2a3b4c5d6));
    p.do_bld = reinterpret_cast<const void*>(std::uintptr_t(0x1f2a3b4c7e8));
    p.fuel_slot_valid = 1; p.fuel_tons = 9.876543;
    p.mirror_valid = 0; p.mirror_tons = -1.0;
    p.cargo_valid = 1; p.cargo_slots = 3; p.cargo_total_tons = 4.567890;
    p.kind_valid = 1; p.kind = 66;
    p.state_valid = 1; p.state = 17;
    p.mode_valid = 1; p.mode = 9;
    p.home_matches_valid = 1; p.home_matches = 1;
    std::memset(localCargo, 'X', sizeof(localCargo));
    return p;
}

int main()
{
    static_assert(std::is_array<decltype(ProbeSnapshotPayload::cargo_list)>::value,
                  "Queue payload must own cargo text");
    ProbeSnapshotPayload queue[1]{};
    {
        auto producer = Produce();
        queue[0] = producer; // The same value-copy operation as QueuePush.
        std::memset(&producer, 0, sizeof(producer));
    } // Consumer runs only AFTER producer stack state has gone away.
    auto consumer = queue[0];
    std::memset(queue, 0, sizeof(queue));
    assert(std::strcmp(consumer.cargo_list, "fuel=9.876t ore=2.345t goods=1.234t") == 0);
    assert(consumer.state == 17 && consumer.mode == 9 && consumer.cargo_slots == 3);
    ProbeSerializeSnapshot(consumer, 1001, PrintLine);

    char bounded[4];
    ProbeCopyText(bounded, sizeof(bounded), "0123456789");
    assert(std::strcmp(bounded, "012") == 0);
    assert((16u & 0xffffffeeu) == 0u);
    return 0;
}

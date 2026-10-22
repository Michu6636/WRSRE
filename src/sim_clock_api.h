// sim_clock_api.h - the calendar and tick dispatch contract.
//
// sim_clock is a Level 0 service: it depends on nothing, and it exists to
// give every other plugin a single, consistent answer to three questions.
//
//   1. What in-game date is it right now?
//   2. Has a simulation frame just run that I should react to?
//   3. Has the calendar crossed a day, week, month, quarter or year?
//
// Everything that used to do this by hand - hooking C3D_TIMER::PowerTime,
// reading game+0x590/594/59C, throttling with GetTickCount(), re-deriving
// which PowerTime call is the frame's own and which is a real-time or
// ignore-pause call - now consumes one service. There is exactly one
// PowerTime hook in the process and it lives here.
//
// The service provides:
//
//   registerTick        once per simulation frame, with dt and dayDelta
//   registerDay         once per in-game calendar day
//   registerPause       on the pause/unpause transition
//   registerScheduled   by the shared TsmScheduleFrequency calendar
//
//   getDay/Month/Year   the current date, safe to call from anywhere
//   getTimeOfDay        the game's own day fraction, 0..getDayLength()
//   getDayFraction      the same as 0..1
//   getDayLength        what the 60 is - the unit getTimeOfDay is measured in
//   isPaused            whether the simulation is currently paused
//
//   parseFrequency      "monthly"/"weekly"/... -> TsmScheduleFrequency
//
// A plugin that only wants the date (for a HUD, say) can consume the
// service and call getDay() without registering anything. A plugin that
// wants to *do* something on a tick must register.

#ifndef TSM_SIM_CLOCK_API_H
#define TSM_SIM_CLOCK_API_H

#ifdef __cplusplus
extern "C" {
#endif

// The service name and version.
//
// v1u is unchanged in layout through the getDaysPerMonth addition: the
// new field is appended after every field a v1 consumer was compiled to
// read, and the structSize the consumer was already receiving tells it
// whether the field is present. This is the same append-only pattern
// TsmHost uses for vfsRoot, and it is what lets a plugin built against
// the original header keep loading against this one. A consumer MUST
// check `structSize >= offsetof(TsmSimClockApi, getDaysPerMonth) +
// sizeof(g_simClock->getDaysPerMonth)` before calling the accessor - see
// the field's own comment.
#define TSM_SERVICE_SIM_CLOCK "sim_clock"
#define TSM_SIM_CLOCK_VERSION 1u

// Calendar frequencies. This enum is the *only* place a "monthly" or
// "quarterly" is defined, so a plugin's own .ini can say
// `schedule = monthly` and every consumer in the process agrees on what
// that means.
//
// The frequency is purely a calendar concept - it says nothing about what
// a plugin does with it. Tax settlement, wage payout, a fleet of trucks
// being reassigned: they all read the same enum.
//
//   INSTANT     no scheduling; the plugin handles the event inline
//   DAILY       every in-game day
//   WEEKLY      every 7 in-game days
//   MONTHLY     on the in-game month boundary
//   QUARTERLY   on the in-game quarter boundary
//   YEARLY      on the in-game year boundary
//
// "Month" is not a game concept - the calendar is a day of the year
// (0..364) and an absolute year. sim_clock's own `days_per_month` setting
// defines month boundaries, and every plugin that asks for a month gets
// the same answer.
typedef enum TsmScheduleFrequency {
    TSM_FREQ_INSTANT   = 0,
    TSM_FREQ_DAILY     = 1,
    TSM_FREQ_WEEKLY    = 2,
    TSM_FREQ_MONTHLY   = 3,
    TSM_FREQ_QUARTERLY = 4,
    TSM_FREQ_YEARLY    = 5
} TsmScheduleFrequency;

// Fired once per simulation frame.
//
// `dt`       is the scaled game time the day tick's own PowerTime call
//            returned. This is the same number every per-frame rate in
//            the game is integrated from. It is always strictly
//            positive: sim_clock does not dispatch while paused.
//
// `dayDelta` is `dt / dayLength`, i.e. the fraction of a calendar day
//            this frame represents. A plugin that integrates progress
//            over a fixed number of days accumulates this. A plugin
//            that does pure per-frame maintenance ignores it.
//
// Callbacks run on the simulation thread. They must not block. If a
// callback faults, sim_clock logs it and disables that subscriber for
// the rest of the session; every other subscriber and the game itself
// are unaffected.
typedef void (*TsmTickCallback)(void* userData, float dt, float dayDelta);

// Fired once per in-game calendar day, on the frame the day field
// changes.
//
// `day`   is 0..364, the game's own day-of-year.
// `month` is 1..12, computed from sim_clock's `days_per_month` setting.
// `year`  is the absolute in-game year, the one the corner of the screen
//         shows.
//
// Fires *after* the day tick has already advanced the calendar, so
// reading the game's own fields inside this callback gives the new day,
// not the old.
typedef void (*TsmDayCallback)(void* userData, int day, int month, int year);

// Fired on the pause/unpause transition.
//
// `isPaused` is 1 on the transition into pause and 0 on the transition
// out. Fires exactly once per transition - never on the first frame of a
// session, which is treated as the initial state. A subscriber that
// wants the current state at registration time asks `isPaused()`.
typedef void (*TsmPauseCallback)(void* userData, int isPaused);

typedef struct TsmSimClockApi {
    unsigned structSize;

    // --- subscriptions -----------------------------------------------------
    //
    // Each returns a non-zero handle on success, or 0 on failure (the
    // subscriber table is full, or the callback was null). Handles may be
    // ignored for the life of the process; a plugin that is loaded once
    // and never unloaded does not need to unregister.

    int (*registerTick)(TsmTickCallback cb, void* userData);
    int (*registerDay)(TsmDayCallback cb, void* userData);
    int (*registerPause)(TsmPauseCallback cb, void* userData);

    // Schedule a day callback by calendar frequency. TSM_FREQ_DAILY is
    // equivalent to registerDay. TSM_FREQ_INSTANT is not schedulable and
    // returns 0 - a plugin that wants instant handling does it inline.
    //
    // The callback receives the same arguments a registerDay callback
    // would, and fires on the boundary the frequency describes: the
    // first day of a new month for MONTHLY, the first day of a new
    // quarter for QUARTERLY, and so on.
    int (*registerScheduled)(TsmScheduleFrequency freq,
                             TsmDayCallback cb, void* userData);

    // Remove a subscription. Handles are one-based; 0 and out-of-range
    // are no-ops. A plugin normally has no reason to call this.
    void (*unregister)(int handle);

    // --- current state -----------------------------------------------------
    //
    // All safe to call from any thread. Return -1 (or 0.0) before the
    // first world has loaded.

    int   (*getDay)(void);         // day of year, 0..364, or -1
    int   (*getMonth)(void);       // 1..12, or -1
    int   (*getYear)(void);        // absolute year, or -1
    float (*getTimeOfDay)(void);   // 0..getDayLength(), or 0.0
    float (*getDayFraction)(void); // 0..1, i.e. getTimeOfDay()/getDayLength()
    int   (*isPaused)(void);       // 1 while paused, 0 while running

    // The number of scaled time units in one calendar day. This is the
    // game's own 60.0f, read at startup and exposed so nothing else has
    // to hard-code it. `dayDelta` in a tick callback is exactly
    // `dt / getDayLength()`.
    float (*getDayLength)(void);

    // --- parsing -----------------------------------------------------------
    //
    // Parse a config string into a frequency. Case-insensitive; "annual"
    // is accepted as a synonym for "yearly". Returns `fallback` on any
    // unrecognised value, so a caller can just pass its own default.
    TsmScheduleFrequency (*parseFrequency)(const char* str,
                                           TsmScheduleFrequency fallback);

    // --- v1, appended (check structSize before calling) --------------------
    //
    // The number of in-game days in one "month", as configured in
    // sim_clock.ini. This is the value ComputeMonth divides by, and it
    // is the multiplier a monthly payday needs: a monthly wage is
    // (daily_wage * days_per_month).
    //
    // A consumer MUST check `structSize >=
    // offsetof(TsmSimClockApi, getDaysPerMonth) + sizeof(...)` before
    // calling. Against a host that does not carry the field, the caller
    // is responsible for falling back to its own default - and that
    // default must be the same number sim_clock itself defaults to, or
    // the two disagree about the length of a month.
    //
    // This exists because the alternative - inferring the month length
    // from a month-boundary day delta - cannot work. At a normal
    // boundary the delta is one day, and months are not uniform lengths
    // in any case (with days_per_month = 30, month 12 has 35 days).
    int (*getDaysPerMonth)(void);

} TsmSimClockApi;

#ifdef __cplusplus
}
#endif

#endif // TSM_SIM_CLOCK_API_H
// sim_clock - the calendar and tick dispatch service.
//
// Every plugin in this project consumes this one for its calendar
// and its tick. It is the only plugin that hooks C3D_TIMER::PowerTime and
// the only one that reads the game's own day and year fields. If it does
// not load, nothing above Level 0 does.
//
// v1.1 updates:
//   - Rejects and clamps `days_per_month >= 365` to 30 at config load with
//     a clear warning, as values >= 365 prevent month rollover from ever
//     occurring and break monthly/quarterly scheduling across all dependent
//     plugins.

#include "../../src/tesmio_plugin.h"
#include "../../src/sim_clock_api.h"

// ---------------------------------------------------------------- sites

#define RVA_GAME_OBJECT    0x9D4F10
#define RVA_SIM_TIMER      0x9D4EE0
#define RVA_DAYTICK_POWER  0x334AA0
#define RVA_DAY_LENGTH     0x90AA78
#define VANILLA_DAY_LEN    60.0f

#define SYM_POWERTIME "?PowerTime@C3D_TIMER@@QEAAMM_N0@Z"

// ---------------------------------------------------------------- state

typedef float (__fastcall *t_PowerTime)(void* self, float v,
                                        bool ignorePause, bool realTime);
static t_PowerTime o_PowerTime;

static int    g_enabled      = 1;
static int    g_daysPerMonth = 30;
static int    g_probe        = 1;

static void*  g_simTimer          = nullptr;
static void*  g_dayTickReturnAddr = nullptr;
static float  g_dayLength         = VANILLA_DAY_LEN;

// ---------------------------------------------------------------- subscribers

enum { SUB_TICK = 0, SUB_DAY, SUB_PAUSE, SUB_SCHED };

struct Sub {
    int   kind;
    int   freq;         // SUB_SCHED only
    void* cb;
    void* userData;
    int   active;
};

#define MAX_SUBS 96
static Sub g_subs[MAX_SUBS];
static int g_subCount;

static int Register(int kind, void* cb, void* userData, int freq)
{
    if (!cb) return 0;
    if (g_subCount >= MAX_SUBS) {
        Logf("sim_clock  subscriber table full (%d) - refusing", MAX_SUBS);
        return 0;
    }
    int i = g_subCount++;
    g_subs[i].kind     = kind;
    g_subs[i].freq     = freq;
    g_subs[i].cb       = cb;
    g_subs[i].userData = userData;
    g_subs[i].active   = 1;
    return i + 1;
}

static void FireTick(float dt, float dayDelta)
{
    for (int i = 0; i < g_subCount; i++) {
        Sub* s = &g_subs[i];
        if (!s->active || s->kind != SUB_TICK) continue;
        __try { ((TsmTickCallback)s->cb)(s->userData, dt, dayDelta); }
        __except (FaultFilter("sim_clock tick subscriber",
                              GetExceptionInformation()))
        {
            Logf("sim_clock  a tick subscriber faulted; disabling it "
                 "for this session");
            s->active = 0;
        }
    }
}

static void FireDay(int day, int month, int year)
{
    for (int i = 0; i < g_subCount; i++) {
        Sub* s = &g_subs[i];
        if (!s->active || s->kind != SUB_DAY) continue;
        __try { ((TsmDayCallback)s->cb)(s->userData, day, month, year); }
        __except (FaultFilter("sim_clock day subscriber",
                              GetExceptionInformation()))
        {
            Logf("sim_clock  a day subscriber faulted; disabling it "
                 "for this session");
            s->active = 0;
        }
    }
}

static void FireScheduled(TsmScheduleFrequency freq,
                          int day, int month, int year)
{
    for (int i = 0; i < g_subCount; i++) {
        Sub* s = &g_subs[i];
        if (!s->active || s->kind != SUB_SCHED || s->freq != (int)freq)
            continue;
        __try { ((TsmDayCallback)s->cb)(s->userData, day, month, year); }
        __except (FaultFilter("sim_clock scheduled subscriber",
                              GetExceptionInformation()))
        {
            Logf("sim_clock  a scheduled subscriber faulted; disabling it "
                 "for this session");
            s->active = 0;
        }
    }
}

static void FirePause(int paused)
{
    for (int i = 0; i < g_subCount; i++) {
        Sub* s = &g_subs[i];
        if (!s->active || s->kind != SUB_PAUSE) continue;
        __try { ((TsmPauseCallback)s->cb)(s->userData, paused); }
        __except (FaultFilter("sim_clock pause subscriber",
                              GetExceptionInformation()))
        {
            Logf("sim_clock  a pause subscriber faulted; disabling it "
                 "for this session");
            s->active = 0;
        }
    }
}

// ---------------------------------------------------------------- calendar

static int   g_day        = -1;
static int   g_month      = -1;
static int   g_year       = -1;
static int   g_quarter    = -1;
static float g_timeOfDay  = 0.0f;
static int   g_dayCounter = 0;
static int   g_paused     = -1;

static int ComputeMonth(int dayOfYear)
{
    int m = dayOfYear / g_daysPerMonth + 1;
    if (m <  1) m =  1;
    if (m > 12) m = 12;
    return m;
}

// ---------------------------------------------------------------- the hook

static float __fastcall h_PowerTime(void* self, float v,
                                    bool ignorePause, bool realTime)
{
    float result = o_PowerTime(self, v, ignorePause, realTime);

    if (self != g_simTimer) return result;
    if (realTime || ignorePause) return result;

    if (_ReturnAddress() != g_dayTickReturnAddr) return result;

    bool running = (result > 0.0f);

    if (g_paused < 0) {
        g_paused = running ? 0 : 1;
    } else if ((int)(!running) != g_paused) {
        g_paused = !running;
        FirePause(g_paused);
        if (g_probe)
            Logf("sim_clock  %s", g_paused ? "paused" : "resumed");
    }

    if (!running) return result;

    BYTE* game = g_exeBase + RVA_GAME_OBJECT;
    if (!game || !ReadablePtr(game, 0x600)) return result;

    int   day  = *(int*)  (game + 0x590);
    int   year = *(int*)  (game + 0x594);
    float tod  = *(float*)(game + 0x59C);

    if (day < 0 || day > 365) return result;

    int month = ComputeMonth(day);
    int qtr   = (month - 1) / 3 + 1;

    if (g_day < 0) {
        g_day        = day;
        g_month      = month;
        g_year       = year;
        g_quarter    = qtr;
        g_timeOfDay  = tod;
        g_dayCounter = 0;
        FireTick(result, result / g_dayLength);
        return result;
    }

    bool newDay  = (day  != g_day);
    bool newYear = (year != g_year);

    bool isLoad = false;
    if (newDay) {
        int rawDelta     = day - g_day;
        int wrappedDelta = (rawDelta < 0) ? (rawDelta + 365) : rawDelta;
        if (wrappedDelta != 1) isLoad = true;
        if (newYear && !(g_day == 364 && day == 0)) isLoad = true;
    } else if (newYear) {
        isLoad = true;
    }

    if (isLoad) {
        if (g_probe)
            Logf("sim_clock  calendar reset: day %d->%d year %d->%d; "
                 "treating as a world load", g_day, day, g_year, year);
        g_day        = day;
        g_month      = month;
        g_year       = year;
        g_quarter    = qtr;
        g_timeOfDay  = tod;
        g_dayCounter = 0;
        FireTick(result, result / g_dayLength);
        return result;
    }

    int oldMonth = g_month;
    int oldQtr   = g_quarter;

    g_day       = day;
    g_month     = month;
    g_year      = year;
    g_quarter   = qtr;
    g_timeOfDay = tod;

    if (newDay) {
        g_dayCounter++;

        FireDay(day, month, year);

        if (g_dayCounter % 7 == 0)
            FireScheduled(TSM_FREQ_WEEKLY,    day, month, year);
        if (month != oldMonth)
            FireScheduled(TSM_FREQ_MONTHLY,   day, month, year);
        if (qtr != oldQtr)
            FireScheduled(TSM_FREQ_QUARTERLY, day, month, year);
        if (newYear)
            FireScheduled(TSM_FREQ_YEARLY,    day, month, year);

        if (g_probe && month != oldMonth)
            Logf("sim_clock  month boundary: day %d month %d year %d",
                 day, month, year);
    }

    FireTick(result, result / g_dayLength);
    return result;
}

// ---------------------------------------------------------------- the API

static int api_RegisterTick(TsmTickCallback cb, void* u)
{
    return Register(SUB_TICK, (void*)cb, u, 0);
}
static int api_RegisterDay(TsmDayCallback cb, void* u)
{
    return Register(SUB_DAY, (void*)cb, u, 0);
}
static int api_RegisterPause(TsmPauseCallback cb, void* u)
{
    return Register(SUB_PAUSE, (void*)cb, u, 0);
}
static int api_RegisterScheduled(TsmScheduleFrequency freq,
                                 TsmDayCallback cb, void* u)
{
    if (freq == TSM_FREQ_INSTANT) return 0;
    if (freq == TSM_FREQ_DAILY)   return Register(SUB_DAY, (void*)cb, u, 0);
    return Register(SUB_SCHED, (void*)cb, u, (int)freq);
}
static void api_Unregister(int handle)
{
    if (handle < 1 || handle > g_subCount) return;
    g_subs[handle - 1].active = 0;
}

static int   api_GetDay(void)         { return g_day; }
static int   api_GetMonth(void)       { return g_month; }
static int   api_GetYear(void)        { return g_year; }
static float api_GetTimeOfDay(void)   { return g_timeOfDay; }
static float api_GetDayFraction(void) { return g_timeOfDay / g_dayLength; }
static int   api_IsPaused(void)       { return g_paused < 0 ? 0 : g_paused; }
static float api_GetDayLength(void)   { return g_dayLength; }

static int api_GetDaysPerMonth(void)  { return g_daysPerMonth; }

static TsmScheduleFrequency api_ParseFrequency(const char* str,
                                               TsmScheduleFrequency fallback)
{
    if (!str || !str[0]) return fallback;
    if (_stricmp(str, "instant")   == 0) return TSM_FREQ_INSTANT;
    if (_stricmp(str, "daily")     == 0) return TSM_FREQ_DAILY;
    if (_stricmp(str, "weekly")    == 0) return TSM_FREQ_WEEKLY;
    if (_stricmp(str, "monthly")   == 0) return TSM_FREQ_MONTHLY;
    if (_stricmp(str, "quarterly") == 0) return TSM_FREQ_QUARTERLY;
    if (_stricmp(str, "yearly")    == 0) return TSM_FREQ_YEARLY;
    if (_stricmp(str, "annual")    == 0) return TSM_FREQ_YEARLY;
    return fallback;
}

static const TsmSimClockApi kApi = {
    sizeof(TsmSimClockApi),
    api_RegisterTick,
    api_RegisterDay,
    api_RegisterPause,
    api_RegisterScheduled,
    api_Unregister,
    api_GetDay,
    api_GetMonth,
    api_GetYear,
    api_GetTimeOfDay,
    api_GetDayFraction,
    api_IsPaused,
    api_GetDayLength,
    api_ParseFrequency,
    api_GetDaysPerMonth
};

// ---------------------------------------------------------------- setup

static bool VerifyDayTickCall()
{
    const BYTE* p = g_exeBase + RVA_DAYTICK_POWER;

    if (!ReadablePtr(p, 20)) {
        Logf("sim_clock  cannot read the day tick call site at rva 0x%X",
             RVA_DAYTICK_POWER);
        return false;
    }

    if (p[0] != 0x48 || p[1] != 0x8D || p[2] != 0x0D) {
        Logf("sim_clock  rva 0x%X is not a `lea rcx` - refusing",
             RVA_DAYTICK_POWER);
        return false;
    }

    const BYTE* timer = p + 7 + *(const int*)(p + 3);
    if (timer != g_exeBase + RVA_SIM_TIMER) {
        Logf("sim_clock  day tick's timer is +0x%llX, expected +0x%X - refusing",
             (unsigned long long)(timer - g_exeBase), RVA_SIM_TIMER);
        return false;
    }

    static const BYTE kFlags[] = { 0x45, 0x33, 0xC9, 0x45, 0x33, 0xC0 };
    if (memcmp(p + 7, kFlags, sizeof(kFlags)) != 0) {
        Logf("sim_clock  day tick PowerTime flags do not match - refusing");
        return false;
    }

    if (p[13] != 0xFF || p[14] != 0x15) {
        Logf("sim_clock  rva 0x%X is not a `call [rip+disp32]` - refusing",
             RVA_DAYTICK_POWER);
        return false;
    }

    void** slot = FindIatSlot(g_exe, DLL_ENGINE, SYM_POWERTIME);
    void** site = (void**)(p + 19 + *(const int*)(p + 15));
    if (!slot || site != slot) {
        Logf("sim_clock  the day tick does not call PowerTime through the import slot - refusing");
        return false;
    }

    return true;
}

static void ReadSettings()
{
    const char* ini = "plugins\\sim_clock.ini";
    char v[32];

    g_enabled = H->configInt(ini, "sim_clock", "enabled", 1);
    if (H->configString(ini, "sim_clock", "days_per_month",
                        v, sizeof(v), "30") && v[0])
        g_daysPerMonth = atoi(v);
    g_probe = H->configInt(ini, "sim_clock", "probe", 1);

    if (g_daysPerMonth < 1) {
        g_daysPerMonth = 1;
    } else if (g_daysPerMonth >= 365) {
        Logf("sim_clock  WARN  days_per_month = %d is invalid (disables monthly boundaries) - clamped to 30",
             g_daysPerMonth);
        g_daysPerMonth = 30;
    }
}

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host,
                                                   TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "sim_clock";
    info->version = "1.1";

    ReadSettings();
    if (!g_enabled) {
        Logf("sim_clock  disabled");
        return 1;
    }

    float dayLen = *(const float*)(g_exeBase + RVA_DAY_LENGTH);
    if (dayLen != VANILLA_DAY_LEN) {
        Logf("sim_clock  day length is %.4f, expected %.1f - refusing",
             dayLen, VANILLA_DAY_LEN);
        return 1;
    }
    g_dayLength = dayLen;

    H->provide(TSM_SERVICE_SIM_CLOCK, TSM_SIM_CLOCK_VERSION, &kApi);
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;

    if (!VerifyDayTickCall()) {
        Logf("sim_clock  the day tick's PowerTime call could not be verified - inactive");
        return 1;
    }

    g_simTimer          = g_exeBase + RVA_SIM_TIMER;
    g_dayTickReturnAddr = g_exeBase + RVA_DAYTICK_POWER + 19;

    if (!PatchIat(g_exe, DLL_ENGINE, SYM_POWERTIME, (void*)h_PowerTime,
                  (void**)&o_PowerTime, "C3D_TIMER::PowerTime"))
        return 1;

    Logf("sim_clock  live v1.1: days_per_month = %d, day length %.1f, dispatch at +0x%X",
         g_daysPerMonth, g_dayLength, RVA_DAYTICK_POWER + 19);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }
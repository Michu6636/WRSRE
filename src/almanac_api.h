// almanac_api.h - mod-owned Almanac open-request service.
//
// Level 2 service: published by the Almanac launcher plugin (transitional:
// the legacy almanac_button plugin still toggles its own state; migration to
// this service is a follow-up milestone). Consumers must handle a missing
// service gracefully.
//
// Threading: the implementation must serialize access (the launcher's live
// path uses interlocked state). Consumers may call requestOpen/isOpen/isPending
// from the render thread; ack methods are backend-only.
//
// A small versioned service published by the Almanac launcher plugin. The
// future real Almanac panel backend consumes it to acknowledge creation and
// close. Missing service or unsupported context must degrade gracefully to
// "no custom button / no open request".
//
// The launcher only EMITS an open request; it never pretends a panel exists.
// The panel is marked open only after the backend acknowledges creation.

#ifndef TSM_ALMANAC_API_H
#define TSM_ALMANAC_API_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TSM_SERVICE_ALMANAC "almanac"

#define TSM_ALMANAC_VERSION    1u
#define TSM_ALMANAC_VERSION_V1 1u

typedef struct TsmAlmanacApi {
    unsigned structSize;

    // Emit an open request (idempotent: no duplicate while pending/open).
    void (*requestOpen)(void);

    // State queries for the panel backend / UI.
    int  (*isOpen)(void);      // 1 if open
    int  (*isPending)(void);   // 1 if a request is pending

    // Acknowledgements from the panel backend (backend-only; consumers must
    // not call these).
    void (*acknowledgeOpened)(void);
    void (*acknowledgeFailed)(void);
    void (*confirmClosed)(void);
} TsmAlmanacApi;

// Guard: does this iface carry the v1 API?
static inline int TsmAlmanacHasApi(const TsmAlmanacApi* api)
{
    return api &&
        api->structSize >= offsetof(TsmAlmanacApi, confirmClosed) +
                              sizeof(api->confirmClosed) &&
        api->requestOpen && api->isOpen && api->isPending &&
        api->acknowledgeOpened && api->acknowledgeFailed && api->confirmClosed;
}

// WHEN THIS PLUGIN IS ABSENT (or the service is not published):
//   - Consumers must check H->consume(TSM_SERVICE_ALMANAC, TSM_ALMANAC_VERSION)
//     for NULL and, on NULL, take no action: no button is drawn, no open request
//     is emitted, and no panel state is touched. The UI is byte-identical to
//     vanilla. Consumers must also verify TsmAlmanacHasApi(api) before calling
//     any member.
//   - Unsupported context (no active widget pass / no sidebar host) likewise
//     degrades to no-op; never fabricate a panel or write native selection.

#ifdef __cplusplus
}
#endif

#endif // TSM_ALMANAC_API_H

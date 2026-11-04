#ifndef TSM_B2B_LOGISTICS_API_H
#define TSM_B2B_LOGISTICS_API_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TSM_SERVICE_B2B_LOGISTICS "b2b_logistics"
#define TSM_B2B_LOGISTICS_VERSION 1u

typedef struct TsmB2bLogisticsApi {
    size_t structSize;
    int (*isCounterpartyBillingActive)(void);
} TsmB2bLogisticsApi;

static inline int TsmB2bCounterpartyActive(const TsmB2bLogisticsApi* api)
{
    return api &&
           api->structSize >= sizeof(TsmB2bLogisticsApi) &&
           api->isCounterpartyBillingActive &&
           api->isCounterpartyBillingActive() != 0;
}

#ifdef __cplusplus
}
#endif

#endif

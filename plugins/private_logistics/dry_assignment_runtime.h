// Simulation-thread sink + file-thread snapshot. All shared data uses g_lock.
#ifndef PRIVATE_LOGISTICS_DRY_ASSIGNMENT_RUNTIME_H
#define PRIVATE_LOGISTICS_DRY_ASSIGNMENT_RUNTIME_H
#include "dry_assignment_codec.h"
#include "assignment_settlement.h"
#include <new>
namespace dry=private_logistics_dry;
static dry::Book g_dryBook;
struct DryToken {std::uint64_t epoch=0,id=0;uintptr_t root=0;unsigned row=dry::Limit;};
static DryToken g_dryTokens[64]; // Ephemeral only. Never encoded or restored.

struct AssignmentToken {
    std::uint64_t epoch=0,id=0,buyerKey=0,carrierKey=0;
    uintptr_t root=0,buyer=0,carrier=0;
};
static AssignmentToken g_assignmentTokens[64];
struct AssignmentReceipt { std::uint64_t epoch=0,id=0; uintptr_t root=0; };
static AssignmentReceipt g_assignmentReceipts[256];
static unsigned g_assignmentReceiptHead=0;
static LONG g_dryEpoch=0;
static bool g_dryReady=false,g_dryCapacityWarned=false;
static int g_dryPersistenceHandle=0;
static constexpr char DryCostFile[]="private_logistics_costs.bin";
static unsigned DryFlags(const fleet::a::Candidate& c,bool complete) {
    return (c.unknownStart || c.observationGap || c.roleUncertain || c.fuelUncertain ||
        c.receiverChanged || c.observedRise || !complete)?dry::Partial:0;
}
static int DryTokenIndex(const fleet::a::Candidate& c) {
    for(unsigned i=0;i<64;++i) if(g_dryTokens[i].root==c.root &&
       g_dryTokens[i].id==c.id && g_dryTokens[i].epoch==c.epoch) return (int)i;
    return -1;
}
static bool DryContext(const fleet::a::Candidate& c) {
    return g_dryReady && !g_billing && c.id && c.epoch==(std::uint64_t)g_dryEpoch &&
        c.epoch==(std::uint64_t)InterlockedCompareExchange(&g_panelEpoch,0,0);
}
static void DryCostSample(const fleet::a::Candidate& c,bool complete) {
    EnterCriticalSection(&g_lock);
    const int token=DryContext(c)?DryTokenIndex(c):-1;
    if(token>=0 && !g_dryBook.Update(g_dryTokens[token].row,c.sampledDecrease,c.sampledIncrease,
                                   c.sampledPairs,DryFlags(c,complete)))
        g_dryBook.rows[g_dryTokens[token].row].flags|=dry::Partial;
    LeaveCriticalSection(&g_lock);
}
static dry::Reason DryCloseReason(const char* reason) {
    if(!std::strcmp(reason,"observed_removed")) return dry::Reason::Removed;
    if(!std::strcmp(reason,"observed_office_only")) return dry::Reason::OfficeOnly;
    if(!std::strcmp(reason,"observed_targets_replaced")) return dry::Reason::Replaced;
    return dry::Reason::Structure;
}
static void DryCostLog(const dry::Record& r) {
    Logf("[DRY COST] record=%llu stage=%s close_reason=%u target_snapshot_available=%d "
         "target_key=%016llX asset=%s type=%u x=%.9g z=%.9g "
         "identity_scope=asset_type_position_snapshot payer_verified=0 "
         "sampled_decrease_native=%.9g sampled_increase_native=%.9g compared_pairs=%u partial=%d "
         "estimated_cost_available=%d would_owe_available=%d estimated_cost=%.9g currency=%d "
         "frozen_price_per_tonne=%.9g price_policy=freeze_at_observed_close units=native_tonnes_provisional "
         "actual_debt=not_created settlement=none billing=off",
         (unsigned long long)r.id,r.stage==dry::Stage::Closed?"closed":"interrupted",(unsigned)r.reason,
         r.target.valid?1:0,(unsigned long long)dry::TargetKey(r.target),r.target.valid?r.target.asset:"unresolved",
         r.target.type,double(r.target.x),double(r.target.z),r.decrease,r.increase,r.pairs,r.flags?1:0,
         r.available?1:0,(r.available && r.target.valid)?1:0,r.amount,r.currency,r.price);
}
static bool AssignmentPartyCurrent(uintptr_t building,std::uint64_t expectedKey)
{
    if(!building || !expectedKey || TsmIsDecayed((void*)building)) return false;
    std::uint64_t key=0;int type=0;
    return AutoBuildingKey((void*)building,&key,&type) && key==expectedKey;
}
static int AssignmentTokenIndex(const fleet::a::Candidate& c)
{
    for(unsigned i=0;i<64;++i) if(g_assignmentTokens[i].root==c.root &&
        g_assignmentTokens[i].id==c.id && g_assignmentTokens[i].epoch==c.epoch) return (int)i;
    return -1;
}
static bool AssignmentDuplicate(const fleet::a::Candidate& c)
{
    for(const auto& r:g_assignmentReceipts) if(r.root==c.root && r.id==c.id && r.epoch==c.epoch) return true;
    return false;
}
static void AssignmentRemember(const fleet::a::Candidate& c)
{
    auto& r=g_assignmentReceipts[g_assignmentReceiptHead++%256];
    r={c.epoch,c.id,c.root};
}
static void AssignmentSettlementEvent(const fleet::a::Event& e,
                                      const PanelEndpointPrice& quote,bool complete)
{
    if(!g_counterpartySettlement) return;
    const auto& c=e.candidate;
    const bool opening=e.kind==fleet::a::Kind::Opened || e.kind==fleet::a::Kind::Adopted;
    if(opening) {
        std::uint64_t buyerKey=0,carrierKey=0;int type=0;
        if(c.originalTargetRows==1 && c.originalTarget && c.home &&
           AutoBuildingKey((void*)c.originalTarget,&buyerKey,&type) &&
           AutoBuildingKey((void*)c.home,&carrierKey,&type)) {
            EnterCriticalSection(&g_lock);
            if(AssignmentTokenIndex(c)<0) for(auto& t:g_assignmentTokens) if(!t.root) {
                t={c.epoch,c.id,buyerKey,carrierKey,c.root,c.originalTarget,c.home};break;
            }
            LeaveCriticalSection(&g_lock);
        }
        return;
    }
    if(e.kind==fleet::a::Kind::Abandoned) {
        EnterCriticalSection(&g_lock);
        const int token=AssignmentTokenIndex(c);if(token>=0) g_assignmentTokens[token]=AssignmentToken{};
        LeaveCriticalSection(&g_lock);
        return;
    }
    if(e.kind!=fleet::a::Kind::Closed) return;

    auto assessment=private_logistics_assignment_settlement::Assess(e,complete,quote.available,quote.amount);
    AssignmentToken token{};
    EnterCriticalSection(&g_lock);
    const bool duplicate=AssignmentDuplicate(c);
    const int index=AssignmentTokenIndex(c);
    if(index>=0) {token=g_assignmentTokens[index];g_assignmentTokens[index]=AssignmentToken{};}
    LeaveCriticalSection(&g_lock);
    if(duplicate) return;
    if(!token.root && assessment.billable) {assessment.billable=false;assessment.reason="identity_missing";}
    if(assessment.billable && (!AssignmentPartyCurrent(token.buyer,token.buyerKey) ||
                               !AssignmentPartyCurrent(token.carrier,token.carrierKey))) {
        assessment.billable=false;assessment.reason="identity_changed_or_demolished";
    }
    // A site under construction cannot receive goods (the engine refuses the
    // unload), so a trip to it delivered nothing and is never charged.
    if(assessment.billable && !AutoBuildingFinished((void*)token.buyer)) {
        assessment.billable=false;assessment.reason="target_under_construction";
    }

    double paid=0.0;
    const bool dry=g_dryRun!=0 || g_billing==0;
    if(assessment.billable) {
        const bool buyerPriv=IsPrivate((void*)token.buyer);
        const bool carrierPriv=IsPrivate((void*)token.carrier);
        if(!buyerPriv || !carrierPriv) {
            assessment.billable=false;assessment.reason="private_parties_required";
        } else {
            unsigned insolvent=0,treasurySkip=0;
            auto cb=MakeTransferCallbacks(&insolvent,&treasurySkip);
            EnterCriticalSection(&g_lock);AssignmentRemember(c);LeaveCriticalSection(&g_lock);
            if(wrsre::b2b::TransferFunds(cb,(void*)token.buyer,wrsre::b2b::CAP_CAT_WHOLESALE,
                                         (void*)token.carrier,wrsre::b2b::CAP_CAT_WHOLESALE,
                                         quote.amount,"b2b_do_assignment_fuel")) paid=quote.amount;
            else {assessment.billable=false;assessment.reason="transfer_failed";}
            EnterCriticalSection(&g_lock);
            g_skipBuyerInsolvent+=insolvent;g_skipTreasury+=treasurySkip;
            if(paid>0.0) {++g_b2bSettleCount;g_b2bTripFuelSum+=paid;}
            LeaveCriticalSection(&g_lock);
            if(!dry && paid>0.0) {
                TrackCarrier((void*)token.carrier);NoteCarrierIncome((void*)token.carrier,paid);
            }
        }
    }
    char buyerName[64],carrierName[64];
    BuildingName((void*)token.buyer,buyerName,sizeof(buyerName));
    BuildingName((void*)token.carrier,carrierName,sizeof(carrierName));
    Logf("[DO TRIP] %s assignment=%llu buyer=\"%s\" carrier=\"%s\" "
         "fuel_decrease_native=%.9g fuel_price_global_close=%.9g charge=%.9g paid=%.9g "
         "eligible=%d reason=%s exact_burn=0 price_policy=global_native_price_at_observed_close",
         dry?"dry_run":"live",(unsigned long long)c.id,buyerName,carrierName,
         c.sampledDecrease,(double)quote.perTonne,quote.amount,paid,
         assessment.billable?1:0,assessment.reason);
}
static void DryCostEvent(const fleet::a::Event& e,const PanelEndpointPrice& quote,bool complete) {
    AssignmentSettlementEvent(e,quote,complete);
    const auto& c=e.candidate;
    const bool opening=e.kind==fleet::a::Kind::Opened || e.kind==fleet::a::Kind::Adopted;
    dry::Identity identity{};
    if(opening && c.epoch==(std::uint64_t)InterlockedCompareExchange(&g_panelEpoch,0,0) && c.originalTargetRows==1)
        DryIdentitySnapshot(c.originalTarget,identity); // Capture ONCE, never rebind on closure/demolition.
    EnterCriticalSection(&g_lock);
    if(!DryContext(c)) {LeaveCriticalSection(&g_lock);return;}
    int token=DryTokenIndex(c);
    if(opening) {
        if(token>=0) {LeaveCriticalSection(&g_lock);return;} // Idempotent event delivery.
        for(unsigned i=0;i<64;++i) if(!g_dryTokens[i].root) {token=(int)i;break;}
        const unsigned row=token>=0?g_dryBook.Begin(identity):dry::Limit;
        if(row==dry::Limit) {
            const bool warn=!g_dryCapacityWarned;g_dryCapacityWarned=true;
            LeaveCriticalSection(&g_lock);
            if(warn) Logf("[DRY COST] capacity_exhausted=1 prior_records_retained=1 no_eviction=1 billing=off");
            return;
        }
        g_dryTokens[token]={c.epoch,c.id,c.root,row};
    }
    if(token<0) {LeaveCriticalSection(&g_lock);return;}
    const unsigned row=g_dryTokens[token].row;
    if(!g_dryBook.Update(row,c.sampledDecrease,c.sampledIncrease,c.sampledPairs,DryFlags(c,complete)))
        g_dryBook.rows[row].flags|=dry::Partial;
    bool finished=false;
    if(e.kind==fleet::a::Kind::Closed)
        finished=g_dryBook.Close(row,{quote.available,double(quote.perTonne),quote.currency},DryCloseReason(e.reason));
    else if(e.kind==fleet::a::Kind::Abandoned) finished=g_dryBook.Interrupt(row);
    const auto record=g_dryBook.rows[row];
    if(finished) g_dryTokens[token]=DryToken{};
    LeaveCriticalSection(&g_lock);
    if(finished) DryCostLog(record);
}
static void DryCostPersistence(TsmSaveEvent event,const char* dir,void*) {
    if(!dir) return;
    if(event==TSM_SAVE_WRITING) {
        auto* data=(unsigned char*)std::malloc(dry::MaxBytes);
        if(!data) {Logf("[DRY COST] save_allocation_failed=1");return;}
        EnterCriticalSection(&g_lock);
        const auto size=g_dryReady?dry::Encode(g_dryBook,data,dry::MaxBytes):0;
        const auto count=g_dryBook.count;
        LeaveCriticalSection(&g_lock);
        if(size) {
            const int ok=g_persistence->writeAtomic(dir,DryCostFile,data,size);
            Logf("[DRY COST] saved=%d records=%u bytes=%llu billing=off",ok,count,(unsigned long long)size);
        } else Logf("[DRY COST] save_skipped_invalid_or_blocked=1 existing_file_retained=1");
        std::free(data);return;
    }
    if(event!=TSM_SAVE_LOADING) return;
    const LONG epoch=InterlockedCompareExchange(&g_panelEpoch,0,0);
    EnterCriticalSection(&g_lock);
    g_dryReady=false;g_dryEpoch=epoch;g_dryBook.count=0;g_dryBook.next=1;
    for(auto& t:g_dryTokens) t=DryToken{};
    for(auto& t:g_assignmentTokens) t=AssignmentToken{};
    for(auto& r:g_assignmentReceipts) r=AssignmentReceipt{};
    g_assignmentReceiptHead=0;
    g_dryCapacityWarned=false;
    LeaveCriticalSection(&g_lock);
    const int state=DryFileState(dir,DryCostFile);
    auto* loaded=new(std::nothrow) dry::Book;
    bool ok=loaded && state==0;
    if(loaded && state==1) {
        size_t size=0;void* data=g_persistence->readFile(dir,DryCostFile,&size);
        ok=dry::Decode((const unsigned char*)data,size,*loaded);
        std::free(data);
    }
    unsigned count=0,interrupted=0;
    EnterCriticalSection(&g_lock);
    const bool current=g_dryEpoch==epoch && epoch==InterlockedCompareExchange(&g_panelEpoch,0,0);
    if(ok && current) {
        g_dryBook=*loaded;g_dryReady=true;count=g_dryBook.count;
        for(unsigned i=0;i<count;++i) if(g_dryBook.rows[i].stage==dry::Stage::Interrupted) ++interrupted;
    }
    LeaveCriticalSection(&g_lock);
    delete loaded;
    Logf("[DRY COST] loaded=%d records=%u interrupted_unpriced=%u "
         "missing_file=%d blocked=%d prior_active_not_rebound=1 billing=off",
         (ok && current)?1:0,count,interrupted,state==0?1:0,(!ok || !current)?1:0);
}
static void DryCostStart() {
    if(!g_fleetStarted || !g_persistence ||
       g_persistence->structSize<offsetof(TsmPersistenceApi,readFile)+sizeof(g_persistence->readFile) ||
       !g_persistence->registerHandler || !g_persistence->readFile || !g_persistence->writeAtomic) return;
    EnterCriticalSection(&g_lock);
    g_dryEpoch=InterlockedCompareExchange(&g_panelEpoch,0,0);g_dryReady=true;
    LeaveCriticalSection(&g_lock);
    g_dryPersistenceHandle=g_persistence->registerHandler("private_logistics_costs",DryCostPersistence,nullptr);
    if(!g_dryPersistenceHandle) {
        EnterCriticalSection(&g_lock);g_dryReady=false;LeaveCriticalSection(&g_lock);
        Logf("[DRY COST] registration_failed=1 billing=off");return;
    }
    g_fleetCostEvent=DryCostEvent;g_fleetCostSample=DryCostSample;
    Logf("[DRY COST] started file=%s max_records=4096 closed_quotes_immutable=1 "
         "open_save_policy=interrupted_unpriced no_money_writes=1 billing=off",DryCostFile);
}
#endif

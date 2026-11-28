// Simulation-thread sink + file-thread snapshot. All shared data uses g_lock.
#ifndef PRIVATE_LOGISTICS_DRY_ASSIGNMENT_RUNTIME_H
#define PRIVATE_LOGISTICS_DRY_ASSIGNMENT_RUNTIME_H
#include "dry_assignment_codec.h"
#include "assignment_settlement.h"
#include "do_fuel_settlement.h"
#include <cstdio>
#include <limits>
#include <new>
namespace dry=private_logistics_dry;
static dry::Book g_dryBook;
struct DryToken {std::uint64_t epoch=0,id=0;uintptr_t root=0;unsigned row=dry::Limit;};
static DryToken g_dryTokens[64]; // Ephemeral only. Never encoded or restored.

struct AssignmentToken {
    std::uint64_t generation=0,epoch=0,id=0,homeKey=0,receiverKey=0,openSequence=0;
    uintptr_t root=0,vehicle=0,home=0,receiver=0;
    private_logistics_step3b::ReceiverKind receiverKind=
        private_logistics_step3b::ReceiverKind::Unknown;
    bool directionUnknown=false;
};
static AssignmentToken g_assignmentTokens[64];
enum class AssignmentReceiptState : unsigned char { Empty, InProgress, Committed, Failed };
struct AssignmentReceipt {
    std::uint64_t generation=0,epoch=0,id=0;
    uintptr_t root=0;
    AssignmentReceiptState state=AssignmentReceiptState::Empty;
};
static AssignmentReceipt g_assignmentReceipts[256];
static unsigned g_assignmentReceiptCount=0;
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
         "actual_debt=not_created settlement=none money_moved=0",
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
static int AssignmentTokenIndex(const fleet::a::Candidate& c,std::uint64_t generation)
{
    for(unsigned i=0;i<64;++i) if(g_assignmentTokens[i].root==c.root &&
        g_assignmentTokens[i].id==c.id && g_assignmentTokens[i].epoch==c.epoch &&
        g_assignmentTokens[i].generation==generation) return (int)i;
    return -1;
}
static int AssignmentReceiptIndex(std::uint64_t generation,
                                  const fleet::a::Candidate& c)
{
    for(unsigned i=0;i<g_assignmentReceiptCount;++i) {
        const auto& r=g_assignmentReceipts[i];
        if(r.generation==generation && r.root==c.root &&
           r.id==c.id && r.epoch==c.epoch) return (int)i;
    }
    return -1;
}
static void AssignmentMarkUnknownByHomeLocked(std::uint64_t generation,
                                               uintptr_t home)
{
    for(auto& token:g_assignmentTokens) if(token.root &&
        token.generation==generation && (!home || token.home==home))
        token.directionUnknown=true;
}

static bool AssignmentBeginTransferSequence(std::uint64_t* generation,
                                            std::uint64_t* sequence)
{
    if(!generation || !sequence) return false;
    EnterCriticalSection(&g_lock);
    bool valid=!g_receiverProofDisabled &&
        g_transferSequence<(std::numeric_limits<std::uint64_t>::max)();
    if(valid) {
        ++g_transferSequence;
    } else {
        g_receiverProofDisabled=true;
        AssignmentMarkUnknownByHomeLocked(g_assignmentGeneration,0);
    }
    *generation=g_assignmentGeneration;
    *sequence=g_transferSequence;
    LeaveCriticalSection(&g_lock);
    return valid;
}

static void AssignmentSaveLoadingLocked()
{
    if(g_assignmentGeneration<(std::numeric_limits<std::uint64_t>::max)()) {
        ++g_assignmentGeneration;
        g_transferSequence=0;
    } else {
        g_receiverProofDisabled=true;
        AssignmentMarkUnknownByHomeLocked(g_assignmentGeneration,0);
    }
    for(auto& token:g_assignmentTokens) token=AssignmentToken{};
}

static void AssignmentInvalidatePendingReceiverLocked(
    const wrsre::b2b::PendingDelivery& pending)
{
    if(!pending.active) return;
    AssignmentMarkUnknownByHomeLocked(pending.generation,pending.fleetOwner);
}

static void AssignmentObserveMode0FailureLocked(std::uint64_t generation,
                                                 uintptr_t home)
{
    AssignmentMarkUnknownByHomeLocked(generation,home);
}

static void AssignmentObserveVehicleDeliveryLocked(
    std::uint64_t generation,std::uint64_t sequence,uintptr_t vehicle,
    uintptr_t home,const wrsre::b2b::MatchedDelivery& delivery,
    std::uint64_t receiverKey,bool receiverKeyValid,
    private_logistics_step3b::ReceiverKind receiverKind)
{
    for(auto& token:g_assignmentTokens) {
        if(!token.root || token.generation!=generation ||
           token.vehicle!=vehicle || token.home!=home) continue;
        if(g_receiverProofDisabled || token.directionUnknown ||
           sequence<=token.openSequence || !delivery.deliveryMatched ||
           delivery.deliveryAmbiguous || !delivery.receiverEvidence ||
           !delivery.destBld || !receiverKeyValid) {
            token.directionUnknown=true;
            continue;
        }
        if(!token.receiver) {
            token.receiver=(uintptr_t)delivery.destBld;
            token.receiverKey=receiverKey;
            token.receiverKind=receiverKind;
        } else if(token.receiver!=(uintptr_t)delivery.destBld ||
                  token.receiverKey!=receiverKey ||
                  token.receiverKind!=receiverKind) {
            token.directionUnknown=true;
        }
    }
}

struct AssignmentTransferContext {
    std::uint64_t generation=0;
    int receipt=-1;
    private_logistics_step3b::PayerKind payer=
        private_logistics_step3b::PayerKind::None;
    int currency=0;
};

static double AssignmentTransferTreasuryBalance(void* userData,int)
{
    const auto* context=(const AssignmentTransferContext*)userData;
    return context && TreasuryReadable()
        ? g_currency->getTreasuryBalance(context->currency)
        : std::numeric_limits<double>::quiet_NaN();
}

static void AssignmentTransferTreasuryMove(void* userData,double amount,
                                            const char* label)
{
    const auto* context=(const AssignmentTransferContext*)userData;
    if(context && TreasuryWritable())
        g_currency->moveTreasury(amount,label);
}

static bool AssignmentTransferPartyCurrent(
    void*,std::uintptr_t building,std::uint64_t key,
    private_logistics_step3b::ReceiverKind expected)
{
    if(!AssignmentPartyCurrent(building,key) ||
       !AutoBuildingFinished((void*)building)) return false;
    switch(expected) {
    case private_logistics_step3b::ReceiverKind::Private:
        return IsPrivate((void*)building);
    case private_logistics_step3b::ReceiverKind::State:
        return !IsPrivate((void*)building) && !IsCustomsBuilding((void*)building);
    case private_logistics_step3b::ReceiverKind::Customs:
        return IsCustomsBuilding((void*)building);
    case private_logistics_step3b::ReceiverKind::Unknown:
        return false;
    }
    return false;
}

static bool AssignmentTransferGenerationCurrent(void* userData)
{
    const auto* context=(const AssignmentTransferContext*)userData;
    EnterCriticalSection(&g_lock);
    const bool current=context && !g_receiverProofDisabled &&
        context->receipt>=0 &&
        (unsigned)context->receipt<g_assignmentReceiptCount &&
        g_assignmentGeneration==context->generation &&
        g_assignmentReceipts[context->receipt].generation==context->generation &&
        g_assignmentReceipts[context->receipt].state==AssignmentReceiptState::InProgress;
    LeaveCriticalSection(&g_lock);
    return current;
}

static bool AssignmentTransferCommit(void* userData,
    private_logistics_step3b::PayerKind payer,double amount)
{
    const auto* context=(const AssignmentTransferContext*)userData;
    if(!context || payer!=context->payer ||
       !std::isfinite(amount) || amount<=0.0) return false;
    EnterCriticalSection(&g_lock);
    const bool current=!g_receiverProofDisabled &&
        context->receipt>=0 &&
        (unsigned)context->receipt<g_assignmentReceiptCount &&
        g_assignmentGeneration==context->generation &&
        g_assignmentReceipts[context->receipt].generation==context->generation &&
        g_assignmentReceipts[context->receipt].state==AssignmentReceiptState::InProgress;
    if(current) {
        g_assignmentReceipts[context->receipt].state=AssignmentReceiptState::Committed;
        ++g_b2bSettleCount;
        g_b2bTripFuelSum+=amount;
        if(payer==private_logistics_step3b::PayerKind::PrivateCompany)
            g_doFuelPrivateSum+=amount;
        else if(payer==private_logistics_step3b::PayerKind::Treasury)
            g_doFuelStateSum+=amount;
    }
    LeaveCriticalSection(&g_lock);
    return current;
}

static void AssignmentFinishReceipt(int receipt,std::uint64_t generation,
                                     AssignmentReceiptState state)
{
    EnterCriticalSection(&g_lock);
    if(receipt>=0 && (unsigned)receipt<g_assignmentReceiptCount &&
       g_assignmentReceipts[receipt].generation==generation &&
       g_assignmentReceipts[receipt].state==AssignmentReceiptState::InProgress)
        g_assignmentReceipts[receipt].state=state;
    LeaveCriticalSection(&g_lock);
}

static void AssignmentSettlementEvent(const fleet::a::Event& e,
                                      const PanelEndpointPrice& quote,bool complete)
{
    if(!g_counterpartySettlement) return;
    const auto& c=e.candidate;
    const bool opening=e.kind==fleet::a::Kind::Opened || e.kind==fleet::a::Kind::Adopted;
    if(opening) {
        std::uint64_t homeKey=0;int type=0;
        if(c.root && c.home && c.epoch==(std::uint64_t)InterlockedCompareExchange(&g_panelEpoch,0,0) &&
           IsPrivate((void*)c.home) && AutoBuildingKey((void*)c.home,&homeKey,&type)) {
            EnterCriticalSection(&g_lock);
            if(AssignmentTokenIndex(c,g_assignmentGeneration)<0 &&
               !g_receiverProofDisabled) {
                for(auto& token:g_assignmentTokens) if(!token.root) {
                    token.generation=g_assignmentGeneration;
                    token.epoch=c.epoch;token.id=c.id;token.root=c.root;
                    token.vehicle=c.root;token.home=c.home;token.homeKey=homeKey;
                    token.openSequence=g_transferSequence;
                    break;
                }
            }
            LeaveCriticalSection(&g_lock);
        }
        return;
    }
    if(e.kind==fleet::a::Kind::Abandoned) {
        EnterCriticalSection(&g_lock);
        const int token=AssignmentTokenIndex(c,g_assignmentGeneration);
        if(token>=0) g_assignmentTokens[token]=AssignmentToken{};
        LeaveCriticalSection(&g_lock);
        return;
    }
    if(e.kind!=fleet::a::Kind::Closed) return;

    const auto assessment=private_logistics_assignment_settlement::Assess(
        e,complete,quote.available,quote.amount);
    AssignmentToken token{};
    std::uint64_t generation=0;
    int receipt=-1;
    bool duplicate=false,receiptFull=false;
    EnterCriticalSection(&g_lock);
    generation=g_assignmentGeneration;
    const int tokenIndex=(c.epoch==(std::uint64_t)InterlockedCompareExchange(&g_panelEpoch,0,0))
        ? AssignmentTokenIndex(c,generation) : -1;
    if(tokenIndex>=0) {
        token=g_assignmentTokens[tokenIndex];
        g_assignmentTokens[tokenIndex]=AssignmentToken{};
    }
    duplicate=AssignmentReceiptIndex(generation,c)>=0;
    LeaveCriticalSection(&g_lock);
    if(duplicate) return;

    const bool trackedToken=token.root && token.generation==generation &&
        token.epoch==c.epoch && token.id==c.id && token.root==c.root &&
        token.vehicle==c.root && token.home==(uintptr_t)c.home;
    const bool privateOffice=trackedToken ||
        (c.home && !TsmIsDecayed((void*)c.home) && IsPrivate((void*)c.home));
    if(!privateOffice) return;

    EnterCriticalSection(&g_lock);
    if(g_assignmentGeneration!=generation) {
        duplicate=true;
    } else if(AssignmentReceiptIndex(generation,c)>=0) {
        duplicate=true;
    } else if(trackedToken) {
        if(g_assignmentReceiptCount>=256) {
            receiptFull=true;
        } else {
            receipt=(int)g_assignmentReceiptCount++;
            g_assignmentReceipts[receipt]={generation,c.epoch,c.id,c.root,
                                            AssignmentReceiptState::InProgress};
        }
    }
    LeaveCriticalSection(&g_lock);
    if(duplicate) return;

    private_logistics_step3b::ReceiverKind receiverKind=
        private_logistics_step3b::ReceiverKind::Unknown;
    bool receiverKnown=trackedToken && !token.directionUnknown && token.receiver &&
        AssignmentPartyCurrent(token.receiver,token.receiverKey);
    const char* reason=assessment.reason;
    if(receiverKnown) {
        if(IsCustomsBuilding((void*)token.receiver))
            receiverKind=private_logistics_step3b::ReceiverKind::Customs;
        else if(IsPrivate((void*)token.receiver))
            receiverKind=private_logistics_step3b::ReceiverKind::Private;
        else
            receiverKind=private_logistics_step3b::ReceiverKind::State;
        receiverKnown=receiverKind==token.receiverKind;
    }
    if(!receiverKnown) reason="direction_unknown";
    else if(!AutoBuildingFinished((void*)token.receiver)) reason="receiver_unfinished";
    else if(receiverKind==private_logistics_step3b::ReceiverKind::Customs)
        reason="customs_export_deferred";
    else if(!trackedToken || !AssignmentPartyCurrent(token.home,token.homeKey) ||
            !IsPrivate((void*)token.home))
        reason="identity_changed_or_demolished";

    const auto decision=private_logistics_step3b::ChoosePayer(receiverKnown
        ? receiverKind : private_logistics_step3b::ReceiverKind::Unknown);
    const bool billable=assessment.billable && receiverKnown &&
        AutoBuildingFinished((void*)token.receiver) &&
        AssignmentPartyCurrent(token.home,token.homeKey) && IsPrivate((void*)token.home) &&
        decision.payer!=private_logistics_step3b::PayerKind::None &&
        !receiptFull && receipt>=0;
    double wouldPay=billable?quote.amount:0.0;
    double paid=0.0;
    bool attemptedLive=false;
    private_logistics_step3b::DoFuelResult transfer{};
    if(!receiptFull && receipt>=0 && billable) {
        if(g_billing && !g_dryRun) {
            attemptedLive=true;
            unsigned insolvent=0,treasurySkip=0;
            auto funds=MakeTransferCallbacks(&insolvent,&treasurySkip);
            AssignmentTransferContext context{
                generation,receipt,decision.payer,quote.currency};
            private_logistics_step3b::DoFuelCallbacks callbacks{};
            callbacks.userData=&context;
            callbacks.isLivePrivate=EnterprisePrivateReady()?funds.isLivePrivate:nullptr;
            callbacks.getCapital=CompanyCapitalReadable()?funds.getCapital:nullptr;
            callbacks.modifyCapital=CompanyCapitalWritable()?funds.modifyCapital:nullptr;
            callbacks.getTreasury=TreasuryReadable()
                ? AssignmentTransferTreasuryBalance : nullptr;
            callbacks.moveTreasury=TreasuryWritable()
                ? AssignmentTransferTreasuryMove : nullptr;
            callbacks.partyCurrent=&AssignmentTransferPartyCurrent;
            callbacks.generationCurrent=&AssignmentTransferGenerationCurrent;
            callbacks.commit=&AssignmentTransferCommit;
            callbacks.allowBuyerOverdraft=funds.allowBuyerOverdraft;
            callbacks.buyerOverdraftLimit=funds.buyerOverdraftLimit;
            callbacks.minPayment=g_minPayment;
            callbacks.payerCategory=wrsre::b2b::CAP_CAT_WHOLESALE;
            callbacks.payeeCategory=wrsre::b2b::CAP_CAT_WHOLESALE;
            callbacks.skipBuyerInsolvent=&insolvent;
            callbacks.skipTreasury=&treasurySkip;
            transfer=private_logistics_step3b::ExecuteDoFuelTransfer(
                callbacks,decision,token.receiver,token.receiverKey,token.home,
                token.homeKey,quote.amount,true);
            paid=transfer.paid?quote.amount:0.0;
            EnterCriticalSection(&g_lock);
            g_skipBuyerInsolvent+=insolvent;g_skipTreasury+=treasurySkip;
            LeaveCriticalSection(&g_lock);
            if(!transfer.paid) {
                reason=transfer.reason;
                AssignmentFinishReceipt(receipt,generation,AssignmentReceiptState::Failed);
            } else {
                reason=transfer.reason;
                TrackCarrier((void*)token.home);NoteCarrierIncome((void*)token.home,paid);
            }
        } else {
            reason=decision.reason;
            AssignmentFinishReceipt(receipt,generation,AssignmentReceiptState::Committed);
        }
    } else if(receipt>=0) {
        if(receiptFull) reason="receipt_capacity";
        else if(!receiverKnown) reason="direction_unknown";
        AssignmentFinishReceipt(receipt,generation,AssignmentReceiptState::Failed);
    }

    if(!receiverKnown) {
        EnterCriticalSection(&g_lock);++g_directionUnknownCount;LeaveCriticalSection(&g_lock);
    } else if(receiverKind==private_logistics_step3b::ReceiverKind::Customs) {
        EnterCriticalSection(&g_lock);++g_customsExportDeferredCount;LeaveCriticalSection(&g_lock);
    }
    char receiverName[64],carrierName[64];
    if(receiverKnown) BuildingName((void*)token.receiver,receiverName,sizeof(receiverName));
    else std::snprintf(receiverName,sizeof(receiverName),"unknown");
    if(token.home) BuildingName((void*)token.home,carrierName,sizeof(carrierName));
    else std::snprintf(carrierName,sizeof(carrierName),"unknown");
    char payerName[64];
    if(decision.payer==private_logistics_step3b::PayerKind::Treasury)
        std::snprintf(payerName,sizeof(payerName),"treasury");
    else if(decision.payer==private_logistics_step3b::PayerKind::PrivateCompany)
        std::snprintf(payerName,sizeof(payerName),"%s",receiverName);
    else
        std::snprintf(payerName,sizeof(payerName),"none");
    if(receiptFull) reason="receipt_capacity";
    Logf("[DO TRIP] assignment=%llu receiver=\"%s\" receiver_kind=%s payer=\"%s\" "
         "carrier=\"%s\" fuel_decrease_native=%.9g fuel_price_global_close=%.9g "
         "charge=%.9g would_pay=%.9g paid=%.9g eligible=%d reason=%s billing=%s "
         "money_moved=%.9g exact_burn=0 price_policy=global_native_price_at_observed_close",
         (unsigned long long)c.id,receiverName,
         receiverKind==private_logistics_step3b::ReceiverKind::Private?"private":
         receiverKind==private_logistics_step3b::ReceiverKind::State?"state":
         receiverKind==private_logistics_step3b::ReceiverKind::Customs?"customs":"unknown",
         payerName,carrierName,c.sampledDecrease,(double)quote.perTonne,quote.amount,
         wouldPay,paid,billable?1:0,reason,
         private_logistics_step3b::BillingStateName(g_billing!=0,g_dryRun!=0),paid);
    if(attemptedLive && !transfer.paid)
        Logf("[DO TRIP] ERROR assignment=%llu reason=%s payer_delta=%.9g payee_delta=%.9g "
             "payer_rollback_verified=%d payee_rollback_verified=%d",
             (unsigned long long)c.id,transfer.reason,transfer.payerDelta,
             transfer.payeeDelta,transfer.payerRollbackVerified?1:0,
             transfer.payeeRollbackVerified?1:0);
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
            if(warn) Logf("[DRY COST] capacity_exhausted=1 prior_records_retained=1 no_eviction=1 money_moved=0");
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
            Logf("[DRY COST] saved=%d records=%u bytes=%llu money_moved=0",ok,count,(unsigned long long)size);
        } else Logf("[DRY COST] save_skipped_invalid_or_blocked=1 existing_file_retained=1");
        std::free(data);return;
    }
    if(event!=TSM_SAVE_LOADING) return;
    const LONG epoch=InterlockedCompareExchange(&g_panelEpoch,0,0);
    EnterCriticalSection(&g_lock);
    g_dryReady=false;g_dryEpoch=epoch;g_dryBook.count=0;g_dryBook.next=1;
    for(auto& t:g_dryTokens) t=DryToken{};
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
         "missing_file=%d blocked=%d prior_active_not_rebound=1 money_moved=0",
         (ok && current)?1:0,count,interrupted,state==0?1:0,(!ok || !current)?1:0);
}
static void AssignmentOnlyEvent(const fleet::a::Event& e,
                                const PanelEndpointPrice& quote,bool complete)
{
    AssignmentSettlementEvent(e,quote,complete);
}
static void DryCostStart() {
    g_fleetCostEvent=AssignmentOnlyEvent;
    g_fleetCostSample=nullptr;
    if(g_billing) {
        Logf("[DRY COST] inactive: billing %s",
             private_logistics_step3b::BillingStateName(g_billing!=0,g_dryRun!=0));
        return;
    }
    if(!g_fleetStarted || !g_persistence ||
       g_persistence->structSize<offsetof(TsmPersistenceApi,readFile)+sizeof(g_persistence->readFile) ||
       !g_persistence->registerHandler || !g_persistence->readFile || !g_persistence->writeAtomic) return;
    EnterCriticalSection(&g_lock);
    g_dryEpoch=InterlockedCompareExchange(&g_panelEpoch,0,0);g_dryReady=true;
    LeaveCriticalSection(&g_lock);
    g_dryPersistenceHandle=g_persistence->registerHandler("private_logistics_costs",DryCostPersistence,nullptr);
    if(!g_dryPersistenceHandle) {
        EnterCriticalSection(&g_lock);g_dryReady=false;LeaveCriticalSection(&g_lock);
        Logf("[DRY COST] registration_failed=1 money_moved=0");return;
    }
    g_fleetCostEvent=DryCostEvent;g_fleetCostSample=DryCostSample;
    Logf("[DRY COST] started file=%s max_records=4096 closed_quotes_immutable=1 "
         "open_save_policy=interrupted_unpriced no_money_writes=1 money_moved=0",DryCostFile);
}
#endif

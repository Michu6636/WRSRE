// Pointer-free, non-economic assignment cost records. Identity is a snapshot,
// NOT a verified native payer ID. Closed quotes are immutable; no money writes.
#ifndef PRIVATE_LOGISTICS_DRY_ASSIGNMENT_LEDGER_H
#define PRIVATE_LOGISTICS_DRY_ASSIGNMENT_LEDGER_H
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
namespace private_logistics_dry {
constexpr unsigned Limit=4096, NameSize=128;
enum class Stage : unsigned char { Open=1, Closed=2, Interrupted=3 };
enum class Reason : unsigned char { None, Removed, OfficeOnly, Replaced, Structure, Unobserved };
constexpr unsigned Partial=1, InterruptedFlag=2, IdentityMissing=4;
struct Identity {
    bool valid=false;
    std::uint32_t type=0;
    float x=0,z=0;
    char asset[NameSize]{};
};
inline bool ValidIdentity(const Identity& t) {
    if(!t.valid) return t.type==0 && t.x==0 && t.z==0 && t.asset[0]==0;
    return t.type<=65535 && std::isfinite(t.x) && std::isfinite(t.z) &&
        std::abs(t.x)<100000 && std::abs(t.z)<100000 && t.asset[0] &&
        std::memchr(t.asset,0,NameSize)!=nullptr;
}
inline Identity MakeIdentity(const char* name,std::uint32_t type,float x,float z) {
    Identity t{};if(!name) return t;
    unsigned n=0;while(n<NameSize && name[n]) ++n;
    if(!n || n==NameSize) return t;
    t.valid=true;t.type=type;t.x=x==0?0:x;t.z=z==0?0:z;
    std::memcpy(t.asset,name,n);
    return ValidIdentity(t)?t:Identity{};
}
inline std::uint64_t Mix(std::uint64_t h,unsigned char b) {return (h^b)*1099511628211ull;}
inline std::uint64_t TargetKey(const Identity& t) {
    if(!t.valid || !ValidIdentity(t)) return 0;
    std::uint64_t h=14695981039346656037ull;
    for(char c:t.asset) h=Mix(h,static_cast<unsigned char>(c));
    std::uint32_t x=0,z=0;std::memcpy(&x,&t.x,4);std::memcpy(&z,&t.z,4);
    const std::uint32_t values[]={t.type,x,z};
    for(auto v:values) for(unsigned i=0;i<4;++i) h=Mix(h,(unsigned char)(v>>(8*i)));
    return h?h:1;
}
struct Quote {bool available=false;double price=0;int currency=0;};
struct Record {
    std::uint64_t id=0;
    Identity target{};
    Stage stage=Stage::Open;
    Reason reason=Reason::None;
    unsigned flags=0,pairs=0;
    double decrease=0,increase=0,price=0,amount=0;
    int currency=0;
    bool available=false;
};
inline bool ValidRecord(const Record& r) {
    if(!r.id || !ValidIdentity(r.target) || (r.flags&~7u) ||
       !std::isfinite(r.decrease) || r.decrease<0 || !std::isfinite(r.increase) || r.increase<0 ||
       !std::isfinite(r.price) || !std::isfinite(r.amount) || r.amount<0 ||
       r.currency<0 || r.currency>1 || static_cast<unsigned>(r.reason)>5) return false;
    if(r.stage!=Stage::Open && r.stage!=Stage::Closed && r.stage!=Stage::Interrupted) return false;
    if(r.stage==Stage::Open && r.reason!=Reason::None) return false;
    if(r.stage==Stage::Closed && (r.reason==Reason::None || r.reason==Reason::Unobserved)) return false;
    if(r.stage==Stage::Interrupted && r.reason!=Reason::Unobserved) return false;
    if(r.available) return r.stage==Stage::Closed && r.pairs && r.price>0 &&
        std::isfinite(r.decrease*r.price) && r.amount==r.decrease*r.price;
    return r.price==0 && r.amount==0;
}
struct Book {
    Record rows[Limit]{};
    unsigned count=0;
    std::uint64_t next=1;
    unsigned Begin(const Identity& t) {
        if(count==Limit || next==UINT64_MAX || !ValidIdentity(t)) return Limit;
        const unsigned i=count++;rows[i]=Record{};rows[i].id=next++;rows[i].target=t;
        if(!t.valid) rows[i].flags|=IdentityMissing;
        return i;
    }
    bool Update(unsigned i,double drop,double rise,unsigned pairs,unsigned flags) {
        if(i>=count || rows[i].stage!=Stage::Open || !std::isfinite(drop) || !std::isfinite(rise) ||
           drop<rows[i].decrease || rise<rows[i].increase || pairs<rows[i].pairs) return false;
        rows[i].decrease=drop;rows[i].increase=rise;rows[i].pairs=pairs;
        rows[i].flags|=(flags&Partial);return true;
    }
    bool Close(unsigned i,const Quote& q,Reason reason) {
        if(i>=count || rows[i].stage!=Stage::Open || reason==Reason::None || reason==Reason::Unobserved) return false;
        auto& r=rows[i];r.stage=Stage::Closed;r.reason=reason;
        const double amount=r.decrease*q.price;
        if(q.available && r.pairs && std::isfinite(q.price) && q.price>0 &&
           q.currency>=0 && q.currency<=1 && std::isfinite(amount) && amount>=0) {
            r.available=true;r.price=q.price;r.amount=amount;r.currency=q.currency;
        } else r.flags|=Partial;
        return true;
    }
    bool Interrupt(unsigned i) {
        if(i>=count || rows[i].stage!=Stage::Open) return false;
        rows[i].stage=Stage::Interrupted;rows[i].reason=Reason::Unobserved;
        rows[i].flags|=Partial|InterruptedFlag;return true;
    }
};
} // namespace private_logistics_dry
#endif

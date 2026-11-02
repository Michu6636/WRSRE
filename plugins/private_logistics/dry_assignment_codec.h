// Versioned explicit little-endian wire format, checksum and strict validation.
#ifndef PRIVATE_LOGISTICS_DRY_ASSIGNMENT_CODEC_H
#define PRIVATE_LOGISTICS_DRY_ASSIGNMENT_CODEC_H
#include "dry_assignment_ledger.h"
namespace private_logistics_dry {
constexpr std::size_t HeaderBytes=28, RowBytes=196, MaxBytes=HeaderBytes+Limit*RowBytes;
static_assert(sizeof(double)==8 && sizeof(float)==4,"wire requires binary64/binary32");
struct Writer {
    unsigned char* p;
    void Int(std::uint64_t v,unsigned n) {for(unsigned i=0;i<n;++i) *p++=(unsigned char)(v>>(8*i));}
    void Real(double v) {std::uint64_t u=0;std::memcpy(&u,&v,8);Int(u,8);}
    void Float(float v) {std::uint32_t u=0;std::memcpy(&u,&v,4);Int(u,4);}
};
struct Reader {
    const unsigned char* p;
    std::uint64_t Int(unsigned n) {std::uint64_t v=0;for(unsigned i=0;i<n;++i) v|=std::uint64_t(*p++)<<(8*i);return v;}
    double Real() {auto u=Int(8);double v=0;std::memcpy(&v,&u,8);return v;}
    float Float() {auto u=(std::uint32_t)Int(4);float v=0;std::memcpy(&v,&u,4);return v;}
};
inline std::uint64_t Checksum(const unsigned char* p,std::size_t n) {
    std::uint64_t h=14695981039346656037ull;
    for(std::size_t i=0;i<n;++i) if(i<20 || i>=28) h=Mix(h,p[i]);
    return h;
}
inline std::size_t Encode(const Book& b,unsigned char* out,std::size_t capacity) {
    if(b.count>Limit || !b.next || capacity<HeaderBytes+b.count*RowBytes) return 0;
    std::memcpy(out,"PLDRY001",8);Writer w{out+8};w.Int(b.count,4);w.Int(b.next,8);w.Int(0,8);
    std::uint64_t prior=0;
    for(unsigned i=0;i<b.count;++i) {
        Record r=b.rows[i];
        if(r.stage==Stage::Open) {r.stage=Stage::Interrupted;r.reason=Reason::Unobserved;r.flags|=Partial|InterruptedFlag;}
        if(!ValidRecord(r) || r.id<=prior || r.id>=b.next) return 0;
        prior=r.id;
        w.Int(r.id,8);w.Int((unsigned)r.stage,1);w.Int((unsigned)r.reason,1);w.Int(r.flags,4);
        w.Int(r.target.valid?1:0,1);w.Int(r.target.type,4);w.Float(r.target.x);w.Float(r.target.z);
        std::memcpy(w.p,r.target.asset,NameSize);w.p+=NameSize;
        w.Real(r.decrease);w.Real(r.increase);w.Int(r.pairs,4);w.Int(r.available?1:0,1);
        w.Real(r.price);w.Real(r.amount);w.Int((unsigned)r.currency,4);
    }
    const auto size=HeaderBytes+b.count*RowBytes;
    Writer hash{out+20};hash.Int(Checksum(out,size),8);return size;
}
// Decode only into a private scratch Book; publish it only after true is returned.
inline bool Decode(const unsigned char* data,std::size_t size,Book& out) {
    if(!data || size<HeaderBytes || size>MaxBytes || std::memcmp(data,"PLDRY001",8)) return false;
    Reader h{data+8};const auto count=h.Int(4),next=h.Int(8),hash=h.Int(8);
    if(count>Limit || !next || size!=HeaderBytes+count*RowBytes || hash!=Checksum(data,size)) return false;
    Reader w{data+HeaderBytes};std::uint64_t prior=0;
    for(unsigned i=0;i<count;++i) {
        Record r{};r.id=w.Int(8);r.stage=(Stage)w.Int(1);r.reason=(Reason)w.Int(1);r.flags=(unsigned)w.Int(4);
        const auto valid=w.Int(1);if(valid>1) return false;
        r.target.valid=valid!=0;r.target.type=(std::uint32_t)w.Int(4);r.target.x=w.Float();r.target.z=w.Float();
        std::memcpy(r.target.asset,w.p,NameSize);w.p+=NameSize;
        r.decrease=w.Real();r.increase=w.Real();r.pairs=(unsigned)w.Int(4);
        const auto available=w.Int(1);if(available>1) return false;
        r.available=available!=0;r.price=w.Real();r.amount=w.Real();r.currency=(int)w.Int(4);
        if(r.stage==Stage::Open || !ValidRecord(r) || r.id<=prior || r.id>=next) return false;
        out.rows[i]=r;prior=r.id;
    }
    out.count=(unsigned)count;out.next=next;return true;
}
} // namespace private_logistics_dry
#endif

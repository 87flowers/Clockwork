#pragma once

#include "position.hpp"

namespace Clockwork {

enum Bound : u8 {
    None,
    Lower,
    Upper,
    Exact,
};

struct TTEntry {
    u16   key16 = 0;
    Move  move  = Move::none();
    i16   score = 0;
    i16   eval  = 0;
    u8    depth = 0;
    Bound bound = Bound::None;
};

static_assert(sizeof(TTEntry) == 10 * sizeof(u8));

struct TTData {
    Value eval;
    Move  move;
    Value score;
    Depth depth;
    Bound bound;
};

class TT {
public:
    static constexpr size_t DEFAULT_SIZE_MB = 16;
    static constexpr size_t TT_ALIGNMENT    = 64;

    TT(size_t mb = DEFAULT_SIZE_MB);
    ~TT();

    std::optional<TTData> probe(const Position& position, i32 ply) const;
    void                  store(const Position& position,
                                i32             ply,
                                Value           eval,
                                Move            move,
                                Value           score,
                                Depth           depth,
                                Bound           bound);
    void                  resize(size_t mb);
    void                  clear();

private:
    TTEntry* m_entries;
    size_t   m_size;
};

}

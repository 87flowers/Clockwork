#pragma once

#include "position.hpp"
#include <array>

namespace Clockwork {

enum Bound : u8 {
    None  = 0,
    Lower = 1,
    Upper = 2,
    Exact = 3,
};

struct TTEntry {
    u16  key16;
    Move move;
    i16  score;
    i16  eval;
    u8   depth;
    u8   ttpv_bound;
};

static_assert(sizeof(TTEntry) == 10 * sizeof(u8));

constexpr usize TT_CLUSTER_SIZE = 3;

struct TTCluster {
    std::array<TTEntry, TT_CLUSTER_SIZE> entries;
    u16                                  current_index;
};

static_assert(sizeof(TTCluster) == 32 * sizeof(u8));

struct TTData {
    Value eval;
    Move  move;
    Value score;
    Depth depth;
    u8    ttpv_bound;

    [[nodiscard]] Bound bound() const {
        return static_cast<Bound>(ttpv_bound & 0b011);
    }
    [[nodiscard]] bool ttpv() const {
        return static_cast<bool>(ttpv_bound & 0b100);
    }
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
                                bool            ttpv,
                                Bound           bound);
    void                  resize(size_t mb);
    void                  clear();

private:
    TTCluster* m_clusters;
    size_t     m_size;
};

}

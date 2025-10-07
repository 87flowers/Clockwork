#include "tt.hpp"
#include <atomic>

namespace Clockwork {

static u16 shrink_key(HashKey key) {
    return static_cast<u16>(key);
}

static u64 mulhi64(u64 a, u64 b) {
    u128 result = static_cast<u128>(a) * static_cast<u128>(b);
    return static_cast<u64>(result >> 64);
}

void* aligned_alloc(size_t alignment, size_t size) {
#ifdef _WIN32
    return _aligned_malloc(size, alignment);
#else
    return std::aligned_alloc(alignment, size);
#endif
}

void aligned_free(void* ptr) {
    if (ptr == nullptr) {
        return;
    }
#ifdef _WIN32
    return _aligned_free(ptr);
#else
    return std::free(ptr);
#endif
}

[[nodiscard]] static u8 make_ttpv_bound(bool is_tt_pv, Bound bound) {
    return static_cast<u8>(bound) | (static_cast<u8>(is_tt_pv) << 2);
}

i16 score_to_tt(Value score, i32 ply) {
    if (score > VALUE_WIN) {
        return static_cast<i16>(score + ply);
    } else if (score < -VALUE_WIN) {
        return static_cast<i16>(score - ply);
    } else {
        return static_cast<i16>(score);
    }
}

Value score_from_tt(i16 ttScore, i32 ply) {
    if (ttScore > VALUE_WIN) {
        return static_cast<Value>(ttScore - ply);
    } else if (ttScore < -VALUE_WIN) {
        return static_cast<Value>(ttScore + ply);
    } else {
        return static_cast<Value>(ttScore);
    }
}

TT::TT(size_t mb) :
    m_clusters{nullptr},
    m_size{0} {
    resize(mb);
}

TT::~TT() {
    aligned_free(m_clusters);
}

std::optional<TTData> TT::probe(const Position& pos, i32 ply) const {
    size_t idx     = mulhi64(pos.get_hash_key(), m_size);
    auto&  cluster = m_clusters[idx];
    for (const auto& entry : cluster.entries) {
        if (entry.key16 == shrink_key(pos.get_hash_key())) {
            TTData data = {.eval       = entry.eval,
                           .move       = entry.move,
                           .score      = score_from_tt(entry.score, ply),
                           .depth      = static_cast<Depth>(entry.depth),
                           .ttpv_bound = entry.ttpv_bound};
            return {data};
        }
    }
    return {};
}

void TT::store(const Position& pos,
               i32             ply,
               Value           eval,
               Move            move,
               Value           score,
               Depth           depth,
               bool            ttpv,
               Bound           bound) {
    size_t idx     = mulhi64(pos.get_hash_key(), m_size);
    auto&  cluster = m_clusters[idx];

    auto& entry = [&]() -> TTEntry& {
        for (auto& entry : cluster.entries) {
            if (entry.key16 == shrink_key(pos.get_hash_key())) {
                return entry;
            }
        }

        std::atomic_ref current_index{cluster.current_index};
        return cluster.entries[current_index.fetch_add(1) % TT_CLUSTER_SIZE];
    }();

    entry.key16      = shrink_key(pos.get_hash_key());
    entry.move       = move;
    entry.score      = score_to_tt(score, ply);
    entry.eval       = static_cast<i16>(eval);
    entry.depth      = static_cast<u8>(depth);
    entry.ttpv_bound = make_ttpv_bound(ttpv, bound);
}

void TT::resize(size_t mb) {
    aligned_free(m_clusters);

    size_t bytes    = mb * 1024 * 1024;
    size_t clusters = bytes / sizeof(TTCluster);

    m_size     = clusters;
    m_clusters = static_cast<TTCluster*>(aligned_alloc(TT_ALIGNMENT, bytes));
    clear();
}

void TT::clear() {
    std::fill(m_clusters, m_clusters + m_size, TTCluster{});
}

}

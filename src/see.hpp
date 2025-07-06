#pragma once

#include "common.hpp"
#include "geometry.hpp"
#include "move.hpp"
#include "position.hpp"
#include "util/types.hpp"
#include "util/vec.hpp"
#include <array>
#include <bit>
#include <cassert>
#include <tuple>

namespace Clockwork::SEE {

struct SideInfo {
    SideInfo(const Position& pos, Color side, Square focus) :
        m_attack_table(pos.attack_table(side)),
        m_ptypes(pos.piece_list(side).to_vec()),
        m_sqs(pos.piece_list_sq(side)) {
        m_current_attackers = m_attack_table.read(focus);
        m_used_attackers    = m_current_attackers;
        m_rays              = geometry::ray_indexes(focus, m_sqs.to_vec());
    }

    forceinline std::tuple<PieceType, Square, v128> next() {
        v128 x   = m_ptypes | v128::fromMask8(~m_current_attackers);
        u8   min = v128::horizontalminu8(x);
        assert(min != 0xFF);

        PieceId id{static_cast<u8>(std::countr_zero(v128::eq8(v128::broadcast8(min), x)))};
        v128    ray = v128::permute8(v128::broadcast8(id.raw), m_rays);
        pop(id);
        return {static_cast<PieceType>(min), m_sqs[id], ray};
    }

    forceinline void pop(PieceId id) {
        Square attacker_sq = m_sqs[id];
        v128   ray         = v128::permute8(v128::broadcast8(id.raw), m_rays);

        u16 mask            = id.to_piece_mask();
        m_used_attackers    = m_used_attackers | mask;
        m_current_attackers = m_current_attackers & ~mask;

        free(attacker_sq, ray);
    }

    forceinline void free(Square attacker_sq, v128 ray) {
        u16 potential = v128::eq8(m_rays, ray);
        u16 next      = m_attack_table.read(attacker_sq);

        m_current_attackers |= potential & ~m_used_attackers & next;
    }

    bool has_attackers() const {
        return m_current_attackers != 0;
    }

private:
    const Wordboard&  m_attack_table;
    u16               m_current_attackers;
    u16               m_used_attackers;
    v128              m_rays;
    v128              m_ptypes;
    PieceList<Square> m_sqs;
};

inline Value value(PieceType ptype) {
    constexpr std::array<Value, 7> table{{0, 100, 300, 300, 500, 900, 10000}};
    return table[static_cast<usize>(ptype)];
}

inline Value gain(const Position& pos, Move move) {
    if (move.is_castle()) {
        return 0;
    }
    if (move.is_en_passant()) {
        return value(PieceType::Pawn);
    }

    Value score = value(pos.board()[move.to()].ptype());
    if (move.is_promotion()) {
        score += value(*move.promo()) - value(PieceType::Pawn);
    }
    return score;
}

// based on stormphrax, which was ported from ethereal/weiss
inline bool see(const Position& pos, Move move, Value threshold) {
    Square sq  = move.to();
    Color  stm = pos.active_color();

    std::array<SideInfo, 2> info{{{pos, Color::White, sq}, {pos, Color::Black, sq}}};

    Value score = gain(pos, move) - threshold;
    if (score < 0) {
        return false;
    }

    info[static_cast<usize>(stm)].pop(pos.board()[move.from()].id());
    PieceType next = pos.board()[move.from()].ptype();
    score -= value(next);
    stm = invert(stm);

    if (score >= 0) {
        return true;
    }

    Square attacker_sq;
    v128   ray;
    while (true) {
        auto& us   = info[static_cast<usize>(stm)];
        auto& them = info[static_cast<usize>(invert(stm))];

        if (!us.has_attackers()) {
            break;
        }

        std::tie(next, attacker_sq, ray) = us.next();
        them.free(attacker_sq, ray);

        score = -score - 1 - value(next);
        stm   = invert(stm);

        if (score >= 0) {
            if (next == PieceType::King && them.has_attackers()) {
                // We'd be in check if we actually did that.
                stm = invert(stm);
            }
            break;
        }
    }

    return stm != pos.active_color();
}

}

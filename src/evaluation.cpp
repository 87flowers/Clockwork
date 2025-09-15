#include <array>
#include <ranges>

#include "bitboard.hpp"
#include "common.hpp"
#include "eval_constants.hpp"
#include "position.hpp"
#include "psqt_state.hpp"

#include "evaluation.hpp"

#include "eval_types.hpp"
#include "square.hpp"

namespace Clockwork {

std::array<Bitboard, 64> king_ring_table = []() {
    std::array<Bitboard, 64> king_ring_table{};
    for (u8 sq_idx = 0; sq_idx < 64; sq_idx++) {
        Bitboard sq_bb     = Bitboard::from_square(Square{sq_idx});
        Bitboard king_ring = sq_bb;
        king_ring |= sq_bb.shift(Direction::North);
        king_ring |= sq_bb.shift(Direction::South);
        king_ring |= sq_bb.shift(Direction::East);
        king_ring |= sq_bb.shift(Direction::West);
        king_ring |= sq_bb.shift(Direction::NorthEast);
        king_ring |= sq_bb.shift(Direction::SouthEast);
        king_ring |= sq_bb.shift(Direction::NorthWest);
        king_ring |= sq_bb.shift(Direction::SouthWest);
        king_ring_table[sq_idx] = king_ring;
    }
    return king_ring_table;
}();

Score evaluate_white_pov(const Position& pos, const PinInfos& pi, const PsqtState& psqt_state) {
    const Color us    = pos.active_color();
    i32         phase = pos.piece_count(Color::White, PieceType::Knight)
              + pos.piece_count(Color::Black, PieceType::Knight)
              + pos.piece_count(Color::White, PieceType::Bishop)
              + pos.piece_count(Color::Black, PieceType::Bishop)
              + 2
                  * (pos.piece_count(Color::White, PieceType::Rook)
                     + pos.piece_count(Color::Black, PieceType::Rook))
              + 4
                  * (pos.piece_count(Color::White, PieceType::Queen)
                     + pos.piece_count(Color::Black, PieceType::Queen));

    phase = std::min<i32>(phase, 24);

    PScore mobility    = PSCORE_ZERO;
    PScore king_attack = PSCORE_ZERO;

    auto add_mobility = [&](Color c, PScore& mob_count, PScore& k_attack) {
        Bitboard bb = pos.bitboard_for(c, PieceType::Pawn) | pos.attacked_by(~c, PieceType::Pawn);
        Bitboard king_ring = king_ring_table[pos.king_sq(~c).raw];
        for (PieceId id : pos.get_piece_mask(c, PieceType::Knight)) {
            mobility += KNIGHT_MOBILITY[pos.mobility_of(pi, c, id, ~bb)];
            k_attack += KNIGHT_KING_RING[pos.mobility_of(c, id, king_ring)];
        }
        for (PieceId id : pos.get_piece_mask(c, PieceType::Bishop)) {
            mobility += BISHOP_MOBILITY[pos.mobility_of(pi, c, id, ~bb)];
            k_attack += BISHOP_KING_RING[pos.mobility_of(c, id, king_ring)];
        }
        for (PieceId id : pos.get_piece_mask(c, PieceType::Rook)) {
            mobility += ROOK_MOBILITY[pos.mobility_of(pi, c, id, ~bb)];
            k_attack += ROOK_KING_RING[pos.mobility_of(c, id, king_ring)];
        }
        for (PieceId id : pos.get_piece_mask(c, PieceType::Queen)) {
            mobility += QUEEN_MOBILITY[pos.mobility_of(pi, c, id, ~bb)];
            k_attack += QUEEN_KING_RING[pos.mobility_of(c, id, king_ring)];
        }
        mobility += KING_MOBILITY[pos.mobility_of(c, PieceId::king(), ~bb)];
    };

    add_mobility(Color::Black, mobility, king_attack);
    mobility *= -1;  // Persy trick
    king_attack *= -1;
    add_mobility(Color::White, mobility, king_attack);

    const std::array<Bitboard, 2> pawns = {pos.board().bitboard_for(Color::White, PieceType::Pawn),
                                           pos.board().bitboard_for(Color::Black, PieceType::Pawn)};

    PScore doubled_pawns_bonus = DOUBLED_PAWN_VAL
                               * ((pawns[0] & pawns[0].shift(Direction::North)).popcount()
                                  - (pawns[1] & pawns[1].shift(Direction::South)).popcount());

    PScore bishop_pair_bonus = BISHOP_PAIR_VAL
                             * ((pos.piece_count(Color::White, PieceType::Bishop) >= 2)
                                - (pos.piece_count(Color::Black, PieceType::Bishop) >= 2));

    PScore tempo = (us == Color::White) ? TEMPO_VAL : -TEMPO_VAL;
    PScore sum =
      psqt_state.score() + mobility + king_attack + tempo + bishop_pair_bonus + doubled_pawns_bonus;
    return sum->phase<24>(phase);
};

Score evaluate_stm_pov(const Position& pos, const PinInfos& pi, const PsqtState& psqt_state) {
    const Color us = pos.active_color();
    return (us == Color::White) ? evaluate_white_pov(pos, pi, psqt_state)
                                : -evaluate_white_pov(pos, pi, psqt_state);
}

}  // namespace Clockwork

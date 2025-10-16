#pragma once

#include <array>
#include <optional>

#include "history.hpp"
#include "movegen.hpp"
#include "position.hpp"
#include "search.hpp"

namespace Clockwork {

bool quiet_move(Move move);

class MovePicker {
public:
    explicit MovePicker(
      const Position& pos, const History& history, Move tt_move, i32 ply, Search::Stack* ss) :
        m_pos(pos),
        m_history(history),
        m_movegen(pos),
        m_tt_move(tt_move),
        m_threat_move(ss->threat),
        m_killer(ss->killer),
        m_ply(ply),
        m_stack(ss) {
    }

    void skip_quiets();

    Move next();

    [[nodiscard]] bool is_legal(Move m) const {
        return m_movegen.is_legal(m);
    }

private:
    enum class Stage {
        EmitTTMove,
        EmitThreatMove,
        GenerateMoves,
        ScoreNoisy,
        EmitGoodNoisy,
        EmitKiller,
        ScoreQuiet,
        EmitQuiet,
        EmitBadNoisy,
        End,
    };

    bool is_early_refutation(Move m) {
        return m == m_tt_move || m == m_threat_move;
    }

    bool is_late_refutation(Move m) {
        return m == m_killer;
    }

    bool is_refutation(Move m) {
        return is_early_refutation(m) || is_late_refutation(m);
    }

    void                 generate_moves();
    std::pair<Move, i32> pick_next(MoveList& moves);
    void                 score_moves(MoveList& moves);

    i32 score_move(Move move) const;

    Stage m_stage = Stage::EmitTTMove;

    const Position&      m_pos;
    const History&       m_history;
    MoveGen              m_movegen;
    MoveList             m_noisy;
    MoveList             m_quiet;
    MoveList             m_bad_noisy;
    usize                m_current_index = 0;
    bool                 m_skip_quiets   = false;
    std::array<i32, 256> m_scores;

    Move           m_tt_move;
    Move           m_threat_move;
    Move           m_killer;
    i32            m_ply;
    Search::Stack* m_stack;
};

}

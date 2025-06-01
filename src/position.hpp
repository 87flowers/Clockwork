#pragma once

#include <array>
#include <cassert>
#include <iosfwd>

#include "board.hpp"
#include "square.hpp"
#include "util/types.hpp"
#include "util/vec.hpp"

namespace Clockwork {

template<typename T>
union alignas(16) PieceList {
    v128              raw;
    std::array<T, 16> array{};

    constexpr T& operator[](PieceId id) { return array[id.raw]; }
    constexpr T  operator[](PieceId id) const { return array[id.raw]; }

    u16 maskEq(PieceType ptype) const {
        return v128::test8(raw, v128::broadcast8(static_cast<u8>(ptype)));
    }

    constexpr bool operator==(const PieceList& other) const { return array == other.array; }
};

struct RookInfo {
    Square aside = Square::invalid();
    Square hside = Square::invalid();

    constexpr void clear() {
        aside = Square::invalid();
        hside = Square::invalid();
    }

    constexpr void unset(Square sq) {
        aside = aside == sq ? Square::invalid() : aside;
        hside = hside == sq ? Square::invalid() : hside;
    }

    constexpr bool isClear() const { return !aside.isValid() && !hside.isValid(); }

    constexpr bool operator==(const RookInfo&) const = default;
};

struct Position {
   private:
    std::array<Wordboard, 2>            m_attack_table{};
    std::array<PieceList<Square>, 2>    m_piece_list_sq{};
    std::array<PieceList<PieceType>, 2> m_piece_list{};
    Byteboard                           m_board{};
    u64                                 m_hash{};
    u16                                 m_50mr{};
    u16                                 m_ply{};
    Color                               m_active_color{};
    Square                              m_enpassant = Square::invalid();
    std::array<RookInfo, 2>             m_rook_info;

   public:
    constexpr Position() = default;

    const Byteboard& board() const { return m_board; }
    const Wordboard& attackTable(Color color) const { return m_attack_table[(int) color]; }
    const PieceList<PieceType>& pieceList(Color color) const { return m_piece_list[(int) color]; }
    const PieceList<Square>& pieceListSq(Color color) const { return m_piece_list_sq[(int) color]; }
    Color                    activeColor() const { return m_active_color; }
    Square                   enpassant() const { return m_enpassant; }
    RookInfo                 rookInfo(Color color) const { return m_rook_info[(int) color]; }

    Square kingSq(Color color) const { return pieceListSq(color)[PieceId{0}]; }

    const std::array<Wordboard, 2> calcAttacksSlow();
    const std::array<u16, 2>       calcAttacksSlow(Square sq);

    static std::optional<Position> parse(std::string_view str);
    static std::optional<Position> parse(std::string_view board,
                                         std::string_view color,
                                         std::string_view castle,
                                         std::string_view enpassant,
                                         std::string_view irreversible_clock,
                                         std::string_view ply);

    bool                 operator==(const Position&) const = default;
    friend std::ostream& operator<<(std::ostream& os, const Position& position);
};

}

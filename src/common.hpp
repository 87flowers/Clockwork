#pragma once

#include "util/types.hpp"

namespace Clockwork {

enum class Color {
    white,
    black
};

constexpr char color_char(Color color) {
    switch (color)
    {
    case Color::white :
        return 'w';
    case Color::black :
        return 'b';
    }
}

constexpr Color invert(Color color) { return static_cast<Color>(static_cast<int>(color) ^ 1); }

enum class PieceType : u8 {
    none,
    pawn,
    knight,
    bishop,
    rook,
    queen,
    king,
};

constexpr char piece_char(PieceType piece) {
    switch (piece)
    {
    case PieceType::none :
        return '.';
    case PieceType::pawn :
        return 'p';
    case PieceType::knight :
        return 'n';
    case PieceType::bishop :
        return 'b';
    case PieceType::rook :
        return 'r';
    case PieceType::queen :
        return 'q';
    case PieceType::king :
        return 'k';
    }
}

}

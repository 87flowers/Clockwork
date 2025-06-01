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

enum class Piece {
    none,
    pawn,
    knight,
    bishop,
    rook,
    queen,
    king,
};

constexpr char piece_char(Piece piece) {
    switch (piece)
    {
    case Piece::none :
        return '.';
    case Piece::pawn :
        return 'p';
    case Piece::knight :
        return 'n';
    case Piece::bishop :
        return 'b';
    case Piece::rook :
        return 'r';
    case Piece::queen :
        return 'q';
    case Piece::king :
        return 'k';
    }
}

}

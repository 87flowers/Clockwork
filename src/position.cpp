#include "position.hpp"

#include <sstream>

namespace Clockwork {

const std::array<Wordboard, 2> Position::calcAttacksSlow() {
    // TODO
    return {};
}
const std::array<u16, 2> Position::calcAttacksSlow(Square sq) {
    // TODO
    (void) sq;
    return {};
}

std::optional<Position> Position::parse(std::string_view str) {
    std::istringstream is{std::string{str}};

    std::string board, color, castle, enpassant, irreversible_clock, ply;
    is >> board >> color >> castle >> enpassant >> irreversible_clock >> ply;

    return parse(board, color, castle, enpassant, irreversible_clock, ply);
}

std::optional<Position> Position::parse(std::string_view board,
                                        std::string_view color,
                                        std::string_view castle,
                                        std::string_view enpassant,
                                        std::string_view irreversible_clock,
                                        std::string_view ply) {
    Position result{};

    // Parse board
    {
        int               place_index = 0;
        usize             i           = 0;
        std::array<u8, 2> id          = {1, 1};
        for (; place_index < 64 && i < board.size(); i++)
        {
            const int    file = place_index % 8;
            const int    rank = 7 - place_index / 8;
            const Square sq   = Square::fromFileAndRank(file, rank);
            const char   ch   = board[i];

            const auto put_piece_raw = [&](Color color, Piece piece, u8 current_id) {
                result.m_board.mailbox[sq.raw] = Place{color, piece, PieceId{current_id}};
                result.m_piece_list_sq[(int) color].array[current_id] = sq;
                result.m_piece_list[(int) color].array[current_id]    = piece;
                place_index++;
            };

            const auto put_piece = [&](Color color, Piece piece) -> bool {
                u8& current_id = id[(int) color];
                if (current_id >= 0x10)
                    return false;
                put_piece_raw(color, piece, current_id);
                current_id++;
                return true;
            };

            switch (ch)
            {
            case '/' :
                if (file != 0 || place_index == 0)
                    return std::nullopt;
                break;
            case '1' :
            case '2' :
            case '3' :
            case '4' :
            case '5' :
            case '6' :
            case '7' :
            case '8' :
                place_index += ch - '0';
                break;
            case 'K' :
                // TODO: Error if king already on board
                put_piece_raw(Color::white, Piece::king, 0);
                break;
            case 'Q' :
                if (!put_piece(Color::white, Piece::queen))
                    return std::nullopt;
                break;
            case 'R' :
                if (!put_piece(Color::white, Piece::rook))
                    return std::nullopt;
                break;
            case 'B' :
                if (!put_piece(Color::white, Piece::bishop))
                    return std::nullopt;
                break;
            case 'N' :
                if (!put_piece(Color::white, Piece::knight))
                    return std::nullopt;
                break;
            case 'P' :
                if (!put_piece(Color::white, Piece::pawn))
                    return std::nullopt;
                break;
            case 'k' :
                // TODO: Error if king already on board
                put_piece_raw(Color::black, Piece::king, 0);
                break;
            case 'q' :
                if (!put_piece(Color::black, Piece::queen))
                    return std::nullopt;
                break;
            case 'r' :
                if (!put_piece(Color::black, Piece::rook))
                    return std::nullopt;
                break;
            case 'b' :
                if (!put_piece(Color::black, Piece::bishop))
                    return std::nullopt;
                break;
            case 'n' :
                if (!put_piece(Color::black, Piece::knight))
                    return std::nullopt;
                break;
            case 'p' :
                if (!put_piece(Color::black, Piece::pawn))
                    return std::nullopt;
                break;
            default :
                return std::nullopt;
            }
        }
        if (place_index != 64 || i != board.size())
            return std::nullopt;
    }

    // Parse color
    if (color.size() != 1)
        return std::nullopt;
    switch (color[0])
    {
    case 'b' :
        result.m_active_color = Color::black;
        break;
    case 'w' :
        result.m_active_color = Color::white;
        break;
    default :
        return std::nullopt;
    }

    // Parse castling rights
    // TODO: FRC, Error detection
    if (castle != "-")
    {
        for (const char ch : castle)
        {
            switch (ch)
            {
            case 'K' :
            case 'H' :
                result.m_rook_info[0].hside = *Square::parse("h1");
                break;
            case 'Q' :
            case 'A' :
                result.m_rook_info[0].aside = *Square::parse("a1");
                break;
            case 'k' :
            case 'h' :
                result.m_rook_info[1].hside = *Square::parse("h8");
                break;
            case 'q' :
            case 'a' :
                result.m_rook_info[1].aside = *Square::parse("a8");
                break;
            default :
                return std::nullopt;
            }
        }
    }

    // Enpassant
    if (enpassant != "-")
    {
        const auto sq = Square::parse(enpassant);
        if (!sq)
            return std::nullopt;
        result.m_enpassant = *sq;
    }

    // Parse 50mr clock
    if (const usize value = std::stoi(std::string{irreversible_clock}); value <= 100)
    {
        result.m_50mr = static_cast<u16>(value);
    }
    else
    {
        return std::nullopt;
    }

    // Parse game ply
    if (const usize value = std::stoi(std::string{ply}); value != 0 && value < 10000)
    {
        result.m_ply = static_cast<u16>((value - 1) * 2 + (int) result.m_active_color);
    }
    else
    {
        return std::nullopt;
    }

    return result;
}

std::ostream& operator<<(std::ostream& os, const Position& position) {
    int        blanks      = 0;
    const auto emit_blanks = [&] {
        if (blanks != 0)
        {
            os << blanks;
            blanks = 0;
        }
    };

    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            const Square sq = Square::fromFileAndRank(file, rank);
            const Place  p  = position.m_board.mailbox[sq.raw];

            if (p.isEmpty())
            {
                blanks++;
            }
            else
            {
                emit_blanks();
                os << p.toChar();
            }

            if (file == 7)
            {
                emit_blanks();
                if (rank != 0)
                    os << '/';
            }
        }
    }

    os << ' ' << color_char(position.m_active_color) << ' ';

    // TODO: FRC
    const RookInfo white_rook_info = position.rookInfo(Color::white);
    const RookInfo black_rook_info = position.rookInfo(Color::black);
    if (white_rook_info.isClear() && black_rook_info.isClear())
        os << '-';
    if (white_rook_info.hside.isValid())
        os << 'K';
    if (white_rook_info.aside.isValid())
        os << 'Q';
    if (black_rook_info.hside.isValid())
        os << 'k';
    if (black_rook_info.aside.isValid())
        os << 'q';

    if (position.m_enpassant.isValid())
        os << ' ' << position.m_enpassant << ' ';
    else
        os << " - ";

    os << position.m_50mr << ' ' << (position.m_ply / 2 + 1);

    return os;
}

}

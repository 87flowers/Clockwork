#include "position.hpp"
#include "see.hpp"
#include "square.hpp"
#include "test.hpp"
#include "util/types.hpp"
#include <array>
#include <bit>
#include <iomanip>
#include <iostream>

using namespace Clockwork;

void test_pos(std::string_view pos_str, std::string_view move_str, Value value) {
    Position pos  = Position::parse(pos_str).value();
    Move     move = Move::parse(move_str, pos).value();

    std::cout << pos << " | " << move << " | " << value << std::endl;

    REQUIRE(SEE::see(pos, move, value - 100) == true);
    REQUIRE(SEE::see(pos, move, value + 1) == false);
}

void test_pos1() {
    Position pos  = Position::parse("3r3k/3r4/2n1n3/8/3p4/2PR4/1B1Q4/3R3K w - - 0 1").value();
    Move     move = Move::parse("d3d4", pos).value();

    REQUIRE(SEE::see(pos, move, -200) == true);
    REQUIRE(SEE::see(pos, move, -100) == true);
    REQUIRE(SEE::see(pos, move, -99) == false);
    REQUIRE(SEE::see(pos, move, 0) == false);
}

void test_sideinfo_next1() {
    Position pos = Position::parse("3r3k/3r4/2n1n3/8/3p4/2PR4/1B1Q4/3R3K w - - 0 1").value();

    SEE::SideInfo white{pos, Color::White, Square::parse("d4").value()};

    PieceType ptype;
    Square    square;
    v128      ray;

    REQUIRE(white.has_attackers() == true);
    std::tie(ptype, square, ray) = white.next();
    REQUIRE(ptype == PieceType::Pawn);
    REQUIRE(square == Square::parse("c3"));
    std::cout << square << std::endl;

    REQUIRE(white.has_attackers() == true);
    std::tie(ptype, square, ray) = white.next();
    REQUIRE(ptype == PieceType::Bishop);
    REQUIRE(square == Square::parse("b2"));
    std::cout << square << std::endl;

    REQUIRE(white.has_attackers() == true);
    std::tie(ptype, square, ray) = white.next();
    REQUIRE(ptype == PieceType::Rook);
    REQUIRE(square == Square::parse("d3"));
    std::cout << square << std::endl;

    REQUIRE(white.has_attackers() == true);
    std::tie(ptype, square, ray) = white.next();
    REQUIRE(ptype == PieceType::Queen);
    REQUIRE(square == Square::parse("d2"));
    std::cout << square << std::endl;

    REQUIRE(white.has_attackers() == true);
    std::tie(ptype, square, ray) = white.next();
    REQUIRE(ptype == PieceType::Rook);
    REQUIRE(square == Square::parse("d1"));
    std::cout << square << std::endl;

    REQUIRE(white.has_attackers() == false);

    SEE::SideInfo black{pos, Color::Black, Square::parse("d4").value()};

    REQUIRE(black.has_attackers() == true);
    std::tie(ptype, square, ray) = black.next();
    REQUIRE(ptype == PieceType::Knight);
    std::cout << square << std::endl;

    REQUIRE(black.has_attackers() == true);
    std::tie(ptype, square, ray) = black.next();
    REQUIRE(ptype == PieceType::Knight);
    std::cout << square << std::endl;

    REQUIRE(black.has_attackers() == true);
    std::tie(ptype, square, ray) = black.next();
    REQUIRE(ptype == PieceType::Rook);
    REQUIRE(square == Square::parse("d7"));
    std::cout << square << std::endl;

    REQUIRE(black.has_attackers() == true);
    std::tie(ptype, square, ray) = black.next();
    REQUIRE(ptype == PieceType::Rook);
    REQUIRE(square == Square::parse("d8"));
    std::cout << square << std::endl;

    REQUIRE(black.has_attackers() == false);
}

void test_sideinfo_next2() {
    Position pos =
      Position::parse("6k1/1pp4p/p1pb4/6q1/3P1pRr/2P4P/PP1Br1P1/5RKN w - - 0 1").value();

    SEE::SideInfo white{pos, Color::White, Square::parse("f4").value()};
    SEE::SideInfo black{pos, Color::Black, Square::parse("f4").value()};
}

int main() {
    test_pos1();
    test_sideinfo_next1();
    test_sideinfo_next2();
    test_pos("6k1/1pp4p/p1pb4/6q1/3P1pRr/2P4P/PP1Br1P1/5RKN w - - 0 1", "f1f4", -100);
    return 0;
}

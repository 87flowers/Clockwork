#include "geometry.hpp"
#include "square.hpp"
#include "test.hpp"
#include "util/types.hpp"
#include <array>
#include <bit>
#include <iomanip>
#include <iostream>

using namespace Clockwork;

// Example from https://87flowers.com/gf2p8affineqb-piece-directions/

int main() {
    Square sq = Square::parse("d4").value();
    v128   coords{std::array<u8, 16>{
      Square::parse("d6").value().raw,
      Square::parse("f6").value().raw,
      Square::parse("h4").value().raw,
      Square::parse("e3").value().raw,
      Square::parse("d2").value().raw,
      Square::parse("a1").value().raw,
      Square::parse("b4").value().raw,
      Square::parse("a7").value().raw,
      Square::parse("e7").value().raw,
      Square::parse("f5").value().raw,
      Square::parse("h3").value().raw,
      Square::parse("e2").value().raw,
      Square::parse("b1").value().raw,
      Square::parse("a2").value().raw,
      Square::parse("a6").value().raw,
      Square::parse("b8").value().raw,
    }};

    v128 rays       = geometry::ray_indexes(sq, coords);
    auto rays_array = std::bit_cast<std::array<u8, 16>>(rays);

    std::cout << std::hex;
    for (i32 i = 0; i < 16; i++) {
        std::cout << std::setfill('0') << std::setw(2) << (int)rays_array[i] << ' ';
    }
    std::cout << std::endl;

    REQUIRE((rays
             == v128{std::array<u8, 16>{0, 1, 2, 3, 4, 5, 6, 7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                        0xFF, 0xFF}}));
}

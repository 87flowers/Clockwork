#pragma once

#include <array>
#include <iosfwd>

#include "util/types.hpp"
#include "util/vec.hpp"
#include "common.hpp"
#include "square.hpp"

namespace Clockwork {

struct PieceId {
    u8 raw;

    constexpr PieceId(u8 raw) :
        raw(raw) {
        assert(raw < 0x10);
    }

    constexpr u16 toPieceMask() const { return static_cast<u16>(1 << raw); }
};

static_assert(sizeof(PieceId) == sizeof(u8));

struct Place {
    inline static constexpr u8 slider_bit = 0b100 << 4;

    u8 raw = 0;

    static constexpr Place empty() { return {}; }

    constexpr Place() = default;
    constexpr Place(Color color, PieceType pt, PieceId id) {
        raw =
          static_cast<u8>((static_cast<int>(color) << 7) | (static_cast<int>(pt) << 4) | id.raw);
    }

    constexpr bool      isEmpty() const { return raw == 0; }
    constexpr Color     color() const { return static_cast<Color>((raw & 0x80) != 0); }
    constexpr PieceType ptype() const { return static_cast<PieceType>((raw >> 4) & 0x7); }
    constexpr PieceId   id() const { return PieceId{static_cast<u8>(raw & 0xF)}; }

    constexpr char toChar() const {
        constexpr std::array<std::string_view, 2> str{{".PNBRQK", ".pnbrqk"}};
        return str[static_cast<int>(color())][static_cast<int>(ptype())];
    }

    inline constexpr bool operator==(const Place&) const = default;
};

static_assert(sizeof(Place) == sizeof(u8));

union Byteboard {
    v512                  raw;
    std::array<Place, 64> mailbox;

    u64 getEmptyBitboard() const { return raw.zero8(); }

    u64 getColorBitboard(Color color) const {
        return ~(raw.msb8() ^ -static_cast<i64>(color)) & raw.nonzero8();
    }

    u64 bitboardFor(Color color, PieceType ptype) const {
        const Place p{color, ptype, PieceId{0}};
        return v512::eq8(raw & v512::broadcast8(0xF0), v512::broadcast8(p.raw));
    }

    constexpr Place& operator[](Square sq) { return mailbox[sq.raw]; }
    constexpr Place  operator[](Square sq) const { return mailbox[sq.raw]; }

    bool operator==(const Byteboard& other) const = default;
};

static_assert(sizeof(Byteboard) == 64);

union Wordboard {
    std::array<v512, 2> raw;
    std::array<u16, 64> mailbox;

    u64 getAttackedBitboard() const { return concat64(raw[0].nonzero16(), raw[1].nonzero16()); }

    constexpr u16& operator[](Square sq) { return mailbox[sq.raw]; }
    constexpr u16  operator[](Square sq) const { return mailbox[sq.raw]; }

    bool                 operator==(const Wordboard& other) const = default;
    friend std::ostream& operator<<(std::ostream& os, const Wordboard& at);
};

static_assert(sizeof(Wordboard) == 128);

}

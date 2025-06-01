#pragma once

#include <array>
#include <x86intrin.h>

namespace Clockwork {

union v128 {
    __m128i            raw{};
    std::array<u8, 16> b;
    std::array<u16, 8> w;
    std::array<u32, 4> d;
    std::array<u64, 2> q;

    using Mask8  = u16;
    using Mask16 = u8;
    using Mask32 = u8;
    using Mask64 = u8;

    forceinline constexpr v128() = default;
    forceinline constexpr v128(__m128i raw) :
        raw(raw){};
    forceinline constexpr explicit v128(std::array<u8, 16> src) :
        b(src) {}
    forceinline constexpr explicit v128(std::array<u16, 8> src) :
        w(src) {}
    forceinline constexpr explicit v128(std::array<u32, 4> src) :
        d(src) {}
    forceinline constexpr explicit v128(std::array<u64, 2> src) :
        q(src) {}

    forceinline auto operator==(const v128& other) const -> bool {
        const __m128i t = _mm_xor_si128(raw, other.raw);
        return _mm_testz_si128(t, t);
    }
};
static_assert(sizeof(v128) == 16);

union v256 {
    __m256i             raw{};
    std::array<u8, 32>  b;
    std::array<u16, 16> w;
    std::array<u32, 8>  d;
    std::array<u64, 4>  q;

    using Mask8  = u32;
    using Mask16 = u16;
    using Mask32 = u8;
    using Mask64 = u8;

    forceinline constexpr v256() = default;
    forceinline constexpr v256(__m256i raw) :
        raw(raw){};
    forceinline constexpr explicit v256(std::array<u8, 32> src) :
        b(src) {}
    forceinline constexpr explicit v256(std::array<u16, 16> src) :
        w(src) {}
    forceinline constexpr explicit v256(std::array<u32, 8> src) :
        d(src) {}
    forceinline constexpr explicit v256(std::array<u64, 4> src) :
        q(src) {}

    forceinline auto operator==(const v256& other) const -> bool {
        const __m256i t = _mm256_xor_si256(raw, other.raw);
        return _mm256_testz_si256(t, t);
    }
};
static_assert(sizeof(v256) == 32);

union v512 {
    std::array<v256, 2> raw{};
    std::array<u8, 64>  b;
    std::array<u16, 32> w;
    std::array<u32, 16> d;
    std::array<u64, 8>  q;

    using Mask8  = u64;
    using Mask16 = u32;
    using Mask32 = u16;
    using Mask64 = u8;

    forceinline constexpr v512() = default;
    forceinline constexpr explicit v512(std::array<v256, 2> raw) :
        raw(raw){};
    forceinline constexpr explicit v512(std::array<u8, 64> src) :
        b(src) {}
    forceinline constexpr explicit v512(std::array<u16, 32> src) :
        w(src) {}
    forceinline constexpr explicit v512(std::array<u32, 16> src) :
        d(src) {}
    forceinline constexpr explicit v512(std::array<u64, 8> src) :
        q(src) {}

    forceinline auto operator==(const v512& other) const -> bool { return raw == other.raw; }
};
static_assert(sizeof(v512) == 64);

}

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <memory>
#include <cstdint>

#define LUMA_BIT(x) 1 << x

namespace luma {
template <typename T>
using ref = std::shared_ptr<T>;

template <typename T>
using local = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr auto make_ref(Args&&... args) -> ref<T> {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
constexpr auto make_local(Args&&... args) -> local<T> {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

// forward declarations
class window;
class image;
class input;

namespace state {
struct key;
}

namespace buffer {
struct element;
class layout;
class vertex;
class array;
class frame;
}

namespace mesh {
struct vertex;
class surface;
}

// luma types
using f32 = float;          // float 32-bit
using f64 = double;         // float 64-bit

using u8  = std::uint8_t;   // unsigned integer  8-bit
using u16 = std::uint16_t;  // unsinged integer 16-bit
using u32 = std::uint32_t;  // unsigned integer 32-bit
using u64 = std::uint64_t;  // unsinged integer 64-bit

using i8  = std::int8_t;    // integer  8-bit
using i16 = std::int16_t;   // integer 16-bit
using i32 = std::int32_t;   // integer 32-bit
using i64 = std::int64_t;   // integer 64-bit

using usize = std::size_t;     // unsigned arch
using isize = std::ptrdiff_t;  // signed   arch

namespace max {
[[maybe_unused]]constexpr u8  u8  = UINT8_MAX;
[[maybe_unused]]constexpr u16 u16 = UINT16_MAX;
[[maybe_unused]]constexpr u32 u32 = UINT32_MAX;
[[maybe_unused]]constexpr u64 u64 = UINT64_MAX;
[[maybe_unused]]
[[maybe_unused]]constexpr i8  i8  = INT8_MAX;
[[maybe_unused]]constexpr i16 i16 = INT16_MAX;
[[maybe_unused]]constexpr i32 i32 = INT32_MAX;
[[maybe_unused]]constexpr i64 i64 = INT64_MAX;

[[maybe_unused]]constexpr usize usize = SIZE_MAX;
[[maybe_unused]]constexpr isize isize = PTRDIFF_MAX;
}

namespace min {
[[maybe_unused]]constexpr i8  i8  = INT8_MIN;
[[maybe_unused]]constexpr i16 i16 = INT16_MIN;
[[maybe_unused]]constexpr i32 i32 = INT32_MIN;
[[maybe_unused]]constexpr i64 i64 = INT64_MIN;

[[maybe_unused]] constexpr isize isize = PTRDIFF_MIN;
}
}

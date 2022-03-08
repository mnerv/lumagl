#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <memory>

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
}

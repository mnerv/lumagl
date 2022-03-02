#pragma once
#include <cstdint>
#include <array>

namespace luma {
namespace input {
}

namespace state {

constexpr auto STATE_SIZE = 2;
struct key {
    int32_t key;
    std::array<bool, STATE_SIZE> states{false, false};
};

auto is_clicked(key const& key) -> bool;
auto is_press(key const& key) -> bool;
auto is_release(key const& key) -> bool;

}
}


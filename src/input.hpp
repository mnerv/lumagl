#pragma once
#include <memory>
#include <cstdint>
#include <array>

#include "glm/vec2.hpp"

namespace luma {

namespace state {

constexpr auto STATE_SIZE = 2;
struct key {
    int32_t value;
    std::array<bool, STATE_SIZE> states{false, false};

    key(int32_t const& k) : value(k) {}
    auto update(bool const& state) -> void;
};

auto is_clicked(key const& key) -> bool;
auto is_clicked(std::shared_ptr<state::key> const& key) -> bool;

auto is_press(key const& key) -> bool;
auto is_press(std::shared_ptr<state::key> const& key) -> bool;

auto is_release(key const& key) -> bool;
auto is_release(std::shared_ptr<state::key> const& key) -> bool;

auto make_key(int32_t const& key) -> std::shared_ptr<state::key>;

}
}


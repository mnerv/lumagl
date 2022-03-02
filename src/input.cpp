#include "input.hpp"
#include "GLFW/glfw3.h"

namespace luma {

namespace state {
auto key::update(bool const& state) -> void {
    states[1] = states[0];
    states[0] = state;
}

auto is_clicked(key const& key) -> bool {
    return key.states[0] == GLFW_PRESS && key.states[1] == GLFW_RELEASE;
}
auto is_press(key const& key) -> bool {
    return key.states[0] == GLFW_PRESS;
}
auto is_release(key const& key) -> bool {
    return key.states[0] == GLFW_RELEASE;
}

auto is_clicked(std::shared_ptr<state::key> const& key) -> bool {
    return is_clicked(*key);
}
auto is_press(std::shared_ptr<state::key> const& key) -> bool {
    return is_press(*key);
}
auto is_release(std::shared_ptr<state::key> const& key) -> bool {
    return is_release(*key);
}

auto make_key(int32_t const& key) -> std::shared_ptr<state::key> {
    return std::make_shared<state::key>(key);
}

}

}


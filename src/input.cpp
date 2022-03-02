#include "input.hpp"
#include "GLFW/glfw3.h"

namespace luma {

namespace state {
auto is_clicked(key const& key) -> bool {
    return key.states[0] == GLFW_PRESS && key.states[1] == GLFW_RELEASE;
}
auto is_press(key const& key) -> bool {
    return key.states[0] == GLFW_PRESS;
}
auto is_release(key const& key) -> bool {
    return key.states[0] == GLFW_RELEASE;
}
}

}


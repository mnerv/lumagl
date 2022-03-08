#include "camera.hpp"
#include <iostream>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace luma {
camera::camera() {}

auto camera::world_to_view() const -> glm::mat4 {
    return glm::lookAt(position, target, up);
}
auto camera::update_perspective(float const& aspect, float const& fov) -> void {
    m_projection = glm::perspective(fov, aspect, near, far);
}

}


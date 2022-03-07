#include "camera.hpp"
#include <iostream>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace luma {
camera::camera(glm::vec3 const& direction, glm::vec3 const& up)
    : m_direction(direction), m_up(up) {
}

auto camera::world_to_view() const -> glm::mat4 {
    return glm::lookAt(m_position, m_direction, m_up);
}
auto camera::update_perspective(float const& aspect, float const& fov) -> void {
    m_projection = glm::perspective(fov, aspect, near, far);
}

}


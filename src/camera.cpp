#include "camera.hpp"
#include "window.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace luma {

camera::camera(glm::vec3 const& position, glm::vec3 const& front,
               glm::vec3 const& up, float const& near, float const& far)
        : m_position(position), m_front(front), m_up(up), m_near(near), m_far(far) {
    m_view = glm::lookAt(m_position, m_position + m_front, m_up);
}

auto camera::move(glm::vec3 const& position) -> void {
    m_position = position;
    move_delta();
}
auto camera::move_delta(glm::vec3 const& delta) -> void {
    m_position += delta;
    m_target = m_position + m_front;
    m_view = glm::lookAt(m_position, m_target, m_up);
}

auto camera::set_front(glm::vec3 const& front) -> void {
    m_front = front;
    m_target = m_position + m_front;
    m_view = glm::lookAt(m_position, m_position + m_front, m_up);
}
perspective_camera::perspective_camera(float const& fov, glm::vec3 const& position,
                                       glm::vec3 const& front, glm::vec3 const& up)
    : camera(position, front, up), m_fov(fov) {}

auto perspective_camera::update() -> void {
    m_projection = glm::perspective(m_fov, m_width / m_height, m_near, m_far);
}

}


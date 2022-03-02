#include "camera.hpp"
#include "window.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace luma {

camera::camera(glm::vec3 const& position, glm::vec3 const& front,
               glm::vec3 const& up, float const& near, float const& far)
        : m_position(position), m_front(front), m_up(up), m_near(near), m_far(far) {
}

perspective_camera::perspective_camera(float const& fov, glm::vec3 const& position,
                                       glm::vec3 const& front, glm::vec3 const& up)
    : camera(position, front, up), m_fov(fov) {}

auto perspective_camera::update(window const& win) -> void {
    m_view = glm::lookAt(m_position, m_position + m_front, m_up);
    m_projection = glm::perspective(m_fov, float(win.width()) / float(win.height()), m_near, m_far);
}

}

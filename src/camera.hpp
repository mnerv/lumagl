#pragma once

#include <numbers>
#include "glm/glm.hpp"
#include "util.hpp"

namespace luma {
class camera {
  public:
    float near = 0.01f;
    float far  = 1000.0f;

  public:
    camera(glm::vec3 const& direction = {0.0f, 0.0f, 0.0f},
           glm::vec3 const& up        = {0.0f, 1.0f, 0.0f});
    ~camera() = default;

    auto update_perspective(float const& aspect, float const& fov = 45.0f) -> void;
    auto update_orthographic() -> void {};

    // view matrix
    auto world_to_view() const -> glm::mat4;
    auto projection() const -> glm::mat4 const& { return m_projection; }

  private:
    glm::vec3 m_position {-2.0f, 1.0f, -2.0f};
    glm::vec3 m_direction;
    glm::vec3 m_up;

  private:
    glm::mat4 m_projection{1.0f};
};
}


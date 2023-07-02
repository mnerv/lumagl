#pragma once

#include <numbers>
#include "glm/glm.hpp"
#include "util.hpp"

namespace luma {
class camera {
  public:
    float near = 0.1f;
    float far  = 1000.0f;

    glm::vec3 position{0.0f, 0.0f, 1.0f};
    glm::vec3 target  {0.0f, 0.0f, 0.0f};
    glm::vec3 up      {0.0f, 1.0f, 0.0f};

  public:
    camera();
    ~camera() = default;

    auto update_perspective(float const& aspect, float const& fov = 45.0f) -> void;
    auto update_orthographic() -> void {};

    auto front() const -> glm::vec3 { return glm::normalize(glm::cross(up, get_right_vector())); }

    // view matrix
    auto world_to_view() const -> glm::mat4;
    auto projection() const -> glm::mat4 const& { return m_projection; }

    auto get_view_direction() const -> glm::vec3 { return -glm::transpose(world_to_view())[2]; }
    auto get_right_vector() const -> glm::vec3 { return glm::transpose(world_to_view())[0]; }

  private:
    glm::mat4 m_projection{1.0f};
};
}


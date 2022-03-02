#pragma once

#include <numbers>
#include "glm/glm.hpp"
#include "util.hpp"

namespace luma {
class window;

constexpr glm::vec3 default_position{0.f,  0.f,  0.f};
constexpr glm::vec3 default_front   {0.f,  0.f, -1.f};
constexpr glm::vec3 default_up      {0.f,  1.f,  0.f};
constexpr float default_near{0.01f};
constexpr float default_far {500.f};

class camera {
  public:

  public:
    camera(glm::vec3 const& position = default_position,
           glm::vec3 const& front = default_front,
           glm::vec3 const& up = default_up,
           float const& near = default_near,
           float const& far = default_far);
    virtual ~camera() = default;

    virtual auto update(window const& win) -> void = 0;

  public:
    auto set_near(float const& near) -> void { m_near = near; }
    auto set_far(float const& far) -> void { m_far = far; }
    auto set_up(glm::vec3 const& up) -> void { m_up = up; }
    auto set_front(glm::vec3 const& front) -> void { m_front = front; }
    auto set_position(glm::vec3 const& position) -> void { m_position = position; }

    auto view() const -> glm::mat4 { return m_view; }
    auto projection() const -> glm::mat4 { return m_projection; }

  protected:
    glm::mat4 m_view{1.f};
    glm::mat4 m_projection{1.f};

    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;

    float m_near;
    float m_far;
};

class perspective_camera : virtual public camera {
  public:
    perspective_camera(float const& fov = 45.f,
                       glm::vec3 const& position = default_position,
                       glm::vec3 const& front = default_front,
                       glm::vec3 const& up    = default_up);

    auto update(window const& win) -> void override;
  private:
    [[maybe_unused]]float m_fov;
};
}


#pragma once

#include <cstdint>
#include <vector>

#include "luma.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace luma {

namespace mesh {
struct vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 uv;
};

class mesh {
  public:
    mesh(std::vector<vertex> const& vertices, std::vector<uint32_t> indices);
    ~mesh() = default;

    auto vertices() const -> std::vector<vertex> const& { return m_vertices; }
    auto indices() const -> std::vector<uint32_t> const& { return m_indices; }

  private:
    std::vector<vertex> m_vertices;
    std::vector<uint32_t> m_indices;
};

auto plane(float const& length = 1.0f, float const& width = 1.0f) -> ref<mesh>;
auto box(float const& length, float const& width, float const& height) -> ref<mesh>;

}

}


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
    //glm::vec3 normal;
};

class surface {
  public:
    surface(std::vector<vertex> const& vertices, std::vector<uint32_t> indices);
    surface();
    ~surface() = default;

    auto set_vertices(std::vector<vertex> const& vertices) -> void;
    auto set_indices(std::vector<uint32_t> const& indices) -> void;
    auto add_triangle(uint32_t const& offset, uint32_t const& v0, uint32_t const& v1, uint32_t const& v2) -> void;
    auto add_quad(uint32_t const& v0, uint32_t const& v1, uint32_t const& v2, uint32_t const& v3) -> void;

    auto add_vertex(glm::vec3 const& point, glm::vec4 const& color = {0.f, 0.f, 0.f, 1.f},
                    glm::vec2 const& uv = {0.f, 0.f}) -> uint32_t;
    auto vertices() const -> std::vector<vertex> const& { return m_vertices; }
    auto indices() const -> std::vector<uint32_t> const& { return m_indices; }

    auto vertex_size() const -> uint32_t { return sizeof(vertex); }
    auto vertices_size() const -> uint32_t { return vertex_size() * vertex_size(); }
    auto vertex_count() const -> int32_t { return m_vertices.size(); }
    auto index_count() const -> int32_t { return m_indices.size(); }

  private:
    std::vector<vertex> m_vertices;
    std::vector<uint32_t> m_indices;

    std::vector<std::vector<uint32_t>> m_faces;
};

auto plane(int32_t const& resolution = 1) -> ref<surface>;
auto cube() -> ref<surface>;
//auto box(float const& length, float const& width, float const& height) -> ref<mesh>;

}

}


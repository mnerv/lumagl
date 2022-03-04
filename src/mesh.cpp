#include "mesh.hpp"
#include <iostream>
#include <array>

namespace luma {
namespace mesh {

surface::surface(std::vector<vertex> const& vertices, std::vector<uint32_t> indices)
    : m_vertices(vertices), m_indices(indices) {}
surface::surface() : m_vertices({}), m_indices({}) {}

auto surface::set_indices(std::vector<uint32_t> const& indices) -> void {
    m_indices = indices;
}

// https://www.danielsieger.com/blog/2021/05/03/generating-primitive-shapes.html
auto surface::add_triangle(uint32_t const& v0, uint32_t const& v1, uint32_t const& v2) -> void {
    m_indices.push_back(v0);
    m_indices.push_back(v1);
    m_indices.push_back(v2);
}

auto surface::add_vertex(glm::vec3 const& point, glm::vec4 const& color, glm::vec2 const& uv) -> uint32_t {
    mesh::vertex vertex{point, color, uv};
    m_vertices.push_back(vertex);
    return m_vertices.size() - 1;
}

auto plane(int32_t const& resolution) -> ref<surface> {
    auto mesh = make_ref<surface>();
    glm::vec4 color{1.f, 0.f, 1.f, 1.f};
    glm::vec3 point{-1.f, -1.f, 0.f};

    for (auto i = 0; i < resolution + 1; i++) {
        for (auto j = 0; j < resolution + 1; j++) {
            mesh->add_vertex(point, color, {float(j) / resolution, float(i) / resolution});
            point[0] += 2.0 / resolution;

        }
        point[0] = -1.f;
        point[1] += 2.0 / resolution;
    }

    std::vector<uint32_t> triangles{};
    triangles.resize(resolution * resolution * 6);
    uint32_t vert = 0;
    uint32_t tris = 0;
    for (auto i = 0; i < resolution; i++) {
        for (auto j = 0; j < resolution; j++) {
            triangles[tris + 0] = vert + 0;
            triangles[tris + 1] = vert + resolution + 1;
            triangles[tris + 2] = vert + 1;
            triangles[tris + 3] = vert + 1;
            triangles[tris + 4] = vert + resolution + 1;
            triangles[tris + 5] = vert + resolution + 2;

            vert++;
            tris += 6;;
        }
        vert++;
    }
    mesh->set_indices(triangles);
    return mesh;
}

auto cube([[maybe_unused]]float const& resolution) -> ref<surface> {
    std::vector<glm::vec3> positions;
    for (auto i = 0; i < 4; i++) {
        positions.push_back({-1, -1, 0});
    }
    return nullptr;
}

}
}

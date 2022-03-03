#include "mesh.hpp"

namespace luma {
namespace mesh {

mesh::mesh(std::vector<vertex> const& vertices, std::vector<uint32_t> indices)
    : m_vertices(vertices), m_indices(indices) {}

auto plane(float const& length, float const& width) -> ref<mesh> {
    std::vector<vertex> vertices{
        {{-0.5f,  0.5f,  0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.0f}, {1.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
    };
    std::vector<uint32_t> indices{
        0, 1, 2,
        0, 2, 3,
    };
    for (auto& v : vertices) {
        v.position.x *= length;
        v.position.y *= width;
    }
    return luma::make_ref<mesh>(vertices, indices);
}

//auto box(float const& length, float const& width, float const& height) -> ref<mesh> {
//}

}
}

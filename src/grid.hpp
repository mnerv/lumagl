#pragma once

#include "luma.hpp"
#include "shader.hpp"
#include "buffer.hpp"
#include "glm/glm.hpp"

namespace luma {
class grid {
  public:
    enum class plane {
        xz, xy,
        yz, yx,
    };

  public:
    grid(bool const& is_cw = true);
    ~grid() = default;

    auto render(glm::mat4 const& view, glm::mat4 const& projection,
                glm::vec2 const& near_far = {0.01f, 500.f}) const -> void;

  private:
    ref<shader>         m_shader;
    ref<buffer::array>  m_array_buffer;
    ref<buffer::vertex> m_vertex_buffer;
    ref<buffer::index>  m_index_buffer;

  private:
    static float    vertices[];
    static uint32_t cw_indices[];
    static uint32_t ccw_indices[];

    static char const* vertex_shader;
    static char const* fragment_shader;
};
}


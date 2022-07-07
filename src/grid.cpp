#include "grid.hpp"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace luma {

float grid::vertices[]{
    -1.0f, -1.0f,
    -1.0f,  1.0f,
     1.0f,  1.0f,
     1.0f, -1.0f,
};
uint32_t grid::cw_indices[]{
    0, 1, 2,
    0, 2, 3,
};
uint32_t grid::ccw_indices[]{
    0, 2, 1,
    0, 3, 2,
};

char const* grid::vertex_shader   = R"(#version 410 core
layout (location = 0) in vec2 a_position;
#ifdef GL_FRAGMENT_PRECISION_HIGH
  precision highp float;
#else
  precision mediump float;
#endif

layout(location = 0) out vec4 o_color;
out vec3 near;
out vec3 far;

uniform mat4 view;
uniform mat4 projection;

vec3 unproject_point(float x, float y, float z) {
    mat4 inv = inverse(projection * view);
    vec4 unproj_point = inv * vec4(x, y, z, 1.f);
    return unproj_point.xyz / unproj_point.w;
}

void main() {
    vec2 p = a_position;
    near = unproject_point(p.x, p.y, -1.f);
    far  = unproject_point(p.x, p.y,  1.f);
    gl_Position = vec4(a_position, 0.0f, 1.0f);
}
)";

char const* grid::fragment_shader = R"(#version 410 core
#ifdef GL_FRAGMENT_PRECISION_HIGH
  precision highp float;
#else
  precision mediump float;
#endif
layout(location = 0) out vec4 o_color;

in vec3 near;
in vec3 far;

uniform float[2] u_nearfar;
uniform mat4 view;
uniform mat4 projection;

vec4 grid(vec3 point, float scale, bool is_axis) {
    vec2 coord = point.xz * scale;
    vec2 dd    = fwidth(coord);
    vec2 uv    = fract(coord - 0.5) - 0.5;
    vec2 grid  = abs(uv) / dd;  // TODO: figure this out, adjust the line thickness
    float line = min(grid.x, grid.y);
    float min_z = min(dd.y, 1.0);
    float min_x = min(dd.x, 1.0);
    vec4 col    = vec4(0.3);
    col.a       = 1.0 - min(line, 1.0);

    if (-1.0 * min_x < point.x && point.x < 0.1 * min_x && is_axis)
        col.rgb = vec3(0.427, 0.792, 0.909);
    if (-1.0 * min_z < point.z && point.z < 0.1 * min_z && is_axis)
        col.rgb = vec3(0.984, 0.380, 0.490);
    return col;
}

float compute_depth(vec3 point) {
    vec4 clip_space = projection * view * vec4(point, 1.0);
    float clip_space_depth = clip_space.z / clip_space.w;
    float far  = gl_DepthRange.far;
    float near = gl_DepthRange.near;
    float depth = (((far - near) * clip_space_depth) + near + far) / 2.0;
    return depth;
}

float compute_fade(vec3 point) {
    vec4 clip_space = projection * view * vec4(point, 1.0);
    float clip_space_depth = (clip_space.z / clip_space.w) * 2.0 - 1.0;
    float near = u_nearfar[0];
    float far  = u_nearfar[1];
    float linear_depth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near));
    return linear_depth / far;
}

void main() {
    float t = -near.y / (far.y - near.y);
    vec3  R =  near + t * (far - near);
    float is_on = float(t > 0);

    //float fade = max(0, 1.0 - compute_fade(R));
    float fade = smoothstep(0.04, 0.0, compute_fade(R));
    o_color = grid(R, 1, true);
    //o_color += grid(R, 10, false) * 0.25;
    o_color *= fade;

    o_color *= is_on;
    gl_FragDepth = compute_depth(R);
}
)";

grid::grid(bool const& is_cw) {
    m_shader        = shader::create(vertex_shader, fragment_shader);
    m_array_buffer  = buffer::array::create();
    m_vertex_buffer = buffer::vertex::create(grid::vertices, sizeof(grid::vertices));
    m_vertex_buffer->set_layout({{
        {shader::type::vec2, "a_position"}
    }});
    if (is_cw)
        m_index_buffer = buffer::index::create(grid::cw_indices,  sizeof(grid::cw_indices) / sizeof(uint32_t));
    else
        m_index_buffer = buffer::index::create(grid::ccw_indices, sizeof(grid::cw_indices) / sizeof(uint32_t));
    m_array_buffer->add_vertex_buffer(m_vertex_buffer);
    m_array_buffer->set_index_buffer(m_index_buffer);
}

auto grid::render(glm::mat4 const& view, glm::mat4 const& projection, glm::vec2 const& near_far) const -> void {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    m_shader->bind();
    m_shader->num("u_nearfar", 2, glm::value_ptr(near_far));
    m_shader->mat4("view", glm::value_ptr(view));
    m_shader->mat4("projection", glm::value_ptr(projection));

    m_array_buffer->bind();
    m_vertex_buffer->bind();
    m_index_buffer->bind();
    glDrawElements(GL_TRIANGLES, m_index_buffer->count(), GL_UNSIGNED_INT, 0);
}
}


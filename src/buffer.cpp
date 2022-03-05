#include "buffer.hpp"
#include "glad/glad.h"
#include "mesh.hpp"

namespace luma {
namespace buffer {

layout::layout(std::vector<element> const& elements) : m_elements(elements) {
    m_stride = calculate_stride();
    calculate_offset();
}

auto layout::calculate_stride() -> std::size_t {
    std::size_t strides = 0;
    for (auto const& e : m_elements)
        strides += element::shader_type_size(e.type);
    return strides;
}

auto layout::calculate_offset() -> void {
    uint32_t offset = 0;
    for (std::size_t i = 0; i < m_elements.size(); i++) {
        m_elements[i].offset = offset;
        offset += element::shader_type_size(m_elements[i].type);
    }
}

vertex::vertex(void const* vertices, uint32_t const& size) : m_layout({}){
    m_id = create_buffer();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
vertex::vertex(std::vector<mesh::vertex> const& vertices) : m_layout({}) {
    m_id = create_buffer();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(luma::mesh::vertex), vertices.data(), GL_STATIC_DRAW);
}
vertex::~vertex() {
    glDeleteBuffers(1, &m_id);
}
auto vertex::bind() const -> void {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

auto vertex::create_buffer() const -> uint32_t {
    uint32_t id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    return id;
}

auto vertex::create(void const* vertices, uint32_t const& size) -> ref<vertex> {
    return make_ref<vertex>(vertices, size);
}

index::index(uint32_t const* indices, uint32_t const& count) : m_count(count) {
    m_id = create_buffer();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}
index::index(std::vector<uint32_t> const& indices) {
    m_id = create_buffer();
    m_count = indices.size();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
}

index::~index() {
    glDeleteBuffers(1, &m_id);
}
auto index::bind() const -> void {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

auto index::create(uint32_t const* indices, uint32_t const& count) -> ref<index> {
    return make_ref<index>(indices, count);
}

auto index::create_buffer() -> uint32_t {
    uint32_t id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    return id;
}

frame::frame() {
    glGenFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}
frame::~frame() {
    glDeleteFramebuffers(1, &m_id);
}

auto frame::bind() const -> void {
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}
auto frame::unbind() const -> void {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

array::array() {
    glGenVertexArrays(1, &m_id);
    glBindVertexArray(m_id);
}
array::~array() {
    glDeleteVertexArrays(1, &m_id);
}
auto array::bind() const -> void {
    glBindVertexArray(m_id);
}
auto array::create() -> ref<array> {
    return make_ref<array>();
}

auto array::add_vertex_buffer(ref<vertex> const& vertex_buffer) -> void {
    glBindVertexArray(m_id);
    vertex_buffer->bind();
    auto const& layout = vertex_buffer->get_layout();
    auto stride = layout.get_stride();

    std::for_each(std::begin(layout), std::end(layout), [&](element const& e) {
        auto size   = element::component_count(e.type);
        auto offset = e.offset;
        switch(e.type) {
            case shader::type::f32:
            case shader::type::vec2:
            case shader::type::vec3:
            case shader::type::vec4:
            case shader::type::mat2:
            case shader::type::mat3:
            case shader::type::mat4:
                glVertexAttribPointer(m_vertex_buffer_index, size, GL_FLOAT, e.normalised ? GL_TRUE : GL_FALSE, stride, (void const*)(intptr_t)offset);
                glEnableVertexAttribArray(m_vertex_buffer_index);
                m_vertex_buffer_index++;
                break;
            default: break;
        }
    });

    m_vertex_buffers.push_back(vertex_buffer);
}

auto array::set_index_buffer(ref<index> const& index_buffer) -> void {
    glBindVertexArray(m_id);
    m_index_buffer = index_buffer;
    m_index_buffer->bind();
}

}}

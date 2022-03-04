#include "buffer.hpp"
#include "glad/glad.h"
#include "mesh.hpp"

namespace luma {
namespace buffer {

vertex::vertex(float const* vertices, uint32_t const& size) {
    m_id = create_buffer();
    m_count = size / sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
vertex::vertex(std::vector<mesh::vertex> const& vertices) {
    m_count = vertices.size();
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

render::render(int32_t const& width, int32_t const& height)
    : m_width(width), m_height(height) {
    glGenRenderbuffers(1, &m_id);
    glBindRenderbuffer(GL_RENDERBUFFER, m_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_id);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    //    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
render::~render() {
    glDeleteRenderbuffers(1, &m_id);
}

auto render::resize(int32_t const& width, int32_t const& height) -> void {
    if (!(width != m_width || height != m_height)) return;
    glDeleteRenderbuffers(1, &m_id);

    glGenRenderbuffers(1, &m_id);
    glBindRenderbuffer(GL_RENDERBUFFER, m_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_id);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    //    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

auto render::bind() const -> void {
    glBindRenderbuffer(GL_RENDERBUFFER, m_id);
}

auto render::unbind() const -> void {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

}}

#include "buffer.hpp"
#include "glad/glad.h"

namespace luma {
namespace buffer {

vertex::vertex(void const* vertices, uint32_t const& size) {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
vertex::~vertex() {
    glDeleteBuffers(1, &m_id);
}
auto vertex::bind() const -> void {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
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
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

index::~index() {
    glDeleteBuffers(1, &m_id);
}
auto index::bind() const -> void {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
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

render::render(int32_t const& width, int32_t const& height) {
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
render::~render() {
    glDeleteRenderbuffers(1, &m_id);
}

auto render::bind() const -> void {
    glBindRenderbuffer(GL_RENDERBUFFER, m_id);
}

auto render::unbind() const -> void {
}

}}

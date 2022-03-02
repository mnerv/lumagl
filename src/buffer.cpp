#include "buffer.hpp"

namespace luma {
namespace buffer {

vertex::vertex(void const* vertices, size_t const& size) {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
vertex::~vertex() {
    glDeleteBuffers(1, &m_id);
}

array::array() {
    glGenVertexArrays(1, &m_id);
    glBindVertexArray(m_id);
}
array::~array() {
    glDeleteVertexArrays(1, &m_id);
}

index::index(uint32_t const* indices, size_t const& count) : m_count(count) {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

index::~index() {
    glDeleteBuffers(1, &m_id);
}

}}

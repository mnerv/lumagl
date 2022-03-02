#pragma once

#include <cstdint>

#include "glad/glad.h"

namespace luma {
namespace buffer {

class vertex {
  public:
    vertex(void const* vertices, uint32_t const& size);
    ~vertex();

    auto bind() -> void { glBindBuffer(GL_ARRAY_BUFFER, m_id); }

  private:
    uint32_t m_id;
};

class array {
  public:
    array();
    ~array();

    auto bind() -> void { glBindVertexArray(m_id); }

  private:
    uint32_t m_id;
};

class index {
  public:
    index(uint32_t const* indices, uint32_t const& count);
    ~index();

    auto bind() -> void { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); }
    auto count() -> uint32_t { return m_count; }

  private:
    uint32_t m_id;
    uint32_t m_count;
};

}}

#pragma once

#include <cstdint>
#include <vector>

#include "luma.hpp"

namespace luma {

// forward declaration
namespace mesh {
struct vertex;
}

namespace buffer {

class vertex {
  public:
    vertex(void const* vertices, uint32_t const& size);
    vertex(std::vector<mesh::vertex> const& vertices);
    ~vertex();

    auto bind() const -> void;

  private:
    auto create_buffer() const -> uint32_t;

  private:
    uint32_t m_id;
};

class array {
  public:
    array();
    ~array();

    auto bind() const -> void;

  private:
    uint32_t m_id;
};

class index {
  public:
    index(uint32_t const* indices, uint32_t const& count);
    index(std::vector<uint32_t> const& indices);
    ~index();

    auto bind() const -> void;
    auto count() -> uint32_t { return m_count; }

  private:
    auto create_buffer() -> uint32_t;

  private:
    uint32_t m_id;
    uint32_t m_count;
};

class frame {
  public:
    frame();
    ~frame();

    auto get_id() const -> int32_t { return m_id; }
    auto bind() const -> void;
    auto unbind() const -> void;

  private:
    uint32_t m_id;
};

class render {
  public:
    render(int32_t const& width, int32_t const& height);
    ~render();

    auto resize(int32_t const& width, int32_t const& height) -> void;
    auto get_id() const -> int32_t { return m_id; }
    auto bind() const -> void;
    auto unbind() const -> void;

  private:
    uint32_t m_id;
    int32_t m_width;
    int32_t m_height;
};

}}

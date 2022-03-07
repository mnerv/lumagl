#pragma once

#include <string>
#include <cstdint>
#include <vector>

#include "luma.hpp"
#include "shader.hpp"

namespace luma {

namespace buffer {

struct element {
    shader::type type;
    std::string  name;
    bool         normalised = false;
    uint32_t     offset     = 0;

    inline static auto shader_type_size(shader::type const& type) -> int32_t {
        switch(type) {
            case shader::type::i8:
            case shader::type::u8:  return 1;
            case shader::type::i16:
            case shader::type::u16:
            case shader::type::f16: return 2;
            case shader::type::i32:
            case shader::type::u32:
            case shader::type::p32:
            case shader::type::f32: return 4;
            case shader::type::f64: return 8;

            case shader::type::ivec2:
            case shader::type::vec2: return 4 * 2;
            case shader::type::ivec3:
            case shader::type::vec3: return 4 * 3;
            case shader::type::ivec4:
            case shader::type::vec4: return 4 * 4;

            case shader::type::dvec2: return 8 * 2;
            case shader::type::dvec3: return 8 * 3;
            case shader::type::dvec4: return 8 * 4;

            case shader::type::mat2: return 4 * 2 * 2;
            case shader::type::mat3: return 4 * 3 * 3;
            case shader::type::mat4: return 4 * 4 * 4;
            default: return 0;
        }
    }

    inline static auto component_count(shader::type const& type) -> int32_t {
        switch(type) {
            case shader::type::vec2:
            case shader::type::ivec2:
            case shader::type::dvec2: return 2;

            case shader::type::vec3:
            case shader::type::ivec3:
            case shader::type::dvec3: return 3;

            case shader::type::vec4:
            case shader::type::ivec4:
            case shader::type::dvec4: return 4;

            case shader::type::mat2: return 2 * 2;
            case shader::type::mat3: return 3 * 3;
            case shader::type::mat4: return 4 * 4;
            default: return 1;
        }
    }
};

class layout {
  public:
    layout(std::initializer_list<element> const& elements);
    layout(std::vector<element> const& elements);
    ~layout() = default;

    auto get_stride() const -> std::size_t { return m_stride; }
    auto get_elements() const -> std::vector<element> const& { return m_elements; }

    auto begin() -> std::vector<element>::iterator { return m_elements.begin(); }
    auto end() -> std::vector<element>::iterator { return m_elements.end(); }
    auto begin() const -> std::vector<element>::const_iterator { return m_elements.begin(); }
    auto end() const -> std::vector<element>::const_iterator { return m_elements.end(); }

  private:
    auto calculate_stride() -> std::size_t;
    auto calculate_offset() -> void;

  private:
    std::size_t          m_stride;
    std::vector<element> m_elements;
};

class vertex {
  public:
    vertex(void const* vertices, uint32_t const& size);
    vertex(std::vector<mesh::vertex> const& vertices);
    ~vertex();

    auto get_layout() const -> layout const& { return m_layout; }
    auto set_layout(layout const& layout) -> void { m_layout = layout; }
    auto bind() const -> void;

    static auto create(void const* vertices, uint32_t const& size) -> ref<vertex>;
  private:
    auto create_buffer() const -> uint32_t;

  private:
    uint32_t m_id;
    layout m_layout;
};

class index {
  public:
    index(uint32_t const* indices, uint32_t const& count);
    index(std::vector<uint32_t> const& indices);
    ~index();

    auto bind() const -> void;
    auto count() -> uint32_t { return m_count; }

    static auto create(uint32_t const* indices, uint32_t const& count) -> ref<index>;
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

class array {
  public:
    array();
    ~array();

    auto bind() const -> void;

    auto add_vertex_buffer(ref<vertex> const& vertex_buffer) -> void;
    auto set_index_buffer(ref<index> const& index_buffer) -> void;

    static auto create() -> ref<array>;
  private:
    uint32_t m_id;
    uint32_t m_vertex_buffer_index = 0;
    std::vector<ref<vertex>> m_vertex_buffers;
    ref<index>               m_index_buffer;
};

}}

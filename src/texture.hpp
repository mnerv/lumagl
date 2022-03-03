#pragma once
#include <cstdint>
#include <string>

#include "luma.hpp"
#include "image.hpp"
#include "buffer.hpp"

namespace luma {

class texture {
  public:
    texture(std::string const& filename, bool const& mipmap = true);
    texture(int32_t const& width, int32_t const& height, int32_t const& channels = 4);
    ~texture();

    auto framebuffer(ref<buffer::frame> const& framebuffer) -> void;
    auto resize(int32_t const& width, int32_t const& height) -> void;
    auto get_image() const -> ref<image> { return m_image; }
    auto id() const -> uint32_t { return m_id; }
    auto generate_mipmap() const -> void;
    auto bind(uint32_t const& id = 0) -> void;
    auto unbind() const -> void;

  private:
    auto create_texture() -> uint32_t;

  private:
    uint32_t   m_id;
    ref<image> m_image;
};

}


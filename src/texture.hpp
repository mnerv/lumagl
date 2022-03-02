#pragma once
#include <cstdint>

#include "image.hpp"
#include "buffer.hpp"

namespace luma {

class texture {
  public:
    texture(image const& image);
    ~texture();

    auto bind(uint32_t const& id = 0) -> void;
    auto unbind() const -> void;

  private:
    uint32_t m_id;
    image m_image;
};

}


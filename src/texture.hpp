#pragma once
#include <cstdint>

#include "image.hpp"

namespace luma {

class texture {
  public:
    texture(image const& img);
    ~texture();

    auto bind(uint32_t const& id = 0) -> void;

  private:
    uint32_t m_id;
};

}


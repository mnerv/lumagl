#pragma once

#include <string>
#include <cstdint>

namespace luma {
class image {
  public:
    image(std::string const& filename, int32_t const& channel = 0);
    ~image();

    auto buffer() -> uint8_t* { return m_buffer; }
    auto width() -> int32_t { return m_width; }
    auto height() -> int32_t { return m_height; }
    auto channels() -> int32_t { return m_channels; }

    auto info() -> std::string {
        return std::string("luma::image{width: ") + std::to_string(m_width) 
               + ", height: "   + std::to_string(m_height)
               + ", channels: " + std::to_string(m_channels) + "}";
    }

  private:
    std::string m_filename;
    int32_t     m_width;
    int32_t     m_height;
    int32_t     m_channels;
    uint8_t*    m_buffer;
};
}


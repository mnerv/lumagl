#pragma once

#include <string>
#include <cstdint>

namespace luma {
class image {
  public:
    image(std::string const& filename, int32_t const& channel = 0, bool const& flip = true);
    image(int32_t const& width, int32_t const& height, int32_t const& channels = 3);
    //image(image const& img);
    ~image();

    auto buffer() const -> uint8_t* { return m_buffer; }
    auto width() const -> int32_t { return m_width; }
    auto height() const -> int32_t { return m_height; }
    auto channels() const -> int32_t { return m_channels; }

    auto info() const -> std::string {
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
    bool        m_is_loaded;
};
}


#include "image.hpp"
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace luma {

image::image(std::string const& filename, int32_t const& channel, bool const& flip)
    : m_filename(filename), m_width(0), m_height(0), m_channels(channel), m_is_loaded(true) {
    stbi_set_flip_vertically_on_load(flip);
    m_buffer = stbi_load(m_filename.c_str(), &m_width, &m_height, &m_channels, 0);
}

image::image(int32_t const& width, int32_t const& height, int32_t const& channels)
    : m_width(width), m_height(height), m_channels(channels), m_is_loaded(false) {
    m_buffer = new uint8_t[m_width * m_height * m_channels];
}

image::~image() {
    if (m_is_loaded) stbi_image_free(m_buffer);
    else delete[] m_buffer;
}

}

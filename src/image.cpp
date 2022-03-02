#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace luma {

image::image(std::string const& filename, int32_t const& channel, bool const& flip)
    : m_filename(filename), m_width(0), m_height(0), m_channels(channel) {
    stbi_set_flip_vertically_on_load(flip);
    m_buffer = stbi_load(m_filename.c_str(), &m_width, &m_height, &m_channels, 0);
}

image::~image() {
    stbi_image_free(m_buffer);
}

}

#include "texture.hpp"
#include "glad/glad.h"

#include <iostream>

namespace luma {
texture::texture(image const& img) : m_image(img) {
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    // Set texture wrapping/filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    uint32_t format = m_image.channels() == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_image.width(), m_image.height(), 0, format, GL_UNSIGNED_BYTE, m_image.buffer());
    glGenerateMipmap(GL_TEXTURE_2D);
}

texture::~texture() {
    glDeleteTextures(1, &m_id);
}

auto texture::bind(uint32_t const& id) -> void {
    glActiveTexture(GL_TEXTURE0 + id);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

auto texture::unbind() const -> void {
    glBindTexture(GL_TEXTURE_2D, 0);
}
}


#include "texture.hpp"
#include "glad/glad.h"

#include <iostream>

namespace luma {
texture::texture(std::string const& filename, bool const& mipmap) {
    m_image = make_ref<image>(filename);
    m_id = create_texture();
    glBindTexture(GL_TEXTURE_2D, m_id);
    if (mipmap) generate_mipmap();
}
texture::texture(int32_t const& width, int32_t const& height, int32_t const& channels) {
    m_image = make_ref<image>(width, height, channels);
    m_id = create_texture();
}

texture::~texture() {
    glDeleteTextures(1, &m_id);
}

auto texture::framebuffer(ref<buffer::frame> const& framebuffer) -> void {
    framebuffer->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_id, 0);
}
auto texture::resize(int32_t const& width, int32_t const& height) -> void {
    if (!(width != m_image->width() || height != m_image->height())) return;
    m_image = make_ref<image>(width, height, m_image->channels());
    glDeleteTextures(1, &m_id);
    m_id = create_texture();
}

auto texture::generate_mipmap() const -> void {
    // Set texture wrapping/filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
}

auto texture::bind(uint32_t const& id) -> void {
    glActiveTexture(GL_TEXTURE0 + id);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

auto texture::unbind() const -> void {
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto texture::create_texture() -> uint32_t {
    uint32_t id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    uint32_t format = m_image->channels() == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_image->width(), m_image->height(), 0, format, GL_UNSIGNED_BYTE, m_image->buffer());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return id;
}
}


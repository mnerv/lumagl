#pragma once
#include <string>
#include <cstdint>
#include <iostream>

#include "glad/glad.h"

namespace luma {
class shader {
  public:
    shader(std::string const& vertex, std::string const& fragment);
    ~shader();

    auto bind() -> void;

    auto set_uniform4f(std::string const& name,
                       float const& v0, float const& v1,
                       float const& v2, float const& v3) -> void;

  private:
    auto compile(uint32_t type, char const* source) -> uint32_t;
    auto link(uint32_t const& vs, uint32_t const& fs) -> uint32_t;

    auto uniform_location(std::string const& name) -> uint32_t;

  private:
    uint32_t m_id;
};
}

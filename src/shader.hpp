#pragma once
#include <string>
#include <cstdint>
#include <iostream>

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace luma {

class shader {
  public:
    shader(std::string const& vertex, std::string const& fragment);
    ~shader();

    auto bind() -> void;

  public:
    auto uniform1i(std::string const& name, int32_t const& value) -> void;

    auto uniform4f(std::string const& name, glm::vec4 const& value) -> void;
    auto uniform4fv(std::string const& name, float const* value, uint32_t const& count = 1) -> void;

    auto uniform_m4(std::string const& name, float const* m4,
                    uint32_t const& count = 1, bool const& transpose = false) -> void;

  private:
    auto compile(uint32_t type, char const* source) -> uint32_t;
    auto link(uint32_t const& vs, uint32_t const& fs) -> uint32_t;

    auto uniform_location(std::string const& name) -> uint32_t;

  private:
    uint32_t m_id;
};

}

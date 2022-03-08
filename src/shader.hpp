#pragma once
#include <string>
#include <cstdint>
#include <iostream>

#include "luma.hpp"
#include "glad/glad.h"
#include "glm/glm.hpp"

namespace luma {

// https://www.khronos.org/opengl/wiki/OpenGL_Type

class shader {
  public:
    enum class type : uint32_t {
        none = 0,
        // basic types
        boolean,
        i8,  u8,
        i16, u16,
        i32, u32, p32, // packed 32-bit integer 16.16 integer
        f16, f32, f64,

        // vector types (f32, i32, f64)
        vec2, vec3, vec4,
        ivec2, ivec3, ivec4,
        dvec2, dvec3, dvec4,

        // matrix types (f32)
        mat2, mat3, mat4,
    };

  public:
    shader(std::string const& vertex, std::string const& fragment);
    ~shader();

    auto bind() -> void;

    static auto create(std::string const& vertex, std::string const& fragment) -> ref<shader>;

  public:
    auto num(std::string const& name, uint32_t const& value) -> void;
    auto num(std::string const& name, int32_t const& value) -> void;
    auto num(std::string const& name, float const& value) -> void;
    auto num(std::string const& name, uint32_t const& count, float const* value) -> void;

    auto vec4(std::string const& name, glm::vec4 const& value) -> void;
    auto vec4(std::string const& name, float const* value, uint32_t const& count = 1) -> void;

    auto mat4(std::string const& name, float const* m4,
                    uint32_t const& count = 1, bool const& transpose = false) -> void;

  private:
    auto compile(uint32_t type, char const* source) -> uint32_t;
    auto link(uint32_t const& vs, uint32_t const& fs) -> uint32_t;

    auto uniform_location(std::string const& name) -> uint32_t;

  private:
    uint32_t m_id;
};

}

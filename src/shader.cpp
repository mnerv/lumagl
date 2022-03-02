#include "shader.hpp"

namespace luma {
shader::shader(std::string const& vertex, std::string const& fragment) {
    auto vs = compile(GL_VERTEX_SHADER,   vertex.c_str());
    auto fs = compile(GL_FRAGMENT_SHADER, fragment.c_str());
    m_id = link(vs, fs);
}
shader::~shader() {
    glDeleteProgram(m_id);
}

auto shader::bind() -> void { glUseProgram(m_id); }

auto shader::set_uniform4f(std::string const& name,
                   float const& v0, float const& v1,
                   float const& v2, float const& v3) -> void {
    bind();
    glUniform4f(uniform_location(name), v0, v1, v2, v3);
}

auto shader::compile(uint32_t type, char const* source) -> uint32_t {
    uint32_t shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    int32_t success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        std::cerr << "ERROR::"<< (type == GL_VERTEX_SHADER ? "VERTEX_" : "FRAGMENT_")
                  << "SHADER::COMPILE_FAILED\n" << info_log << '\n';
        throw std::runtime_error("Shader compilation error");
    }

    return shader;
}

auto shader::link(uint32_t const& vs, uint32_t const& fs) -> uint32_t {
    uint32_t program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int32_t success;
    char info_log[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, info_log);
        std::cerr << "ERROR::SHADER::LINK\n"
                  << info_log << '\n';
        throw std::runtime_error("Shader linking error");
    }
    glUseProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

auto shader::uniform_location(std::string const& name) -> uint32_t {
    return glGetUniformLocation(m_id, name.c_str());
}
}

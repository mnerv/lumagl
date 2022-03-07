#include "shader.hpp"

namespace luma {
auto shader::create(std::string const& vertex, std::string const& fragment) -> ref<shader> {
    return make_ref<shader>(vertex, fragment);
}

shader::shader(std::string const& vertex, std::string const& fragment) {
    auto vs = compile(GL_VERTEX_SHADER,   vertex.c_str());
    auto fs = compile(GL_FRAGMENT_SHADER, fragment.c_str());
    m_id = link(vs, fs);
}
shader::~shader() {
    glDeleteProgram(m_id);
}

auto shader::bind() -> void { glUseProgram(m_id); }

auto shader::num(std::string const& name, uint32_t const& value) -> void {
    bind();
    glUniform1ui(uniform_location(name), value);
}
auto shader::num(std::string const& name, int32_t const& value) -> void {
    bind();
    glUniform1i(uniform_location(name), value);
}
auto shader::num(std::string const& name, float const& value) -> void {
    bind();
    glUniform1f(uniform_location(name), value);
}
auto shader::num(std::string const& name, uint32_t const& count, float const* value) -> void {
    bind();
    glUniform1fv(uniform_location(name), count, value);
}

auto shader::vec4(std::string const& name, glm::vec4 const& value) -> void {
    bind();
    glUniform4f(uniform_location(name), value[0], value[1], value[2], value[3]);
}
auto shader::vec4(std::string const& name, float const* value, uint32_t const& count) -> void {
    bind();
    glUniform4fv(uniform_location(name), count, value);
}

auto shader::mat4(std::string const& name, float const* m4,
                uint32_t const& count, bool const& transpose) -> void {
    bind();
    glUniformMatrix4fv(uniform_location(name), count, (transpose ? GL_TRUE : GL_FALSE), m4);
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

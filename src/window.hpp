#pragma once
#include <string>
#include <cstdint>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace luma {
class window{
  public:
    inline static auto GLSL_VERSION = "#version 410";

  public:
    window(std::string const& name = "helloworld", int32_t const& width = 640, int32_t const& height = 480);
    ~window();

    auto swap() -> void;
    auto poll() -> void;
    auto get_native() -> GLFWwindow*;
    auto should_close() -> bool;
    auto get_key(int32_t key) -> int32_t;

    auto width() const -> int32_t { return m_data.width; }
    auto height() const -> int32_t { return m_data.height; }

  private:
    GLFWwindow* m_window;

    struct data {
        int32_t width;
        int32_t height;
        int32_t x;
        int32_t y;
    };
    data m_data;
};
}


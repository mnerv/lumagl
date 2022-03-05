#pragma once
#include <string>
#include <cstdint>
#include <array>
#include <vector>
#include <unordered_map>

#include "luma.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace luma {
constexpr int32_t DONT_CARE = INT32_MIN;

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
    auto make_key(int32_t key) -> std::shared_ptr<state::key>;

    auto update_key(state::key& key) -> void;
    auto update_key(std::shared_ptr<state::key> const& key) -> void;
    auto update_keys(std::vector<std::shared_ptr<luma::state::key>> const& keys) -> void;

    auto position(int32_t const& x = DONT_CARE, int32_t const& y = DONT_CARE) -> void;
    auto width() const -> int32_t { return m_data.width; }
    auto height() const -> int32_t { return m_data.height; }

  private:
    GLFWwindow* m_window;

    struct data {
        int32_t width;
        int32_t height;
        int32_t x;
        int32_t y;
        std::unordered_map<int32_t, std::shared_ptr<state::key>> keys;
    };
    data m_data;
};
}


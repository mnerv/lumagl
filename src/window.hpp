#pragma once
#include <string>
#include <cstdint>
#include <array>
#include <vector>
#include <unordered_map>

#include "luma.hpp"
#include "event.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace luma {
constexpr i32 DONT_CARE = min::i32;

class window{
  public:
    inline static auto GLSL_VERSION = "#version 410";
    using event_fn = std::function<void(event const&)>;

  public:
    window(std::string const& name = "luma", int32_t const& width = 640, int32_t const& height = 480);
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
    auto width()  const -> int32_t { return m_data.width; }
    auto height() const -> int32_t { return m_data.height; }
    auto buffer_width()  const -> int32_t { return m_data.buffer_width; }
    auto buffer_height() const -> int32_t { return m_data.buffer_height; }

    auto add_event_listener(event::type const& type, event_fn const& callback) -> void;
    // FIXME: Can't check for equality with std::function
    //        Find a way to fix this. A solution might be
    //        to return an id when adding the listener.
    auto remove_event_listener(event::type const& type, event_fn const& callback) -> void;

  private:
    GLFWwindow* m_window;

    struct data {
        int32_t buffer_width;
        int32_t buffer_height;
        int32_t width;
        int32_t height;
        int32_t x;
        int32_t y;

        std::unordered_map<int32_t, std::shared_ptr<state::key>> keys;
        std::unordered_map<event::type, std::vector<event_fn>> events;
    };
    data m_data;
};
}


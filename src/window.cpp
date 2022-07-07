#include "window.hpp"
#include "input.hpp"

#include <algorithm>
#include <iostream>

namespace luma {
static auto setup_opengl() -> void {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

window::window(std::string const& name, int32_t const& width, int32_t const& height) {
    if (!glfwInit()) throw std::runtime_error("error initialising glfw");
    m_data.width  = width;
    m_data.height = height;
    setup_opengl();

    m_window = glfwCreateWindow(m_data.width, m_data.height, name.c_str(), nullptr, nullptr);
    if (!m_window)
        throw std::runtime_error("Failed to create window");

    glfwGetWindowPos(m_window, &m_data.x, &m_data.y);
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
        throw std::runtime_error("Failed to initialize GLAD");

    glfwSetWindowUserPointer(m_window, &m_data);
    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int32_t width, int32_t height) {
        auto data = static_cast<window::data*>(glfwGetWindowUserPointer(window));
        data->width  = width;
        data->height = height;

        auto it = data->events.find(event::type::window_resize);
        if (it == data->events.end()) return;
        auto fns = it->second;
        auto event = window_resize_event(width, height);
        std::for_each(std::begin(fns), std::end(fns), [&](auto const& fn) {
            fn(event);
        });
    });
    glfwSetFramebufferSizeCallback(m_window,
    [](GLFWwindow* window, int32_t width, int32_t height) {
        auto data = static_cast<window::data*>(glfwGetWindowUserPointer(window));
        data->buffer_width  = width;
        data->buffer_height = height;

        auto it = data->events.find(event::type::buffer_resize);
        if (it == data->events.end()) return;
        auto fns = it->second;
        auto event = buffer_resize_event(width, height);
        std::for_each(std::begin(fns), std::end(fns), [&](auto const& fn) {
            fn(event);
        });
    });
    glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int32_t xpos, int32_t ypos){
        auto data = static_cast<window::data*>(glfwGetWindowUserPointer(window));
        data->x = xpos;
        data->y = ypos;

        auto it = data->events.find(event::type::window_move);
        if (it == data->events.end()) return;
        auto fns = it->second;
        auto event = window_move_event(xpos, ypos);
        std::for_each(std::begin(fns), std::end(fns), [&](auto const& fn) {
            fn(event);
        });
    });
    glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int32_t focused) {
        auto data = static_cast<window::data*>(glfwGetWindowUserPointer(window));

        auto it = data->events.find(event::type::window_focus);
        if (it == data->events.end()) return;
        auto fns = it->second;
        auto event = window_focus_event(focused);
        std::for_each(std::begin(fns), std::end(fns), [&](auto const& fn) {
            fn(event);
        });
    });
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
        auto data = static_cast<window::data*>(glfwGetWindowUserPointer(window));

        auto it = data->events.find(event::type::mouse_move);
        if (it == data->events.end()) return;
        auto fns = it->second;
        auto event = mouse_move_event(xpos, ypos);
        std::for_each(std::begin(fns), std::end(fns), [&](auto const& fn) {
            fn(event);
        });
    });
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int32_t button, int32_t action, int32_t mods) {
        auto data = static_cast<window::data*>(glfwGetWindowUserPointer(window));
        double pos_x, pos_y;
        glfwGetCursorPos(window, &pos_x, &pos_y);

        if (action == GLFW_PRESS) {
            auto it = data->events.find(event::type::mouse_press);
            if (it == data->events.end()) return;
            auto fns = it->second;

            auto event = mouse_press_event(button, mods, pos_x, pos_y);
            std::for_each(std::begin(fns), std::end(fns), [&](auto const& fn) {
                fn(event);
            });
        } else {
            auto it = data->events.find(event::type::mouse_release);
            if (it == data->events.end()) return;
            auto fns = it->second;

            auto event = mouse_release_event(button, mods, pos_x, pos_y);
            std::for_each(std::begin(fns), std::end(fns), [&](auto const& fn) {
                fn(event);
            });
        }
    });
    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset){
        auto data = static_cast<window::data*>(glfwGetWindowUserPointer(window));

        auto it = data->events.find(event::type::mouse_wheel);
        if (it == data->events.end()) return;
        auto fns = it->second;
        auto event = mouse_wheel_event(xoffset, yoffset);
        std::for_each(std::begin(fns), std::end(fns), [&](auto const& fn) {
            fn(event);
        });
    });
    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int32_t key, int32_t code, int32_t action, int32_t mods) {
        auto data = static_cast<window::data*>(glfwGetWindowUserPointer(window));

        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            auto it = data->events.find(event::type::key_down);
            if (it == data->events.end()) return;
            auto fns = it->second;

            auto event = key_down_event(key, code, mods, action == GLFW_REPEAT);
            std::for_each(std::begin(fns), std::end(fns), [&](auto const& fn) {
                fn(event);
            });
        } else {
            auto it = data->events.find(event::type::key_up);
            if (it == data->events.end()) return;
            auto fns = it->second;

            auto event = key_up_event(key, code, mods);
            std::for_each(std::begin(fns), std::end(fns), [&](auto const& fn) {
                fn(event);
            });
        }
    });
    glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint) {
        auto data = static_cast<window::data*>(glfwGetWindowUserPointer(window));
        auto it = data->events.find(event::type::key_typed);
        if (it == data->events.end()) return;
        auto fns = it->second;

        auto event = key_typed(codepoint);
        std::for_each(std::begin(fns), std::end(fns), [&](auto const& fn) {
            fn(event);
        });
    });

    glfwGetWindowSize(m_window, &m_data.width, &m_data.height);
}
window::~window() {
    glfwTerminate();
}

auto window::swap() -> void { glfwSwapBuffers(m_window); }
auto window::poll() -> void {
    std::for_each(std::begin(m_data.keys), std::end(m_data.keys),
    [this](auto const& pair) {
        pair.second->update(this->get_key(pair.second->value));
    });
    glfwPollEvents();
}
auto window::get_native() -> GLFWwindow* { return m_window; }
auto window::should_close() -> bool { return glfwWindowShouldClose(m_window); }
auto window::get_key(int32_t key) -> int32_t { return glfwGetKey(m_window, key); }
auto window::make_key(int32_t key) -> std::shared_ptr<state::key> {
    auto new_key = luma::state::make_key(key);
    m_data.keys[key] = new_key;
    return new_key;
}

auto window::update_key(state::key& key) -> void {
    key.update(this->get_key(key.value));
}
auto window::update_key(std::shared_ptr<state::key> const& key) -> void {
    key->update(this->get_key(key->value));
}

auto window::position(int32_t const& x, int32_t const& y) -> void {
    if (x != DONT_CARE) m_data.x = x;
    if (y != DONT_CARE) m_data.y = y;

    glfwSetWindowPos(m_window, m_data.x, m_data.y);
}

auto window::add_event_listener(event::type const& type, event_fn const& callback) -> void {
    if (m_data.events.find(type) == m_data.events.end()) {
        std::vector<event_fn> fns{callback};
        m_data.events.insert({type, fns});
    } else {
        // TODO: Maybe check if the callback has already be added.
        m_data.events[type].push_back(callback);
    }
}

auto window::remove_event_listener(event::type const& type, event_fn const& callback) -> void {
    auto it = m_data.events.find(type);
    if (it == m_data.events.end()) return;
    auto fns = it->second;

    auto found = std::find_if(std::begin(fns), std::end(fns),
    [&](event_fn const& fn) {
        if (fn.target_type() == callback.target_type()) {
            auto t1 = fn.target<event_fn>();
            auto t2 = callback.target<event_fn>();
            return t1 == t2;
        }
        return false;
    });
    std::cout << "found callback to remove: " << (found == fns.end() ? "true" : "false") << '\n';
}
}


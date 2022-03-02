#include "window.hpp"

namespace luma {
window::window(std::string const& name, int32_t const& width, int32_t const& height) {
    if (!glfwInit()) throw std::runtime_error("error initialising glfw");
    m_data.width  = width;
    m_data.height = height;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_data.width, m_data.height, name.c_str(), nullptr, nullptr);
    if (!m_window) throw std::runtime_error("Failed to create window");

    glfwGetWindowPos(m_window, &m_data.x, &m_data.y);
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
        throw std::runtime_error("Failed to initialize GLAD");

    glfwSetWindowUserPointer(m_window, &m_data);

    glfwSetFramebufferSizeCallback(m_window,
    [](GLFWwindow* window, int32_t width, int32_t height) {
        auto data = static_cast<window::data*>(glfwGetWindowUserPointer(window));
        data->width  = width;
        data->height = height;
        glViewport(0, 0, width, height);
    });
}
window::~window() {
    glfwTerminate();
}

auto window::swap() -> void { glfwSwapBuffers(m_window); }
auto window::poll() -> void { glfwPollEvents(); }
auto window::get_native() -> GLFWwindow* { return m_window; }
auto window::should_close() -> bool { return glfwWindowShouldClose(m_window); }
auto window::get_key(int32_t key) -> int32_t { return glfwGetKey(m_window, key); }

auto window::query_key(state::key& key) -> void {
    key.states[1] = key.states[0];
    key.states[0] = this->get_key(key.key);
}

auto window::position(int32_t const& x, int32_t const& y) -> void {
    if (x != DONT_CARE) m_data.x = x;
    if (y != DONT_CARE) m_data.y = y;

    glfwSetWindowPos(m_window, m_data.x, m_data.y);
}
}


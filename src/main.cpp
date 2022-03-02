#include <iostream>
#include <string>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace luma {
class image {
  public:
    image(std::string const& filename, int32_t const& channel = 0)
        : m_filename(filename), m_width(0), m_height(0), m_channels(channel) {
        m_buffer = stbi_load(m_filename.c_str(), &m_width, &m_height, &m_channels, 0);
    }
    ~image() {
        stbi_image_free(m_buffer);
    }

    auto buffer() -> uint8_t* { return m_buffer; }
    auto width() -> int32_t { return m_width; }
    auto height() -> int32_t { return m_height; }
    auto channels() -> int32_t { return m_channels; }

    auto info() -> std::string {
        return std::string("luma::image{width: ") + std::to_string(m_width) 
               + ", height: "   + std::to_string(m_height)
               + ", channels: " + std::to_string(m_channels) + "}";
    }

  private:
    std::string m_filename;
    int32_t     m_width;
    int32_t     m_height;
    int32_t     m_channels;
    uint8_t*    m_buffer;
};

class event {
  public:
    enum class type {
        window,
        mouse,
        keyboard,
    };

  public:
    event(event::type const& type) : m_type(type) {}
    virtual ~event() = default;

    auto get_type() -> type { return m_type; }
    virtual auto get_name() -> std::string;
    virtual auto to_string() -> std::string;

  protected:
    type m_type;
};

class window{
  public:
    inline static auto GLSL_VERSION = "#version 410";

  public:
    window(std::string const& name = "helloworld", int32_t const& width = 640, int32_t const& height = 480) {
        if (!glfwInit()) throw std::runtime_error("error initialising glfw");
        m_data.width  = width;
        m_data.height = height;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(m_data.width, m_data.height, name.c_str(), nullptr, nullptr);
        if (!m_window) throw std::runtime_error("Failed to create window");

        glfwGetWindowPos(m_window, &m_data.x, &m_data.y);
        glfwSetWindowPos(m_window, m_data.x, -800);

        glfwMakeContextCurrent(m_window);
        if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
            throw std::runtime_error("Failed to initialize GLAD");

        glfwSetWindowUserPointer(m_window, &m_data);

        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int32_t width, int32_t height) {
            auto data = static_cast<window::data*>(glfwGetWindowUserPointer(window));
            data->width  = width;
            data->height = height;
            glViewport(0, 0, width, height);
        });
    }
    ~window() {
        glfwTerminate();
    }

    auto swap() -> void { glfwSwapBuffers(m_window); }
    auto poll() -> void { glfwPollEvents(); }
    auto get_native() -> GLFWwindow* { return m_window; }
    auto should_close() -> bool { return glfwWindowShouldClose(m_window); }
    auto get_key(int32_t key) -> int32_t { return glfwGetKey(m_window, key); }

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

auto main([[maybe_unused]]int32_t argc, [[maybe_unused]]char const* argv[]) -> int32_t {
    luma::window window;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding= 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    ImGui_ImplGlfw_InitForOpenGL(window.get_native(), true);
    ImGui_ImplOpenGL3_Init(luma::window::GLSL_VERSION);

    auto is_running = true;
    while(is_running) {
        is_running = !window.should_close();
        // Handle inputs
        if (window.get_key(GLFW_KEY_Q) == GLFW_PRESS)
            is_running = false;

        // New Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        auto dockspace_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        ImGui::SetNextWindowDockID(dockspace_id);
        ImGui::Begin("scene", nullptr, ImGuiWindowFlags_NoMove);
        ImGui::End();

        ImGui::Render();

        // Viewport resize
        glClearColor(0.f, 0.f, 0.f, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Begin Draw
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            auto backup_current = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current);
        }

        window.swap();
        window.poll();
    }

    // Dear ImGui cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}


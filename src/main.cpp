#include <iostream>
#include <string>

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
        return std::string("luma::image{ width: ") + std::to_string(m_width) 
            + ", height: " + std::to_string(m_height)
            + ", channels: " + std::to_string(m_channels) + " }";
    }

  private:
    std::string m_filename;
    int32_t     m_width;
    int32_t     m_height;
    int32_t     m_channels;
    uint8_t*    m_buffer;
};
}

auto main([[maybe_unused]] int32_t argc, [[maybe_unused]] char const* argv[]) -> int32_t {
    GLFWwindow* window;
    if (!glfwInit()) return -1;

    constexpr auto GLSL_VERSION = "#version 410";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello, World!", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to initialize GLFW\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }

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
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);

    while(!glfwWindowShouldClose(window)) {
        // New Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();

        ImGui::Begin("Hello, World!");
        ImGui::Text("This is some text!");
        ImGui::End();

        ImGui::Begin("The world is ending");
        ImGui::End();

        ImGui::Begin("Console");
        ImGui::End();

        ImGui::Render();

        // Viewport resize
        int32_t width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.f, 0.f, 0.f, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            auto backup_current = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Dear ImGui cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}


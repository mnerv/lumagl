#include <iostream>
#include <string>
#include <stdexcept>

#include "window.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "image.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace luma {
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

template<typename T>
struct vec2 {
    T x;
    T y;
};
typedef vec2<float> vec2f;

auto imgui_window_size() -> vec2f {
    auto v_min = ImGui::GetWindowContentRegionMin();
    auto v_max = ImGui::GetWindowContentRegionMax();
    return { v_max.x - v_min.x, v_max.y - v_min.y };
}

}

float vertices[] = {
//  positions,              colors,                    texture coordinates
    -0.5f,  0.5f,  0.0f,    1.0f, 0.0f, 0.0f, 1.0f,    0.0f,  1.0f,
     0.5f,  0.5f,  0.0f,    0.0f, 1.0f, 0.0f, 1.0f,    1.0f,  1.0f,
     0.5f, -0.5f,  0.0f,    0.0f, 0.0f, 1.0f, 1.0f,    1.0f,  0.0f,
    -0.5f, -0.5f,  0.0f,    1.0f, 0.0f, 1.0f, 1.0f,    0.0f,  0.0f,
};

uint32_t indices[] = {
    0, 1, 2,
    0, 2, 3,
};

auto vertex_shader = R"(#version 410 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec2 a_uv;

out vec4 io_color;
out vec2 io_uv;

void main() {
    io_color = a_color;
    io_uv    = a_uv;
    gl_Position = vec4(a_position, 1.0);
}
)";

auto fragment_shader = R"(#version 410 core
layout(location = 0) out vec4 color;

in vec4 io_color;
in vec2 io_uv;

void main() {
    color = io_color;
}
)";

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

    luma::buffer::array arr{};
    luma::buffer::vertex vertex{vertices, sizeof(vertices)};
    luma::buffer::index index{indices, sizeof(indices) / sizeof(uint32_t)};
    luma::shader shader(vertex_shader, fragment_shader);
    vertex.bind();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);
    shader.bind();


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

        //auto dockspace_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        //ImGui::SetNextWindowDockID(dockspace_id);
        //ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));  // Add style
        //ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0.0f);
        //ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        //ImGui::Begin("scene", nullptr, ImGuiWindowFlags_NoMove);
        //ImGui::PopStyleVar(4);  // Apply style

        //{
        //    //auto size = luma::imgui_window_size();
        //}

        //ImGui::End();

        ImGui::Render();

        // Viewport resize
        glClearColor(0.f, 0.f, 0.f, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.bind();
        vertex.bind();
        arr.bind();
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, index.count(), GL_UNSIGNED_INT, 0);

        // Begin ImGui Draw
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


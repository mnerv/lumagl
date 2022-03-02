#include <iostream>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <array>

#include "window.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "input.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

std::array<float, 9 * 4> vertices{
//  positions,              colors,                    texture coordinates
    -0.5f,  0.5f,  0.0f,    1.0f, 0.0f, 0.0f, 1.0f,    0.0f,  1.0f,
     0.5f,  0.5f,  0.0f,    0.0f, 1.0f, 0.0f, 1.0f,    1.0f,  1.0f,
     0.5f, -0.5f,  0.0f,    0.0f, 0.0f, 1.0f, 1.0f,    1.0f,  0.0f,
    -0.5f, -0.5f,  0.0f,    1.0f, 0.0f, 1.0f, 1.0f,    0.0f,  0.0f,
};

std::array<uint32_t, 3 * 2> indices{
    0, 1, 2,
    0, 2, 3,
};

auto vertex_shader = R"(#version 410 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec2 a_uv;

out vec4 io_color;
out vec2 io_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    io_color = a_color;
    io_uv    = a_uv;
    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0f);
}
)";

auto fragment_shader = R"(#version 410 core
layout(location = 0) out vec4 color;

in vec4 io_color;
in vec2 io_uv;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;

void main() {
    color = io_color;
    color = mix(texture(u_texture1, io_uv), texture(u_texture2, io_uv), 0.5f);
    color = texture(u_texture1, io_uv);
}
)";

auto main([[maybe_unused]]int32_t argc, [[maybe_unused]]char const* argv[]) -> int32_t {
    luma::window window;
    window.position(luma::DONT_CARE, -800);

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

    luma::buffer::array vertex_array{};
    luma::buffer::vertex vertex{vertices.data(), sizeof(vertices)};
    luma::buffer::index index{indices.data(), indices.size()};
    luma::shader shader{vertex_shader, fragment_shader};
    vertex.bind();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);
    shader.bind();

    luma::texture texture{{"/Users/k/Downloads/marin.png"}};
    shader.uniform1i("u_texture1", 0);

    luma::perspective_camera camera{};
    camera.set_position({0.f, 0.f, 1.f});
    glm::mat4 model{1.f};

    bool is_cursor_on = true;
    luma::state::key toggle_cursor{GLFW_KEY_ESCAPE};
    luma::state::key quit_key{GLFW_KEY_Q};

    auto is_running = true;
    while(is_running) {
        is_running = !window.should_close();

        window.update_key(quit_key);
        window.update_key(toggle_cursor);

        // Handle inputs
        if (luma::state::is_press(quit_key)) is_running = false;
        if (luma::state::is_clicked(toggle_cursor)) {
            is_cursor_on = !is_cursor_on;
            auto cursor_status = is_cursor_on ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
            glfwSetInputMode(window.get_native(), GLFW_CURSOR, cursor_status);
        }
        camera.update(window);

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

        texture.bind(0);
        shader.bind();
        shader.uniform_m4("u_model", glm::value_ptr(model));
        shader.uniform_m4("u_view", glm::value_ptr(camera.view()));
        shader.uniform_m4("u_projection", glm::value_ptr(camera.projection()));

        vertex.bind();
        vertex_array.bind();
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


#include <iostream>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <array>
#include <cmath>

#include "luma.hpp"
#include "window.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "mesh.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

auto vertex_shader = R"(#version 410 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec2 a_uv;

out vec4 io_color;
out vec2 io_uv;
out vec3 near_point;
out vec3 far_point;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

vec3 unproject_point(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 inv = inverse(projection * view);
    vec4 unprojected_point = inv * vec4(x, y, z, 1.f);
    return unprojected_point.xyz / unprojected_point.w;
}

void main() {
    io_color = a_color;
    io_uv    = a_uv;

    vec3 p = a_position;

    near_point = unproject_point(a_position.x, a_position.y, 0.f, u_view, u_projection).xyz;
    far_point  = unproject_point(a_position.x, a_position.y, 1.f, u_view, u_projection).xyz;

    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0f);
    //gl_Position = vec4(a_position, 1.0f);
}
)";

auto fragment_shader = R"(#version 410 core
layout(location = 0) out vec4 color;

in vec4 io_color;
in vec2 io_uv;
in vec3 near_point;
in vec3 far_point;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;

void main() {
    //color = io_color;
    //color = mix(texture(u_texture1, io_uv), texture(u_texture2, io_uv), 0.5f);
    //float t = - near_point.y / (far_point.y - near_point.y);
    //color = vec4(1.f, 0.f, 0.f, 1.f);
    color = io_color;
}
)";

auto screen_vertex_shader = R"(#version 410 core
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
    gl_Position = vec4(a_position, 1.0f);
}
)";

auto screen_fragment_shader = R"(#version 410 core
layout(location = 0) out vec4 color;

in vec4 io_color;
in vec2 io_uv;

uniform sampler2D u_texture;

void main() {
    color = texture(u_texture, io_uv);
}
)";

float cube_vertices[] = {
    // bottom
    -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  1.0f, 0.0f,

    // top
    -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
};

uint32_t cube_indices[] = {
    0, 2, 3,
    0, 1, 2,

    6, 5, 4,
    6, 4, 7,

    7, 3, 2,
    2, 6, 7,

    4, 1, 0,
    1, 4, 5,

    5, 2, 1,
    2, 5, 6,

    7, 4, 0,
    0, 3, 7,
};

auto main([[maybe_unused]]int32_t argc, [[maybe_unused]]char const* argv[]) -> int32_t {
    luma::window window;
    //window.position(luma::DONT_CARE, -800);

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

    luma::buffer::layout vertex_layout{{
        {luma::shader::type::vec3, "a_position"},
        {luma::shader::type::vec4, "a_color"},
        {luma::shader::type::vec2, "a_uv"},
    }};

    auto cube_mesh = luma::mesh::cube();
    auto cube_va = luma::buffer::array::create();
    auto cube_vb = luma::buffer::vertex::create(cube_mesh->vertices().data(), cube_mesh->vertex_size());
    cube_vb->set_layout(vertex_layout);
    auto cube_ib = luma::buffer::index::create(cube_mesh->indices().data(), cube_mesh->index_count());
    cube_va->add_vertex_buffer(cube_vb);
    cube_va->set_index_buffer(cube_ib);

    auto plane = luma::mesh::plane();
    auto vertex_array = luma::buffer::array::create();
    auto vertex_buffer = luma::buffer::vertex::create(plane->vertices().data(), plane->vertices_size());
    vertex_buffer->set_layout(vertex_layout);
    auto index_buffer = luma::buffer::index::create(plane->indices().data(), plane->indices().size());
    vertex_array->add_vertex_buffer(vertex_buffer);
    vertex_array->set_index_buffer(index_buffer);

    luma::shader shader{vertex_shader, fragment_shader};
    vertex_buffer->bind();

    auto screen_va = luma::buffer::array::create();
    auto screen = luma::mesh::plane();
    auto screen_vb = luma::buffer::vertex::create(screen->vertices().data(), screen->vertex_count() * sizeof(luma::mesh::vertex));
    auto screen_ib = luma::buffer::index::create(screen->indices().data(), screen->indices().size());
    screen_vb->set_layout(vertex_layout);
    screen_va->add_vertex_buffer(screen_vb);
    screen_va->set_index_buffer(screen_ib);

    luma::shader screen_shader{screen_vertex_shader, screen_fragment_shader};

    screen_shader.bind();

    shader.num("u_texture1", 0);
    auto texture = luma::make_ref<luma::texture>("/Users/k/Downloads/marin.png");

    int32_t width, height;
    glfwGetFramebufferSize(window.get_native(), &width, &height);
    int32_t w_width, w_height;
    glfwGetWindowSize(window.get_native(), &w_width, &w_height);

    auto framebuffer = luma::make_ref<luma::buffer::frame>();
    uint32_t texture_render_buffer;
    glGenTextures(1, &texture_render_buffer);
    glBindTexture(GL_TEXTURE_2D, texture_render_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_render_buffer, 0);
    uint32_t render_buffer;
    glGenRenderbuffers(1, &render_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER: Framebuffer is not complete!\n";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    luma::perspective_camera camera{};
    camera.move({0.f, 0.f, 2.f});
    camera.set_front({0.f, 0.f, -1.f});

    glm::mat4 model{1.f};

    bool is_cursor_on  = true;
    auto toggle_cursor = window.make_key(GLFW_KEY_ESCAPE);
    auto quit_key      = window.make_key(GLFW_KEY_Q);
    auto left_key      = window.make_key(GLFW_KEY_A);
    auto right_key     = window.make_key(GLFW_KEY_D);
    auto up_key        = window.make_key(GLFW_KEY_W);
    auto down_key      = window.make_key(GLFW_KEY_S);
    auto plus_alt      = window.make_key(GLFW_KEY_F);
    auto neg_alt       = window.make_key(GLFW_KEY_V);
    auto speed_boost   = window.make_key(GLFW_KEY_LEFT_SHIFT);
    auto reset_camera  = window.make_key(GLFW_KEY_0);

    double time[2]{glfwGetTime(), 0};
    double delta_time = 0;
    float camera_speed = 1.f;

    glm::dvec2 mouse_current{};
    glm::dvec2 mouse_previous{};
    float sensitivity = 0.1f;
    float yaw = 0.f, pitch = 0.f;
    float boost = 1.0f;
    //bool first_loop = true;

    glm::vec2 scene_size{window.width(), window.height()};
    //constexpr auto filename_size = 512;
    //auto filename = new char[filename_size];
    //std::memset(filename, 0, filename_size);

    auto is_running = true;
    while(is_running) {
        glfwGetFramebufferSize(window.get_native(), &width, &height);
        glfwGetWindowSize(window.get_native(), &w_width, &w_height);

        time[1] = time[0];
        time[0] = glfwGetTime();
        delta_time = time[0] - time[1];

        scene_size.x = window.width();
        scene_size.y = window.height();
        is_running = !window.should_close();
        mouse_previous = mouse_current;
        glfwGetCursorPos(window.get_native(), &mouse_current.x, &mouse_current.y);
        auto mouse_delta = mouse_current - mouse_previous;

          // Handle inputs
        if (luma::state::is_press(quit_key)) is_running = false;
        if (luma::state::is_clicked(toggle_cursor)) {
            is_cursor_on = !is_cursor_on;
            auto cursor_status = is_cursor_on ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
            glfwSetInputMode(window.get_native(), GLFW_CURSOR, cursor_status);
        }

        if (luma::state::is_press(up_key)) {
            camera.move_delta(camera_speed * boost * camera.front() * float(delta_time));
        } else if (luma::state::is_press(down_key)) {
            camera.move_delta(-camera_speed * boost * camera.front() * float(delta_time));
        }

        if (luma::state::is_press(left_key)) {
            camera.move_delta(-camera_speed * boost * glm::normalize(glm::cross(camera.front(), camera.up())) * float(delta_time));
        } else if (luma::state::is_press(right_key)) {
            camera.move_delta(camera_speed * boost * glm::normalize(glm::cross(camera.front(), camera.up())) * float(delta_time));
        }

        if (luma::state::is_press(plus_alt)) {
            camera.move_delta(camera_speed * boost * glm::vec3{0.f, 1.f, 0.f} * float(delta_time));
        } else if (luma::state::is_press(neg_alt)) {
            camera.move_delta(-camera_speed * boost * glm::vec3{0.f, 1.f, 0.f} * float(delta_time));
        }

        if (luma::state::is_press(speed_boost)) {
            boost = 4.f;
        } else {
            boost = 1.f;
        }

        if (luma::state::is_clicked(reset_camera)) {
            yaw = 0;
            pitch = 1;
        }

        if (!is_cursor_on) {
            yaw   += mouse_delta.x * -sensitivity;
            pitch += mouse_delta.y * -sensitivity;

            if (pitch > 90.f - 1.f) pitch = 89.f;
            if (pitch < -90.f + 1.f) pitch = -89.f;
            glm::vec3 direction{};
            direction.x = -std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
            direction.y = std::sin(glm::radians(pitch));
            direction.z = -std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
            camera.set_front(glm::normalize(direction));
        }

        framebuffer->bind();
        glBindTexture(GL_TEXTURE_2D, texture_render_buffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_render_buffer, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "ERROR::FRAMEBUFFER: Framebuffer is not complete!\n";
        framebuffer->unbind();

        camera.set_screen(width, height);
        camera.update();

        // FIRST PASS
        framebuffer->bind();
        glViewport(0, 0, width, height);
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        shader.bind();
        shader.num("u_texture1", 0);
        texture->bind(0);
        shader.mat4("u_model", glm::value_ptr(model));
        shader.mat4("u_view", glm::value_ptr(camera.view()));
        shader.mat4("u_projection", glm::value_ptr(camera.projection()));

        vertex_array->bind();
        index_buffer->bind();
        vertex_buffer->bind();
        glDrawElements(GL_TRIANGLES, index_buffer->count(), GL_UNSIGNED_INT, 0);

        cube_va->bind();
        cube_vb->bind();
        cube_ib->bind();
        glDrawElements(GL_TRIANGLES, cube_ib->count(), GL_UNSIGNED_INT, 0);
        framebuffer->unbind();

        // SECOND PASS
        glViewport(0, 0, width, height);
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        screen_shader.bind();
        screen_shader.num("u_texture", 0);
        screen_shader.mat4("u_model", glm::value_ptr(model));
        screen_shader.mat4("u_view", glm::value_ptr(camera.view()));
        screen_shader.mat4("u_projection", glm::value_ptr(camera.projection()));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_render_buffer);

        screen_va->bind();
        screen_vb->bind();
        screen_ib->bind();
        glDrawElements(GL_TRIANGLES, screen_ib->count(), GL_UNSIGNED_INT, 0);

        // New Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //auto dockspace_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        //if (first_loop) {
        //    first_loop = false;
        //    ImGui::SetNextWindowDockID(dockspace_id);
        //}
        //ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));  // Add style
        //ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0.0f);
        //ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        //ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.8f);
        //ImGui::Begin("scene", nullptr, ImGuiWindowFlags_None | ImGuiWindowFlags_NoBringToFrontOnFocus);
        //ImGui::PopStyleVar(5);  // Apply style
        //scene_size = luma::imgui_window_size();
        //ImGui::Image((void*)intptr_t(texture_render_buffer), ImVec2{scene_size.x, scene_size.y}, ImVec2{0, 1}, ImVec2{1, 0});
        //ImGui::End();

        //ImGui::Begin("property", nullptr, ImGuiWindowFlags_None);
        //if (ImGui::InputText("texture file", filename, filename_size, ImGuiInputTextFlags_EnterReturnsTrue)) {
        //    std::cout << filename << '\n';
        //    texture = luma::make_ref<luma::texture>(filename);
        //}
        //ImGui::End();

        ImGui::Render();

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

    glDeleteTextures(1, &texture_render_buffer);
    glDeleteRenderbuffers(1, &render_buffer);

    return 0;
}


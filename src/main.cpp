#include <iostream>
#include <string>
#include <numbers>

#include "luma.hpp"
#include "window.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "mesh.hpp"
#include "grid.hpp"
#include "event.hpp"

// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

auto vertex_shader = R"glsl(#version 410 core
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
)glsl";

auto fragment_shader = R"glsl(#version 410 core
layout(location = 0) out vec4 color;

in vec4 io_color;
in vec2 io_uv;

uniform sampler2D u_texture;

void main() {
    color = texture(u_texture, io_uv);
}
)glsl";

auto screen_vertex_shader = R"glsl(#version 410 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec2 a_uv;

out vec4 io_color;
out vec2 io_uv;

void main() {
    io_color = a_color;
    io_uv    = a_uv;
    gl_Position = vec4(a_position, 1.0f);
}
)glsl";

auto screen_fragment_shader = R"glsl(#version 410 core
layout(location = 0) out vec4 color;

in vec4 io_color;
in vec2 io_uv;

uniform sampler2D u_texture;

void main() {
    color = texture(u_texture, io_uv);
}
)glsl";

auto main([[maybe_unused]]int32_t argc, [[maybe_unused]]char const* argv[]) -> int32_t {
    luma::window window{"Hello, Grid!", 1280, 720};
    //window.position(luma::DONT_CARE, -800);

    int32_t width, height;
    glfwGetFramebufferSize(window.get_native(), &width, &height);
    int32_t w_width, w_height;
    glfwGetWindowSize(window.get_native(), &w_width, &w_height);

    luma::buffer::layout vertex_layout{
        {luma::shader::type::vec3, "a_position"},
        {luma::shader::type::vec4, "a_color"},
        {luma::shader::type::vec2, "a_uv"},
    };
    luma::shader shader{vertex_shader, fragment_shader};
    luma::shader screen_shader{screen_vertex_shader, screen_fragment_shader};
    auto texture = luma::make_ref<luma::texture>("C:/Users/miku/Downloads/profilepic.png");

    std::cout << texture->get_image()->info() << "\n";

    auto plane = luma::mesh::plane();
    auto plane_va = luma::buffer::array::create();
    auto plane_vb = luma::buffer::vertex::create(plane->vertices().data(), plane->vertices_size());
    auto plane_ib = luma::buffer::index::create(plane->indices().data(), plane->index_count());
    plane_vb->set_layout(vertex_layout);
    plane_va->add_vertex_buffer(plane_vb);
    plane_va->set_index_buffer(plane_ib);

    auto screen = luma::mesh::plane();
    auto screen_va = luma::buffer::array::create();
    auto screen_vb = luma::buffer::vertex::create(screen->vertices().data(), screen->vertex_count() * sizeof(luma::mesh::vertex));
    auto screen_ib = luma::buffer::index::create(screen->indices().data(), screen->indices().size());
    screen_vb->set_layout(vertex_layout);
    screen_va->add_vertex_buffer(screen_vb);
    screen_va->set_index_buffer(screen_ib);

    auto framebuffer = luma::make_ref<luma::buffer::frame>();
    uint32_t texture_render_buffer;
    glGenTextures(1, &texture_render_buffer);
    glBindTexture(GL_TEXTURE_2D, texture_render_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
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
    luma::grid grid_render{};

    bool is_cursor_on  = true;
    auto toggle_cursor = window.make_key(GLFW_KEY_ESCAPE);
    auto quit_key      = window.make_key(GLFW_KEY_Q);
    auto left_key      = window.make_key(GLFW_KEY_A);
    auto right_key     = window.make_key(GLFW_KEY_D);
    auto up_key        = window.make_key(GLFW_KEY_W);
    auto down_key      = window.make_key(GLFW_KEY_S);
    auto plus_alt      = window.make_key(GLFW_KEY_F);
    auto neg_alt       = window.make_key(GLFW_KEY_V);
    auto reset_camera  = window.make_key(GLFW_KEY_0);

    auto pan_on  = window.make_key(GLFW_KEY_LEFT_SHIFT);
    auto zoom_on = window.make_key(GLFW_KEY_LEFT_CONTROL);

    double time[2]{glfwGetTime(), 0};
    double delta_time = 0;
    //float camera_speed = 1.f;

    glm::dvec2 mouse_current{};
    glm::dvec2 mouse_previous{};
    //bool first_loop = true;

    glm::mat4 model{1.f};
    luma::camera camera{};
    camera.position = {0.0f, 0.0f, 2.0f};
    auto is_running = true;

    bool arcball_on = true;
    bool is_mouse_down = false;
    float arcball_speed = 4.0f;


    // persistent
    glm::quat cam_q{1,0,0,0};
    glm::vec3 world_up = glm::normalize(glm::vec3(0,1,0));

    auto on_mouse_move = [&](luma::event const& e) {
        if (!is_mouse_down) return;
        auto const& evt = static_cast<luma::mouse_move_event const&>(e);
        mouse_current = {evt.x(), evt.y()};

        if (arcball_on) {
            float const orbit_radius = glm::length(camera.position - camera.target);

            glm::vec2 d_angle{
                2.0f * float(std::numbers::pi) / float(width),
                2.0f * float(std::numbers::pi) / float(height)
            };

            float dx = (mouse_current.x - mouse_previous.x) * d_angle.x * arcball_speed;
            float dy = (mouse_current.y - mouse_previous.y) * d_angle.y * arcball_speed;

            // Camera's current up from orientation
            glm::vec3 cam_up = glm::normalize(cam_q * glm::vec3(0,1,0));

            // If upside-down, invert yaw so left/right drag stays intuitive
            float upSign = (glm::dot(cam_up, world_up) >= 0.0f) ? 1.0f : -1.0f;
            float yaw   = -dx * upSign;
            float pitch = -dy;

            glm::vec3 right = glm::normalize(cam_q * glm::vec3(1,0,0));

            glm::quat q_yaw   = glm::angleAxis(yaw, world_up);
            glm::quat q_pitch = glm::angleAxis(pitch, right);

            cam_q = glm::normalize(q_yaw * q_pitch * cam_q);

            glm::vec3 forward = glm::normalize(cam_q * glm::vec3(0,0,-1));
            glm::vec3 final_position = camera.target - forward * orbit_radius;

            // Optional: keep camera.up derived from orientation (roll allowed)
            camera.up = glm::normalize(cam_q * glm::vec3(0,1,0));

            camera.position = final_position;
        }

        mouse_previous = mouse_current;
    };


    auto on_wheel = [&](luma::event const& e) {
        auto evt = static_cast<luma::mouse_wheel_event const&>(e);
        if (arcball_on) {
            glm::vec2 d_angle{2.0 * std::numbers::pi / float(width), 2.0 * std::numbers::pi / float(height)};
            glm::vec4 position{camera.position.x, camera.position.y, camera.position.z, 1.0f};
            glm::vec4 pivot{camera.target.x, camera.target.y, camera.target.z, 1.0f};

            float angle_x = evt.x() * d_angle.x * arcball_speed;
            float angle_y = evt.y() * d_angle.y * arcball_speed;

            glm::mat4 rotation_mat_x{1.0f};
            rotation_mat_x = glm::rotate(rotation_mat_x, angle_x, camera.up);
            position = (rotation_mat_x * (position - pivot)) + pivot;

            glm::mat4 rotation_mat_y{1.0f};
            rotation_mat_y = glm::rotate(rotation_mat_y, angle_y, camera.get_right_vector());
            glm::vec3 final_position = (rotation_mat_y * (position - pivot)) + pivot;

            camera.position = final_position;
        } else if(luma::state::is_press(zoom_on)) {
            const float sensitivity = 100.0f;
            auto front_vector = glm::normalize(camera.position - camera.target);
            glm::vec3 move_delta = front_vector * float(delta_time) * float(evt.y() * sensitivity);
            camera.position += move_delta;
        } else if (luma::state::is_press(pan_on)) {
            auto front_vector = camera.front();
            auto right_vector = glm::normalize(glm::cross(front_vector, camera.up));

            camera.position += camera.up * float(evt.y()) * float(delta_time);
            camera.position += right_vector * float(evt.x()) * float(delta_time);
            camera.target = camera.position + front_vector;
        }
    };
    auto on_key_down = [&](luma::event const& e) {
        auto evt = static_cast<luma::key_down_event const&>(e);
        std::cout << e.to_string() << "\n";
        if (evt.key() == GLFW_KEY_Q) is_running = false;
        if (evt.key() == GLFW_KEY_LEFT_SHIFT || evt.key() == GLFW_KEY_LEFT_CONTROL)
            arcball_on = false;
    };
    auto on_key_up = [&](luma::event const& e) {
        auto evt = static_cast<luma::key_up_event const&>(e);
        if (evt.key() == GLFW_KEY_LEFT_SHIFT || evt.key() == GLFW_KEY_LEFT_CONTROL)
            arcball_on = true;
    };
    auto on_mouse_down = [&](luma::event const& e) {
        auto evt = static_cast<luma::mouse_press_event const&>(e);
        is_mouse_down = true;
    };
    auto on_mouse_up = [&](luma::event const& e) {
        auto evt = static_cast<luma::mouse_release_event const&>(e);
        is_mouse_down = false;
    };

    window.add_event_listener(luma::event::type::key_down, on_key_down);
    window.add_event_listener(luma::event::type::key_up, on_key_up);
    window.add_event_listener(luma::event::type::mouse_wheel, on_wheel);
    window.add_event_listener(luma::event::type::mouse_move, on_mouse_move);
    window.add_event_listener(luma::event::type::mouse_press, on_mouse_down);
    window.add_event_listener(luma::event::type::mouse_release, on_mouse_up);

    // GOAL: Blender camera navigation
    //          #1. Orbit:
    //              - scroll
    //              - middle mouse
    //          #2. Pan:
    //              - [shift | option]+scroll
    //              - [shift | option]+middle mouse
    //          #2. Zoom:
    //              - control+scroll
    //              - control+middle mouse

    while(is_running) {
        glfwGetFramebufferSize(window.get_native(), &width, &height);
        glfwGetWindowSize(window.get_native(), &w_width, &w_height);

        is_running = !window.should_close();
        time[1] = time[0];
        time[0] = glfwGetTime();
        delta_time = time[0] - time[1];

        mouse_previous = mouse_current;
        glfwGetCursorPos(window.get_native(), &mouse_current.x, &mouse_current.y);

          // Handle inputs
        if (luma::state::is_clicked(toggle_cursor)) {
            is_cursor_on = !is_cursor_on;
            auto cursor_status = is_cursor_on ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
            glfwSetInputMode(window.get_native(), GLFW_CURSOR, cursor_status);
        }

        camera.update_perspective(float(width) / float(height), 45.0f);

        framebuffer->bind();
        glBindTexture(GL_TEXTURE_2D, texture_render_buffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
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

        // FIRST PASS
        framebuffer->bind();
        glViewport(0, 0, width, height);
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_FRONT);

        shader.bind();
        shader.num("u_texture", 0);
        texture->bind(0);

        model = glm::mat4{1.0f};
        model = glm::translate(model, {0.0f, 1.f, 0.0f});

        auto world_to_view = camera.world_to_view();
        auto projection    = camera.projection();

        shader.mat4("u_model", glm::value_ptr(model));
        shader.mat4("u_view", glm::value_ptr(world_to_view));
        shader.mat4("u_projection", glm::value_ptr(projection));

        plane_va->bind();
        plane_vb->bind();
        plane_ib->bind();
        glDrawElements(GL_TRIANGLES, plane_ib->count(), GL_UNSIGNED_INT, 0);

        grid_render.render(world_to_view, projection,
                           glm::vec2{camera.near, camera.far});
        framebuffer->unbind();

        // SECOND PASS
        glViewport(0, 0, width, height);
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        screen_shader.bind();
        screen_shader.num("u_texture", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_render_buffer);

        screen_va->bind();
        screen_vb->bind();
        screen_ib->bind();
        glDrawElements(GL_TRIANGLES, screen_ib->count(), GL_UNSIGNED_INT, 0);

        window.swap();
        window.poll();
    }

    glDeleteTextures(1, &texture_render_buffer);
    glDeleteRenderbuffers(1, &render_buffer);

    return 0;
}

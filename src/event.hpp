#pragma once

#include "luma.hpp"
#include <string>
#include <functional>
#include <sstream>
#include <cstdint>

namespace luma {

class event {
  public:
    enum class type : uint32_t {
        none = 0,
        buffer_resize,
        window_resize, window_move, window_focus,
        mouse_move,    mouse_press, mouse_release, mouse_wheel,
        key_down,      key_up,      key_typed,
    };

    enum class category : uint8_t {
        none        = LUMA_BIT(0),
        window      = LUMA_BIT(1),
        application = LUMA_BIT(2),
        mouse       = LUMA_BIT(3),
        keyboard    = LUMA_BIT(4),
        buffer      = LUMA_BIT(5),
    };

  public:
    event(event::type const& type, event::category const& category)
        : m_type(type), m_category(category) {}
    virtual ~event() = default;

    auto get_type() const -> type { return m_type; }
    auto get_category() const -> category { return m_category; }
    virtual auto get_name() const -> std::string = 0;
    virtual auto to_string() const -> std::string = 0;

  protected:
    type     m_type;
    category m_category;
};

class buffer_resize_event : public event {
  public:
    buffer_resize_event(int32_t const& width, int32_t const& height) :
                        event(type::buffer_resize, category::buffer),
                        m_width(width), m_height(height) {}
    ~buffer_resize_event() = default;

    auto get_name() const -> std::string override { return "buffer_resize_event"; }
    auto to_string() const -> std::string override {
        std::stringstream ss;
        ss << "buffer_resize_event {";
        ss << "width: "  << m_width << ", ";
        ss << "height: " << m_height;
        ss << " }";
        return ss.str();
    }

    auto width() const -> int32_t { return m_width; }
    auto height() const -> int32_t { return m_height; }

  private:
    int32_t m_width;
    int32_t m_height;
};

class window_resize_event : public event {
  public:
    window_resize_event(int32_t const& width, int32_t const& height) :
                        event(type::window_resize, category::window),
                        m_width(width), m_height(height) {}
    ~window_resize_event() = default;

    auto get_name() const -> std::string override { return "window_resize_event"; }
    auto to_string() const -> std::string override {
        std::stringstream ss;
        ss << "window_resize_event {";
        ss << "width: "  << m_width << ", ";
        ss << "height: " << m_height;
        ss << " }";
        return ss.str();
    }

    auto width() const -> int32_t { return m_width; }
    auto height() const -> int32_t { return m_height; }

  private:
    int32_t m_width;
    int32_t m_height;
};

class window_move_event : public event {
  public:
    window_move_event(int32_t const& x, int32_t const& y) :
                        event(type::window_move, category::window),
                        m_x(x), m_y(y) {}
    ~window_move_event() = default;

    auto get_name() const -> std::string override { return "window_move_event"; }
    auto to_string() const -> std::string override {
        std::stringstream ss;
        ss << "window_move_event { ";
        ss << "x: " << m_x << ", ";
        ss << "y: " << m_y;
        ss << " }";
        return ss.str();
    }

    auto x() const -> int32_t { return m_x; }
    auto y() const -> int32_t { return m_y; }

  private:
    int32_t m_x;
    int32_t m_y;
};

class window_focus_event : public event {
  public:
    window_focus_event(bool const& is_focus) :
                       event(type::window_focus, category::window),
                       m_is_focus(is_focus) {}
    ~window_focus_event() = default;

    auto get_name() const -> std::string override { return "window_focus_event"; }
    auto to_string() const -> std::string override {
        std::stringstream ss;
        ss << "window_focus_event { ";
        ss << "focus: " << (m_is_focus ? "true" : "false");
        ss << " }";
        return ss.str();
    }

    auto is_focus() const -> bool { return m_is_focus; }

  private:
    bool m_is_focus;
};

class mouse_move_event : public event {
  public:
    mouse_move_event(double const& x, double const& y) :
                     event(type::mouse_move, category::mouse),
                     m_x(x), m_y(y) {}
    ~mouse_move_event() = default;

    auto get_name() const -> std::string override { return "mouse_move_event"; }
    auto to_string() const -> std::string override {
        std::stringstream ss;
        ss << "mouse_move_event { ";
        ss << "x: " << m_x << ", ";
        ss << "y: " << m_y;
        ss << " }";
        return ss.str();
    }

    auto x() const -> double { return m_x; }
    auto y() const -> double { return m_y; }

  private:
    double m_x;
    double m_y;
};

class mouse_press_event : public event {
  public:
    mouse_press_event(int32_t const& button, int32_t const& mods, int32_t const& x, int32_t const& y)
        : event(type::mouse_press, category::mouse),
          m_button(button), m_mods(mods), m_x(x), m_y(y) {};
    ~mouse_press_event() = default;

    auto get_name() const -> std::string override { return "mouse_press_event"; }
    auto to_string() const -> std::string override {
        std::stringstream ss;
        ss << "mouse_press_event { ";
        ss << "button: " << m_button << ", ";
        ss << "mods: " << m_mods << ", ";
        ss << "x: " << m_x << ", ";
        ss << "y: " << m_y << " }";
        return ss.str();
    }

    auto button() const -> int32_t { return m_button; }
    auto mods() const -> int32_t { return m_mods; }
    auto x() const -> int32_t { return m_x; }
    auto y() const -> int32_t { return m_y; }

  private:
    int32_t m_button;
    int32_t m_mods;
    int32_t m_x;
    int32_t m_y;
};

class mouse_release_event : public event {
  public:
    mouse_release_event(int32_t const& button, int32_t const& mods,
                        int32_t const& x, int32_t const& y)
        : event(type::mouse_release, category::mouse),
          m_button(button), m_mods(mods), m_x(x), m_y(y) {};
    ~mouse_release_event() = default;

    auto get_name() const -> std::string override { return "mouse_release_event"; }
    auto to_string() const -> std::string override {
        std::stringstream ss;
        ss << "mouse_release_event { ";
        ss << "button: " << m_button << ", ";
        ss << "mods: " << m_mods << ", ";
        ss << "x: " << m_x << ", ";
        ss << "y: " << m_y << " }";
        return ss.str();
    }

    auto button() const -> int32_t { return m_button; }
    auto mods() const -> int32_t { return m_mods; }
    auto x() const -> int32_t { return m_x; }
    auto y() const -> int32_t { return m_y; }

  private:
    int32_t m_button;
    int32_t m_mods;
    int32_t m_x;
    int32_t m_y;
};

class mouse_wheel_event : public event {
  public:
    mouse_wheel_event(double const& x_offset, double const& y_offset) : 
                      event(type::mouse_wheel, category::mouse),
                      m_x(x_offset), m_y(y_offset) {}
    ~mouse_wheel_event() = default;

    auto get_name() const -> std::string override { return "mouse_wheel_event"; }
    auto to_string() const -> std::string override {
        std::stringstream ss;
        ss << "mouse_wheel_event { ";
        ss << "x: " << m_x << ", ";
        ss << "y: " << m_y;
        ss << " } ";
        return ss.str();
    }

    auto x() const -> double { return m_x; }
    auto y() const -> double { return m_y; }

  private:
    double m_x;
    double m_y;
};

class key_down_event : public event {
  public:
    key_down_event(int32_t const& key, int32_t const& code,
                   int32_t const& mods, bool const& is_repeat = false)
        : event(type::key_down, category::keyboard),
          m_key(key), m_code(code), m_mods(mods),
          m_is_repeat(is_repeat) {};
    ~key_down_event() = default;

    auto get_name() const -> std::string override { return "key_down_event"; }
    auto to_string() const -> std::string override {
        std::stringstream ss;
        ss << "key_down_event { ";
        ss << "key: " << m_key << ", ";
        ss << "code: " << m_code << ", ";
        ss << "mods: " << m_mods << ", ";
        ss << "repeat: " << (m_is_repeat ? "true" : "false") << " }";
        return ss.str();
    }

    auto key() const -> int32_t { return m_key; }
    auto code() const -> int32_t { return m_code; }
    auto mods() const -> int32_t { return m_mods; }
    auto is_repeat() const -> bool { return m_is_repeat; }

  private:
    int32_t m_key;
    int32_t m_code;
    int32_t m_mods;
    bool    m_is_repeat;
};

class key_up_event : public event {
  public:
    key_up_event(int32_t const& key, int32_t const& code, int32_t const& mods)
        : event(type::key_up, category::keyboard), m_key(key),
          m_code(code), m_mods(mods) {};
    ~key_up_event() = default;

    auto get_name() const -> std::string override { return "key_up_event"; }
    auto to_string() const -> std::string override {
        std::stringstream ss;
        ss << "key_up_event { ";
        ss << "key: " << m_key << ", ";
        ss << "code: " << m_code << ", ";
        ss << "mods: " << m_mods << "} ";
        return ss.str();
    }

    auto key() const -> int32_t { return m_key; }
    auto code() const -> int32_t { return m_code; }
    auto mods() const -> int32_t { return m_mods; }

  private:
    int32_t m_key;
    int32_t m_code;
    int32_t m_mods;
};

class key_typed : public event {
  public:
    key_typed(uint32_t const& code_point)
        : event(type::key_typed, category::keyboard),
          m_code_point(code_point) {};
    ~key_typed() = default;

    auto get_name() const -> std::string override { return "key_typed_event"; }
    auto to_string() const -> std::string override {
        std::stringstream ss;
        ss << "key_typed_event { ";
        ss << "code: " << m_code_point << " }";
        return ss.str();
    }

    auto code() const -> uint32_t { return m_code_point; }

  private:
    uint32_t m_code_point;
};
}

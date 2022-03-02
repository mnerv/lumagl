#pragma once
#include <string>

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

}

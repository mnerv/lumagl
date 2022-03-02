#include "util.hpp"
#include "imgui.h"

namespace luma {
auto imgui_window_size() -> glm::vec2 {
    auto v_min = ImGui::GetWindowContentRegionMin();
    auto v_max = ImGui::GetWindowContentRegionMax();
    return { v_max.x - v_min.x, v_max.y - v_min.y };
}
}


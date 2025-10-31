cmake_minimum_required(VERSION 3.21)
project(lumagl VERSION 0.0.2)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)  # Group CMake targets inside a folder
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)         # Generate compile_commands.json for language servers
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/deps")

option(USE_MINIAUDIO "Use Miniaudio library" OFF)

find_package(fmt REQUIRED)
find_package(ctre REQUIRED)
find_package(stb REQUIRED)
find_package(utf8cpp REQUIRED)

include(FetchContent)
if (NOT EMSCRIPTEN)
    option(GLFW_BUILD_EXAMPLES OFF)
    option(GLFW_BUILD_TESTS OFF)
    option(GLFW_BUILD_DOCS OFF)
    option(GLFW_INSTALL OFF)
    option(GLFW_BUILD_WAYLAND OFF)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
    )
    list(APPEND FETCH_CONTENTS glfw)
    FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/mononerv/glad.git
        GIT_TAG        f4759d7c5143c0a23391ab05caaf43052cefdd65
    )
    list(APPEND FETCH_CONTENTS glad)
endif()
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
list(APPEND FETCH_CONTENTS glm)
FetchContent_MakeAvailable(${FETCH_CONTENTS})

# Group dependencies in Visual Studio and Xcode
if (CMAKE_GENERATOR MATCHES "Visual Studio" OR CMAKE_GENERATOR MATCHES "Xcode")
    set_target_properties(fmt PROPERTIES FOLDER deps)
    set_target_properties(stb PROPERTIES FOLDER deps)
endif()

if (APPLE)
    message(STATUS "Platform: Apple")
    list(APPEND BASE_DEFINITIONS
        "LUMA_PLATFORM=Apple"
    )
    set(IS_UNIX true)
    set(PLATFORM_LINK_LIBRARIES
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework OpenGL"
    )
elseif (UNIX AND NOT APPLE AND NOT EMSCRIPTEN)  # Linux, BSD, Solaris, Minix
    message(STATUS "Platform: Unix")
    list(APPEND BASE_DEFINITIONS
        "LUMA_PLATFORM=Linux"
    )
    set(PLATFORM_LINK_LIBRARIES
        "dl"
        "m"
        "GL"
        "X11"
    )
    set(IS_UNIX true)
elseif (WIN32)
    message(STATUS "Platform Windows")
    list(APPEND BASE_DEFINITIONS
        "LUMA_PLATFORM=Windows"
    )
    set(PLATFORM_LINK_LIBRARIES "OpenGL32.lib")
elseif(EMSCRIPTEN)
    message(STATUS "Platform: Emscripten")
    message(FATAL_ERROR "Currently not supported")
else()
    message(FATAL_ERROR "Platform: Unknown!")
endif()

# Compiler specific options
if (NOT MSVC)
    set(BASE_OPTIONS
        "-Wall"
        "-Wextra"
        "-Wconversion"
        "-Wpedantic"
        "-Wshadow"
        "-Werror"
        # fmt warnings
        "-Wno-unknown-attributes"
        # asio warnings
        "-Wno-sign-conversion"
        "-Wno-shadow"
        "-Wno-implicit-int-conversion"
        "-Wno-shorten-64-to-32"
        "-Wno-language-extension-token"
        "-Wno-conversion"
    )
else()
    set(BASE_OPTIONS
        "/W4"
        "/WX"
        "/utf-8"
        "/Zc:__cplusplus"
        #"/fsanitize=address"  # Doesn't work without Visual Studio
    )
endif()

set(HEADERS
    "src/buffer.hpp"
    "src/camera.hpp"
    "src/event.hpp"
    "src/grid.hpp"
    "src/image.hpp"
    "src/input.hpp"
    "src/luma.hpp"
    "src/mesh.hpp"
    "src/shader.hpp"
    "src/texture.hpp"
    "src/util.hpp"
    "src/window.hpp"
)
set(SOURCES
    "src/buffer.cpp"
    "src/camera.cpp"
    "src/grid.cpp"
    "src/image.cpp"
    "src/input.cpp"
    "src/main.cpp"
    "src/mesh.cpp"
    "src/shader.cpp"
    "src/texture.cpp"
    "src/util.cpp"
    "src/window.cpp"
)
add_executable(lumagl ${HEADERS} ${SOURCES})
target_include_directories(lumagl PRIVATE ${PROJECT_SOURCE_DIR})
target_compile_features(lumagl PRIVATE cxx_std_23)
target_compile_options(lumagl PRIVATE ${BASE_OPTIONS})
target_compile_definitions(lumagl PRIVATE ${BASE_DEFINITIONS})
target_link_libraries(lumagl
    PRIVATE
    ${PLATFORM_LINK_LIBRARIES}
    fmt
    $<$<NOT:$<BOOL:${EMSCRIPTEN}>>:glfw>
    $<$<NOT:$<BOOL:${EMSCRIPTEN}>>:glad::glad>
    glm
    stb
)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${HEADERS} ${SOURCES})


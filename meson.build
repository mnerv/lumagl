project('luma', 'cpp',
  version: '0.0.1',
  default_options: [
    'cpp_std=c++20',
    'warning_level=3',
    'werror=true'
  ]
)
add_project_arguments('-Wno-deprecated-volatile', language: 'cpp')

cpp = meson.get_compiler('cpp')
core_deps = []

if build_machine.system() == 'darwin'
  core_deps += [dependency('appleframeworks', modules: [
    'Cocoa',
    'IOKit',
    'CoreVideo',
    'OpenGL'
  ])]
endif

# Precompiled library
glfw_path = get_option('glfw')
glfw = declare_dependency(
  dependencies: cpp.find_library('glfw3', dirs: join_paths(glfw_path, 'build/src')),
  include_directories: join_paths(glfw_path, 'include')
)
glad_path = get_option('glad')
glad = declare_dependency(
  dependencies: cpp.find_library('glad', dirs: join_paths(glad_path, 'build')),
  include_directories: join_paths(glad_path, 'include')
)
# Header library
stb_path = get_option('stb')
stb = declare_dependency(
  include_directories: join_paths(stb_path)
)
glm_path = get_option('glm')
glm = declare_dependency(
  include_directories: join_paths(glm_path)
)

# Compile external library
imgui_path = get_option('imgui')
imgui_inc  = [
  include_directories(imgui_path),
  include_directories(join_paths(imgui_path, 'backends')),
]
imgui = declare_dependency(
  include_directories: imgui_inc,
  link_with: library(
    'imgui',
    [
      join_paths(imgui_path, 'imgui.cpp'),
      join_paths(imgui_path, 'imgui_demo.cpp'),
      join_paths(imgui_path, 'imgui_draw.cpp'),
      join_paths(imgui_path, 'imgui_tables.cpp'),
      join_paths(imgui_path, 'imgui_widgets.cpp'),

      # Backends
      join_paths(imgui_path, 'backends/imgui_impl_glfw.h'),
      join_paths(imgui_path, 'backends/imgui_impl_glfw.cpp'),
      join_paths(imgui_path, 'backends/imgui_impl_opengl3.cpp'),
    ],
    include_directories: imgui_inc,
    dependencies: [glfw] + core_deps
  ),
)

executable(
  'luma',
  [  # ls src -1 --sort=extension
    'src/buffer.hpp',
    'src/camera.hpp',
    'src/event.hpp',
    'src/grid.hpp',
    'src/image.hpp',
    'src/input.hpp',
    'src/luma.hpp',
    'src/mesh.hpp',
    'src/shader.hpp',
    'src/texture.hpp',
    'src/util.hpp',
    'src/window.hpp',

    'src/buffer.cpp',
    'src/camera.cpp',
    'src/grid.cpp',
    'src/image.cpp',
    'src/input.cpp',
    'src/main.cpp',
    'src/mesh.cpp',
    'src/shader.cpp',
    'src/texture.cpp',
    'src/util.cpp',
    'src/window.cpp',
  ],  # source files
  include_directories: [
    'src'
  ],
  dependencies: [
    glfw,
    glad,
    stb,
    glm,
    imgui,
  ] + core_deps,
  override_options: [
  ]
)


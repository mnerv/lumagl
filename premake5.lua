workspace 'helloworld'
  configurations {
    'debug',
    'release'
  }

project 'helloworld'
  kind 'ConsoleApp'
  language 'C++'
  staticruntime 'On'
  cppdialect 'C++17'

  targetdir('%{wks.location}/bin')
  objdir('%{wks.location}/obj/%{cfg.buildcfg}/%{prj.name}')

  warnings 'Extra'
  enablewarnings {
    'all'
  }

  dependson {}

  defines {}

  -- Need to be on docking branch
  local imgui = os.getenv("IMGUI_PATH")

  files {
    'src/**.h',
    'src/**.hpp',
    'src/**.cpp',
    imgui .. '/*.h',
    imgui .. '/*.cpp',
    imgui .. '/backends/imgui_impl_glfw.h',
    imgui .. '/backends/imgui_impl_glfw.cpp',
    imgui .. '/backends/imgui_impl_opengl3.cpp',
  }

  local glfw = os.getenv("GLFW_PATH")
  local glad = os.getenv("GLAD_PATH")
  local stb  = os.getenv("STB_PATH")

  includedirs {
    'src',
    imgui,
    imgui .. '/backends',
    glfw  .. '/include',
    glad  .. '/include',
    stb,
  }

  libdirs {
    glfw .. '/build/src',
    glad .. '/lib',
  }

  links {
    "glfw3",
    "glad",
  }

  filter {'not system:windows'}
    buildoptions {
      '-pedantic',
      '-Werror'
    }

  filter 'system:macosx'
    system 'macosx'

    defines {
      "GL_SILENCE_DEPRECATION"
    }

    links {
      "Cocoa.framework",
      "IOKit.framework",
      "CoreVideo.framework",
      "OpenGL.framework"
    }

  filter 'system:linx'
    system 'linux'

  filter 'system:windows'
    system 'windows'

  filter 'configurations:debug'
    symbols 'On'

  filter 'configurations:release'
    optimize 'On'


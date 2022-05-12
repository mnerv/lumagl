# luma_gl

This project is for learning purpose only.

## requirements

  - [premake5](https://premake.github.io)
  - [glfw](https://github.com/glfw/glfw)
  - [glad](https://glad.dav1d.de) or [mononerv/glad](https://github.com/mononerv/glad)
  - [stb](https://github.com/nothings/stb)
  - [glm](https://github.com/g-truc/glm)
  - [imgui](https://github.com/ocornut/imgui)

## development

Download the required dependencies above. The `glfw` and `glad` library needs to
be build first. When selecting glad make sure to select gl `Version 4.1` and
profile in `Compatibility`. The build output needs to be in `lib` library.

Define theses environment variables `GLFW_PATH`, `GLAD_PATH`, `IMGUI_PATH`,
`STB_PATH` and `GLM_PATH` that points to the correct path.

Use `premake5` to create the project, use `premake5 --help` to get the different
targets.

Example `Makefile`

```
preamek5 gmake2
```

## resources

 - [Learn OpenGL](https://learnopengl.com)
 - [Built-in Variable (GLSL)](https://www.khronos.org/opengl/wiki/Built-in_Variable_(GLSL))

### camera

 - [LearnOpenGL - Camera](https://learnopengl.com/Getting-started/Camera)
 - [OpenGL Programming/Modern OpenGL Tutorial Arcball](https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball)

### create infinite grid ground plane

 - [Infinite ground plane using GLSL shaders](https://github.com/martin-pr/possumwood/wiki/Infinite-ground-plane-using-GLSL-shaders)
 - [How to make an infinite grid.](http://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/)

### mouse pickup

 - [Screen View to World Coordinates](https://codersdesiderata.com/2016/09/10/screen-view-to-world-coordinates/)
 - [Mouse Picking with Ray Casting](https://antongerdelan.net/opengl/raycasting.html)

### shader programming

 - [The Art of Code](https://www.youtube.com/channel/UCcAlTqd9zID6aNX3TzwxJXg) - Shader programming
 - [ShaderToy](https://www.shadertoy.com) - Shader programs

### algorithms

 - [Delaunay triangulation](https://en.wikipedia.org/wiki/Bowyer–Watson_algorithm) - For mesh creation

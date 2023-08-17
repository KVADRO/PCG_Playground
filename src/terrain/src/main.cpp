#include <iostream>

#include "FrameCounter.h"
#include "Game.h"
#include "Input.h"

#include "glad/gl.h"

#include <GLFW/glfw3.h>

static void
error_callback(int error, const char* description)
{
  std::cout << error << " -> " << description << std::endl;
}

int
main(int _argc, char** _argv)
{
  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    return 0;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(800, 600, "Simple example", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return 0;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (!gladLoadGL(glfwGetProcAddress))
    return 0;

  glfwSetKeyCallback(window, Input::key_callback);
  glfwSetCursorPosCallback(window, Input::mouse_motion_callback);

  Game game;
  if (game.init()) {
    vv::FrameCounter::InitInfo frameCounterInitInfo;
    frameCounterInitInfo.targetFrameRate = 1.0f / 60.0f;

    vv::FrameCounter frameCounter;
    frameCounter.init(frameCounterInitInfo);

    while (!glfwWindowShouldClose(window)) {
      Input::clear();
      glfwPollEvents();

      frameCounter.beginFrame();
      game.update(frameCounter.getLastFrameDelta());
      frameCounter.endFrame();

      glfwSwapBuffers(window);
    }
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
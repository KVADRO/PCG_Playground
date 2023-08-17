#pragma once

#include <map>
#include <vector>

class GLFWwindow;
class Input
{
public:
  struct KeyboardEvent
  {
    int key;
    bool isPress;
  };

  static std::vector<KeyboardEvent> gKeyboardEvents;
  static void key_callback(GLFWwindow* _window,
                           int _key,
                           int _scancode,
                           int _action,
                           int _mods);

  struct MouseMotionEvent
  {
    double gPrevMouseXPos;
    double gPrevMouseYPos;

    double gMouseXPos;
    double gMouseYPos;
  };

  static std::vector<MouseMotionEvent> gMouseMotionEvents;
  static void mouse_motion_callback(GLFWwindow* _window,
                                    double _xPos,
                                    double _yPos);

  static bool isKeyPressed(int _key);

  static void clear()
  {
    gKeyboardEvents.clear();
    gMouseMotionEvents.clear();
  }

private:
  static double gPrevMouseXPos;
  static double gPrevMouseYPos;

  static double gMouseXPos;
  static double gMouseYPos;

  static std::map<int, bool> m_KeyStates;
};
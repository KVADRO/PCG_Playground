#include "Input.h"

#include <GLFW/glfw3.h>

#include <iostream>

double Input::gPrevMouseXPos = 0.0;
double Input::gPrevMouseYPos = 0.0;

double Input::gMouseXPos = 0.0;
double Input::gMouseYPos = 0.0;

std::vector<Input::KeyboardEvent> Input::gKeyboardEvents;
std::vector<Input::MouseMotionEvent> Input::gMouseMotionEvents;

std::map<int, bool> Input::m_KeyStates;

void
Input::key_callback(GLFWwindow* _window,
                    int _key,
                    int _scancode,
                    int _action,
                    int _mods)
{
  KeyboardEvent& event = gKeyboardEvents.emplace_back();
  event.key = _key;
  event.isPress = _action == GLFW_PRESS || _action == GLFW_REPEAT;

  m_KeyStates[_key] = event.isPress;
}

void
Input::mouse_motion_callback(GLFWwindow* _window, double _xPos, double _yPos)
{
  gPrevMouseXPos = gMouseXPos;
  gPrevMouseYPos = gMouseYPos;

  gMouseXPos = _xPos;
  gMouseYPos = _yPos;

  MouseMotionEvent& event = gMouseMotionEvents.emplace_back();
  event.gMouseXPos = gMouseXPos;
  event.gMouseYPos = gMouseYPos;
  event.gPrevMouseXPos = gPrevMouseXPos;
  event.gPrevMouseYPos = gPrevMouseYPos;
}

bool
Input::isKeyPressed(int _key)
{
  auto it = m_KeyStates.find(_key);
  if (it != m_KeyStates.end())
    return it->second;

  return false;
}
#include "Game.h"

std::string
ReadFile(const char* _path)
{
  std::ifstream file{ _path };
  if (!file.is_open())
    return "";

  return std::string{ std::istreambuf_iterator<char>(file),
                      std::istreambuf_iterator<char>() };
}
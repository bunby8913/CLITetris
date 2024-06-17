#include "../header/color.h"

std::string GetColorString(TetrisColorEnum color) {
  switch (color) {
  case (Cyan):
    return "\033[0;36m";
  case (Yellow):
    return "\033[1;33m";
  case (Purple):
    return "\033[0;35m";
  case (Green):
    return "\033[0;32m";
  case (Red):
    return "\033[0;31m";
  case (Blue):
    return "\033[0;34m";
  case (Orange):
    return "\033[0;33m";
  case (Reset):
    return "\033[0m";
  default:
    return "unknown";
  }
}

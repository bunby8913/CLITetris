#ifndef INITIALIZATIONHELPER
#define INITIALIZATIONHELPER

#include <termios.h>
#include <utility>
class InitializationHelper {
public:
  InitializationHelper();
  ~InitializationHelper();

  void DisableRawMode();

  void EnableRawMode();

  // Used at beginning and end to clear screen
  void ResetScreen();
  // Used every frame to refresh screen without flickering
  void ClearScreen();

  std::pair<int, int> GetWindowsSize();

private:
  struct termios originalTermios;
  static InitializationHelper *instance;
};
#endif // !INITIALIZATIONHELPER

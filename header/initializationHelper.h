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

  void ClearScreen();

  std::pair<int, int> GetWindowsSize();

private:
  struct termios originalTermios;
  static InitializationHelper *instance;
};
#endif // !INITIALIZATIONHELPER

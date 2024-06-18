#include "../header/initializationHelper.h"
#include <asm-generic/ioctls.h>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <memory.h>
#include <sys/ioctl.h>
#include <unistd.h>

InitializationHelper *InitializationHelper::instance = nullptr;

InitializationHelper::InitializationHelper() {
  ClearScreen();
  if (instance == nullptr) {
    instance = this;
    EnableRawMode();
  }
}

InitializationHelper::~InitializationHelper() {
  if (instance == this) {
    DisableRawMode();
  }
  ClearScreen();
  instance = nullptr;
}

void InitializationHelper::DisableRawMode() {
  // Restore the default terminal mode saved earlier
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios);
}

void InitializationHelper::EnableRawMode() {
  tcgetattr(STDIN_FILENO, &originalTermios);

  // Not need as the disable raw mode are managed by the destructor
  // atexit([] { instance->disableRawMode(); });
  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  raw.c_lflag &= ~(ECHO | ICANON);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

  int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}
void InitializationHelper::ResetScreen() {

#if defined(__linux__) || defined(__gnu_linux__) || defined(__LINUX__)
  system("clear");
#endif
}
void InitializationHelper::ClearScreen() {

#if defined(__linux__) || defined(__gnu_linux__) || defined(__LINUX__)
  // system("clear");
  std::cout << "\x1b[H";
#endif
}

std::pair<int, int> InitializationHelper::GetWindowsSize() {

  struct winsize size;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == -1) {
    std::cerr << "Cannot find terminal size" << std::endl;
  }
  std::cout << "Width:" << size.ws_col << std::endl;
  std::cout << "Height:" << size.ws_row << std::endl;
  return std::pair<int, int>{size.ws_col, size.ws_row};
}

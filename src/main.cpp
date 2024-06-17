#include "../header/initializationHelper.h"
#include "../header/tetrisBoard.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <unistd.h>

int main(int argc, char *argv[]) {
  std::unique_ptr<InitializationHelper> InitHelper =
      std::make_unique<InitializationHelper>();
  std::unique_ptr<TetrisBoard> tetrisBoard = std::make_unique<TetrisBoard>();
  tetrisBoard->drawBoard();
  char character;
  int characterAscii;
  auto lastTime = std::chrono::steady_clock::now();
  auto tickDuration = std::chrono::milliseconds(1000 / 30);

#if defined(__linux__) || defined(__gnu_linux__) || defined(__LINUX__)
  system("clear");
#endif

  // Hide cursor
  std::cout << "\x1b[?25l";
  // read a single byte to character, return 0 if end of file + -1 for error
  while (true) {
    if (read(STDIN_FILENO, &character, 1) == 1) {
      if (character == 'q') {
        break;
      } else {
        characterAscii = character;
        std::cout << characterAscii << std::endl;
      }
    }
    auto currentTime = std::chrono::steady_clock::now();
    if (currentTime - lastTime >= tickDuration) {
      lastTime = currentTime;
      InitHelper->ClearScreen();
      tetrisBoard->AddTetrominoToBoard(TetrisTypeEnum::J,
                                       TetrisColorEnum::Blue);
      tetrisBoard->drawBoard();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
#if defined(__linux__) || defined(__gnu_linux__) || defined(__LINUX__)
  system("clear");
  // Show cursor
  std::cout << "\x1b[?25h";
#endif
  return 0;
}

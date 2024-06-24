#include "../header/initializationHelper.h"
#include "../header/tetrisBoard.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // The InitializationHelper helps to setup + reset the CLI to display the game
  // board
  std::unique_ptr<InitializationHelper> InitHelper =
      std::make_unique<InitializationHelper>();
  std::unique_ptr<TetrisBoard> tetrisBoard = std::make_unique<TetrisBoard>();
  tetrisBoard->drawBoard();
  char character;
  int characterAscii;
  auto lastTime = std::chrono::steady_clock::now();
  auto tickDuration = std::chrono::milliseconds(1000 / 60);
  // Use a counter to determine how many frames before the block moves down by 1
  int BASELINE = 20;
  int currentBaseline = 0;

  InitHelper->ResetScreen();
  // Hide cursor
  std::cout << "\x1b[?25l";
  tetrisBoard->SpawnTetromino();
  while (true) {
    // read a single byte to character, return 0 if end of file + -1 for error
    if (read(STDIN_FILENO, &character, 1) == 1) {
      if (character == 'q') {
        break;
      } else {
        characterAscii = character;
        tetrisBoard->ProcessPlayerInput(characterAscii);
      }
    }
    auto currentTime = std::chrono::steady_clock::now();
    if (currentTime - lastTime >= tickDuration) {
      lastTime = currentTime;
      ++currentBaseline;
      if (currentBaseline == BASELINE) {
        tetrisBoard->UpdateBoard();
        currentBaseline = 0;
      }
      InitHelper->ClearScreen();
      tetrisBoard->drawBoard();
    }
    // Sleep for 1 milliseconds before processing the next frame
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  InitHelper->ResetScreen();
  //  Bring back cursor
  std::cout << "\x1b[?25h";
  return 0;
}

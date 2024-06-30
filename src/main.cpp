#include "../header/initializationHelper.h"
#include "../header/tetrisBoard.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <unistd.h>
#include <vector>

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
  std::vector<double> levelSpeeds = {
      1.00000, 1.00000, 0.79300, 0.61780, 0.47273, 0.35520, 0.26200,
      0.18968, 0.13473, 0.09388, 0.06415, 0.04298, 0.02822, 0.01815,
      0.01144, 0.00706, 0.00426, 0.00252, 0.00146, 0.00082, 0.00046};
  int currentTick = 0;
  int softDropCounter = 0;
  int softDropLimit = 4;

  InitHelper->ResetScreen();
  // Hide cursor
  std::cout << "\x1b[?25l";
  tetrisBoard->SpawnTetromino();
  while (true) {
    // read a single byte to character, return 0 if end of file + -1 for error
    if (read(STDIN_FILENO, &character, 1) == 1) {
      if (character == 'q') {
        break;
      } else if (character == 'r' && tetrisBoard->GetIsGameEnd()) {
        tetrisBoard->ResetBoard();
        tetrisBoard->SetIsGameEnd(false);
      } else {
        characterAscii = character;
        tetrisBoard->ProcessPlayerInput(characterAscii);
      }
    }
    if (tetrisBoard->GetIsGameEnd()) {
      InitHelper->ClearScreen();
      std::cout << "You lost, pres 'r' to try againg.   " << std::endl;
    } else {
      auto currentTime = std::chrono::steady_clock::now();
      if (currentTime - lastTime >= tickDuration) {
        lastTime = currentTime;
        ++currentTick;
        int currentLevel = 0;

        if (tetrisBoard->GetIsSoftDropping()) {
          if (softDropCounter < softDropLimit) {
            currentLevel = 10;
          } else {
            std::cout << "done" << std::endl;
            softDropCounter = 0;
            currentLevel = 2;
            tetrisBoard->SetisSoftDropping(false);
          }
        } else {
          currentLevel = 2;
        }
        // Compare the current tick with how many ticks required to update
        if (currentTick >= static_cast<int>(levelSpeeds[currentLevel] * 60)) {
          tetrisBoard->UpdateBoard();
          if (tetrisBoard->GetIsSoftDropping())
            ++softDropCounter;
          currentTick = 0;
        }
        InitHelper->ClearScreen();
        tetrisBoard->drawBoard();
      }
    }
    // Sleep for 1 milliseconds before processing the next frame
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  InitHelper->ResetScreen();
  //  Bring back cursor
  std::cout << "\x1b[?25h";
  return 0;
}

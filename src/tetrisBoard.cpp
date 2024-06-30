#include "../header/tetrisBoard.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

TetrisBoard::TetrisBoard()
    : board(ROW_SIZE, std::vector<std::pair<bool, TetrisColorEnum>>(
                          COLUMN_SIZE, std::pair<bool, TetrisColorEnum>{
                                           false, TetrisColorEnum::Reset})) {
  InitializeBoarder();
}

TetrisBoard::~TetrisBoard() {}

void TetrisBoard::InitializeBoarder() {
  for (int i = 0; i < COLUMN_SIZE; ++i) {
    board[0][i].first = true;
    board[board.size() - 1][i].first = true;
  }
  for (int i = 1; i < ROW_SIZE; ++i) {
    board[i][0].first = true;
    board[i][board[0].size() - 1].first = true;
  }
  // Generate the next 14 tetromino order before the game start
  mainBag = GenerateTetrominoBag();
  reserveBag = GenerateTetrominoBag();
}

void TetrisBoard::drawBoard() {
  // Only the visible rows should be drawn
  for (int i = DRAW_LIMIT; i < board.size(); ++i) {
    // This draws a margin from the left side of the CLI
    std::cout << "      ";
    for (int j = 0; j < board[0].size(); ++j) {
      if (board[i][j].first) {
        std::cout << GetColorString(board[i][j].second) + "\u2588\u2588";
      } else {
        std::cout << "  ";
      }
    }
    if (i == DRAW_LIMIT) {
      std::cout << "  NEXT";
      // draw Next Tetromino
    } else if (i < DRAW_LIMIT + 5) {
      if (nextTetromino != nullptr) {
        std::vector<std::vector<bool>> next = nextTetromino->GetTetrisMatrix();
        TetrisColorEnum color = nextTetromino->GetTetrisColor();
        std::cout << "  ";
        for (int j = 0; j < next[0].size(); ++j) {
          if (next[i - (DRAW_LIMIT + 1)][j]) {
            std::cout << GetColorString(color) + "\u2588\u2588";
          } else {
            std::cout << "  ";
          }
        }
      }
    } else if (i == DRAW_LIMIT + 5) {
      std::cout << "  HOLD";
    } else if (i < DRAW_LIMIT + 10) {
      if (holdTetromino != nullptr) {
        auto next = holdTetromino->GetTetrisMatrix();
        TetrisColorEnum color = holdTetromino->GetTetrisColor();
        std::cout << "  ";
        for (int j = 0; j < next[0].size(); ++j) {
          if (next[i - (DRAW_LIMIT + 6)][j]) {
            std::cout << GetColorString(color) + "\u2588\u2588";
          } else {
            std::cout << "  ";
          }
        }
      }
    }
    std::cout << std::endl;
  }
  std::cout << GetColorString(TetrisColorEnum::Reset);
  std::cout << "Score: " << score << std::endl;
  std::cout << "lvl: " << level << std::endl;
}

void TetrisBoard::UpdateBoard() {
  if (currentTetromino != nullptr) {

    std::pair<int, int> matrixLocation = currentTetromino->GetTetrisLocation();
    std::vector<std::vector<bool>> currentMatrix =
        currentTetromino->GetTetrisMatrix();
    for (int i = 0; i < currentMatrix[0].size(); ++i) {
      // if that bottom grid is occupied
      for (int j = currentMatrix.size() - 1; j >= 0; --j) {
        if (currentMatrix[j][i]) {

          int row = std::min(static_cast<int>(board.size()),
                             matrixLocation.first + (j + 1));
          int column = std::min(static_cast<int>(board[0].size()),
                                matrixLocation.second + i);
          if (board[row][column].first) {
            LockCurrentTetromino();
            return;
          }
          break;
        }
      }
    }
    // Update score if the board update is resulted by soft dropping
    if (isSoftDropping) {
      score += 1;
    }
    if (matrixLocation.first + 4 < (ROW_SIZE - 1)) {
      EraseTetrominoOnBoard();
      matrixLocation.first += 1;
      currentTetromino->SetTetrisLocation(matrixLocation);
      DrawTetrominoOnBoard();
    } else {
      LockCurrentTetromino();
    }
  }
}
void TetrisBoard::DropTetromino() {
  if (currentTetromino) {
    // Get the current tetromino location + shape
    std::pair<int, int> matrixLocation = currentTetromino->GetTetrisLocation();
    std::vector<std::vector<bool>> currentMatrix =
        currentTetromino->GetTetrisMatrix();
    // Used to stop loop conditions
    bool canMoveDown = true;
    // Used to store how many rows the tetromino should move down
    int dropDownRows = 0;
    while (canMoveDown) {
      for (int i = 0; i < currentMatrix[0].size(); ++i) {
        for (int j = currentMatrix.size() - 1; j >= 0; --j) {
          if (currentMatrix[j][i]) {
            int row = std::min(static_cast<int>(board.size()),
                               matrixLocation.first + j + 1 + dropDownRows);
            int column = std::min(static_cast<int>(board[0].size()),
                                  matrixLocation.second + i);
            if (board[row][column].first) {
              // The drop should end here
              canMoveDown = false;
            }
            break;
          }
        }
      }
      if (canMoveDown) {
        ++dropDownRows;
      }
    }
    if (matrixLocation.first + 4 < (ROW_SIZE - 1)) {
      EraseTetrominoOnBoard();
      matrixLocation.first += dropDownRows;
      // Use the hardDropLineCounter for score calculation
      hardDropLineCounter = dropDownRows;
      currentTetromino->SetTetrisLocation(matrixLocation);
      DrawTetrominoOnBoard();
    } else {
      LockCurrentTetromino();
    }
    UpdateBoard();
  }
}

void TetrisBoard::HoldTetromino() {
  if (canHold) {
    EraseTetrominoOnBoard();
    if (holdTetromino) {
      nextTetromino = std::move(holdTetromino);
    }
    holdTetromino = std::move(currentTetromino);
    SpawnTetromino(true);
    canHold = false;
  }
}

void TetrisBoard::SpawnTetromino(bool spawnFromHolding) {
  // When the game first starts
  if (spawnFromHolding) {
    currentTetromino = std::move(nextTetromino);
  } else {
    if (nextTetromino)
      currentTetromino = std::move(nextTetromino);
    else
      currentTetromino = std::move(mainBag[0]);
  }
  currentTetromino->SetTetrisLocation(spawnLocation);
  CheckIfGameShouldEnd();
  DrawTetrominoOnBoard();

  // If we have gone through the main bag
  if (bagCounter == mainBag.size() - 1) {
    for (int i = 0; i < mainBag.size(); ++i) {
      mainBag[i] = std::move(reserveBag[i]);
    }
    reserveBag = GenerateTetrominoBag();
  }
  if (bagCounter < mainBag.size() - 1) {
    nextTetromino = std::move(mainBag[bagCounter + 1]);
  } else {
    nextTetromino = std::move(mainBag[0]);
  }
  ++bagCounter;
  bagCounter %= mainBag.size();
}

void TetrisBoard::DrawTetrominoOnBoard() {
  if (currentTetromino) {
    std::pair<int, int> currentLocation = currentTetromino->GetTetrisLocation();
    std::vector<std::vector<bool>> tetrominoMatrix =
        currentTetromino->GetTetrisMatrix();
    for (int i = 0; i < tetrominoMatrix.size(); ++i) {
      for (int j = 0; j < tetrominoMatrix[0].size(); ++j) {
        if (tetrominoMatrix[i][j]) {
          int row = currentLocation.first + i;
          int column = currentLocation.second + j;
          board[row][column].first = true;
          board[row][column].second = currentTetromino->GetTetrisColor();
        }
      }
    }
  }
}

std::vector<std::unique_ptr<Tetromino>> TetrisBoard::GenerateTetrominoBag() {
  std::vector<TetrisTypeEnum> typeBag;
  while (typeBag.size() < 7) {
    TetrisTypeEnum tempType = Tetromino::GetNextType();
    if (std::find(typeBag.begin(), typeBag.end(), tempType) == typeBag.end()) {
      typeBag.push_back(tempType);
    }
  }
  std::vector<std::unique_ptr<Tetromino>> tetrominoBag;
  for (const auto &type : typeBag) {
    tetrominoBag.push_back(
        std::make_unique<Tetromino>(type, Tetromino::GetColorBaseOnType(type)));
  }
  return tetrominoBag;
}

void TetrisBoard::EraseTetrominoOnBoard() {
  if (currentTetromino) {
    std::pair<int, int> currentLocation = currentTetromino->GetTetrisLocation();
    std::vector<std::vector<bool>> tetrominoMatrix =
        currentTetromino->GetTetrisMatrix();
    for (int i = 0; i < tetrominoMatrix.size(); ++i) {
      for (int j = 0; j < tetrominoMatrix[0].size(); ++j) {
        if (tetrominoMatrix[i][j]) {
          int row = currentLocation.first + i;
          int column = currentLocation.second + j;
          board[row][column].first = false;
          board[row][column].second = TetrisColorEnum::Reset;
        }
      }
    }
  }
}

void TetrisBoard::ProcessPlayerInput(int characterAscii) {
  // left input
  if (characterAscii == 68) {
    MoveTetrominoLeft();
  }
  // right input
  else if (characterAscii == 67) {
    MoveTetrominoRight();
  }
  // Down input
  else if (characterAscii == 66) {
    isSoftDropping = true;
  }
  // Z to rotate left
  else if (characterAscii == 122) {
    EraseTetrominoOnBoard();
    if (currentTetromino) {
      currentTetromino->RotatePatternLeft();
      auto currentPattern = currentTetromino->GetTetrisMatrix();
      auto currentLocation = currentTetromino->GetTetrisLocation();
      bool badRotation = false;
      for (int i = 0; i < currentPattern.size(); ++i) {
        for (int j = 0; j < currentPattern[0].size(); ++j) {
          if (currentPattern[i][j]) {
            // If the block is already occupied, revert the rotation
            if (board[currentLocation.first + i][currentLocation.second + j]
                    .first) {
              badRotation = true;
              break;
            }
          }
        }
      }
      if (badRotation) {
        currentTetromino->RotatePatternRight();
      }
    }
    // Check if there will be any collision, if so, revert rotation back
    DrawTetrominoOnBoard();
    // Up arrow of X to rotate right
  } else if (characterAscii == 120 || characterAscii == 65) {
    EraseTetrominoOnBoard();
    if (currentTetromino) {
      currentTetromino->RotatePatternRight();
      auto currentPattern = currentTetromino->GetTetrisMatrix();
      auto currentLocation = currentTetromino->GetTetrisLocation();
      bool badRotation = false;
      for (int i = 0; i < currentPattern.size(); ++i) {
        for (int j = 0; j < currentPattern[0].size(); ++j) {
          if (currentPattern[i][j]) {
            // If the block is already occupied, revert the rotation
            if (board[currentLocation.first + i][currentLocation.second + j]
                    .first) {
              badRotation = true;
              break;
            }
          }
        }
      }
      if (badRotation) {
        currentTetromino->RotatePatternLeft();
      }
    }

    DrawTetrominoOnBoard();
  }
  // 99 to hold
  else if (characterAscii == 99) {
    HoldTetromino();
  }
  // 32 (space) to drop the block
  else if (characterAscii == 32) {
    DropTetromino();
  }

  else {
    // std::cout << characterAscii << std::endl;
  }
}

void TetrisBoard::MoveTetrominoLeft() {
  if (currentTetromino) {
    std::pair<int, int> currentLocation = currentTetromino->GetTetrisLocation();
    if (currentLocation.second > 1) {
      // Figure out there are anything blocking the tetromine from moving left
      // for each row, find the leftest element, see if there are anything on
      // the left of them
      auto tetrominoMatrix = currentTetromino->GetTetrisMatrix();
      for (int i = 0; i < tetrominoMatrix.size(); ++i) {
        for (int j = 0; j < tetrominoMatrix[0].size(); ++j) {
          if (tetrominoMatrix[i][j]) {
            int row = currentLocation.first + i;
            int column = std::max(0, currentLocation.second + j - 1);
            if (board[row][column].first) {
              return;
            }
            break;
          }
        }
      }
      EraseTetrominoOnBoard();
      --currentLocation.second;
      currentTetromino->SetTetrisLocation(currentLocation);
      DrawTetrominoOnBoard();
    }
  }
}

void TetrisBoard::MoveTetrominoRight() {
  if (currentTetromino) {
    std::pair<int, int> currentLocation = currentTetromino->GetTetrisLocation();
    if (currentLocation.second < COLUMN_SIZE - 2) {
      // Figure out if there are anything blocking from the right
      // for each row, find the leftest element, see if there are anything on
      // the left of them
      auto tetrominoMatrix = currentTetromino->GetTetrisMatrix();
      for (int i = 0; i < tetrominoMatrix.size(); ++i) {
        for (int j = tetrominoMatrix[0].size() - 1; j >= 0; --j) {
          if (tetrominoMatrix[i][j]) {
            int row = currentLocation.first + i;
            int column = currentLocation.second + j + 1;
            if (board[row][column].first) {
              return;
            }
            break;
          }
        }
      }
      EraseTetrominoOnBoard();
      ++currentLocation.second;
      currentTetromino->SetTetrisLocation(currentLocation);
      DrawTetrominoOnBoard();
    }
  }
}

void TetrisBoard::CheckAndRemoveCompletedLines() {
  int removedLines = 0;
  for (int i = ROW_SIZE - 2; i > DRAW_LIMIT; --i) {
    bool completedRow = true;
    for (int j = 0; j < board[0].size(); ++j) {
      if (!board[i][j].first) {
        completedRow = false;
        break;
      }
    }
    if (completedRow) {
      for (int j = i; j > DRAW_LIMIT; --j) {
        board[j] = board[j - 1];
      }
      ++i;
      ++removedLines;
    }
  }
  int tempScore = 0;
  switch (removedLines) {
  case 1:
    tempScore = (100 * level);
    break;
  case 2:
    tempScore = (300 * level);
    break;
  case 3:
    tempScore = (500 * level);
    break;
  case 4:
    break;
    tempScore = (800 * level);
    break;
  default:
    tempScore += 0;
  }
  // Special case of back to back tetris
  if (lastLinesCleared == 4 && removedLines == 4) {
    tempScore = 1200 * level;
  }
  std::cout << tempScore << std::endl;
  lastLinesCleared = removedLines;
  tempScore += hardDropLineCounter * 2;
  //  Reset hard drop line counter to 0
  hardDropLineCounter = 0;
  // Update the current score and level
  score += tempScore;
  totalClearedLineCounter += removedLines;
  // Use fix goal, level up every 10 lines gets cleared
  level += totalClearedLineCounter / 10;
  totalClearedLineCounter %= 10;
}

void TetrisBoard::LockCurrentTetromino() {
  currentTetromino = nullptr;
  CheckAndRemoveCompletedLines();
  SpawnTetromino();
  canHold = true;
}

void TetrisBoard::CheckIfGameShouldEnd() {
  // Determine if there are any collisions when spawning new Tetromion, if so,
  // end the game
  if (currentTetromino) {
    auto tetrominoPattern = currentTetromino->GetTetrisMatrix();
    for (int i = 0; i < tetrominoPattern.size(); ++i) {
      for (int j = 0; j < tetrominoPattern[0].size(); ++j) {
        if (tetrominoPattern[i][j]) {
          int row = spawnLocation.first + i;
          int column = spawnLocation.second + j;
          if (board[row][column].first) {
            // end game
            endGame = true;
          }
        }
      }
    }
  }
}

void TetrisBoard::ResetBoard() {
  std::cout << "reset board called" << std::endl;
  for (int i = 0; i < board.size(); ++i) {
    for (int j = 0; j < board[0].size(); ++j) {
      board[i][j].first = false;
      board[i][j].second = TetrisColorEnum::Reset;
    }
  }
  score = 0;
  level = 1;
  InitializeBoarder();
}

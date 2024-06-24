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
  std::cout << "Score:" << score << std::endl;
  std::cout << "lvl:" << std::endl;
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
      currentTetromino->SetTetrisLocation(matrixLocation);
      DrawTetrominoOnBoard();
    } else {
      LockCurrentTetromino();
    }
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
    nextTetromino->SetTetrisLocation(spawnLocation);
    currentTetromino = std::move(nextTetromino);
  } else if (nextTetromino) {
    currentTetromino = std::move(nextTetromino);
    currentTetromino->SetTetrisLocation(spawnLocation);
    DrawTetrominoOnBoard();
  } else {
    TetrisTypeEnum currentType = Tetromino::GetNextType();
    TetrisColorEnum currentColor = Tetromino::GetColorBaseOnType(currentType);
    currentTetromino = std::make_unique<Tetromino>(currentType, currentColor);
    currentTetromino->SetTetrisLocation(spawnLocation);
    DrawTetrominoOnBoard();
  }
  // Create the next Tetromino, so the player can know what's coming next
  TetrisTypeEnum nextType = Tetromino::GetNextType();
  TetrisColorEnum nextColor = Tetromino::GetColorBaseOnType(nextType);
  nextTetromino = std::make_unique<Tetromino>(nextType, nextColor);
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
  switch (removedLines) {
  case 1:
    score += 100;
    break;
  case 2:
    score += 300;
    break;
  case 3:
    score += 500;
    break;
  case 4:
    break;
    score += 800;
    break;
  default:
    score += 0;
  }
}

void TetrisBoard::LockCurrentTetromino() {
  currentTetromino = nullptr;
  CheckAndRemoveCompletedLines();
  SpawnTetromino();
  canHold = true;
}

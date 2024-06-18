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
    std::cout << std::endl;
  }
  std::cout << GetColorString(TetrisColorEnum::Reset);
}

void TetrisBoard::UpdateBoard() {
  if (currentTetromino != nullptr) {

    std::pair<int, int> matrixLocation = currentTetromino->GetTetrisLocation();
    std::vector<std::vector<bool>> currentMatrix =
        currentTetromino->GetTetrisMatrix();
    for (int i = 0; i < currentMatrix[0].size(); ++i) {
      // if that bottom grid is occupied
      if (currentMatrix[3][i]) {
        // if there are blocks underneath that are true, lock to current
        // position
        int row =
            std::min(static_cast<int>(board.size()), matrixLocation.first + 4);
        int column = std::min(static_cast<int>(board[0].size()),
                              matrixLocation.second + i);
        if (board[row][column].first == true) {
          LockCurrentTetromino();
          return;
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

void TetrisBoard::SpawnTetromino() {
  std::pair<int, int> spawnLocation = {18, 5};

  // When the game first starts
  if (!currentTetromino) {
    TetrisTypeEnum currentType = Tetromino::GetNextType();
    TetrisColorEnum currentColor = Tetromino::GetColorBaseOnType(currentType);
    currentTetromino = std::make_unique<Tetromino>(currentType, currentColor);
    currentTetromino->SetTetrisLocation(spawnLocation);
    DrawTetrominoOnBoard();
    // Else, use the next tetromino
  } else {
    if (nextTetromino) {
      currentTetromino = std::move(nextTetromino);
    }
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
  // 122 to rotate left
  else if (characterAscii == 122) {
    EraseTetrominoOnBoard();
    currentTetromino->RotatePatternLeft();
    DrawTetrominoOnBoard();
    // c to rotate right
  } else if (characterAscii == 99) {
    EraseTetrominoOnBoard();
    currentTetromino->RotatePatternRight();
    DrawTetrominoOnBoard();

  }
  // 99 is c
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

int TetrisBoard::CheckForCompletedLines() {
  int completedLine = 0;
  for (int i = ROW_SIZE - 2; i > DRAW_LIMIT; --i) {
    for (int j = 0; j < board[0].size(); ++j) {
      if (!board[i][j].first) {
        return completedLine;
      }
    }
    ++completedLine;
  }
  return completedLine;
}

void TetrisBoard::RemoveCompletedLines(int lines) {
  // Start from the bottom of the board and move upwards
  for (int i = ROW_SIZE - 2; i >= lines; --i) {
    for (int j = 0; j < board[0].size(); ++j) {
      // Move the line `lines` rows up
      board[i][j] = board[i - lines][j];
    }
  }

  // Clear the top `lines` rows
  for (int i = lines - 1; i >= 0; --i) {
    for (int j = 0; j < board[0].size(); ++j) {
      board[i][j].first = false;
      board[i][j].second = TetrisColorEnum::Reset;
    }
  }
}

void TetrisBoard::LockCurrentTetromino() {
  currentTetromino = nullptr;
  int linesToErase = CheckForCompletedLines();
  if (linesToErase > 0) {
    RemoveCompletedLines(linesToErase);
  }
  SpawnTetromino();
}

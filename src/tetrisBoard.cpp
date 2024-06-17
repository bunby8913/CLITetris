#include "../header/tetrisBoard.h"
#include <iostream>
#include <memory>
#include <utility>

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
    std::cout << "      ";
    for (int j = 0; j < board[0].size(); ++j) {
      if (board[i][j].first == true) {
        std::cout << GetColorString(board[i][j].second) + "\u2588\u2588";
      } else {
        std::cout << "  ";
      }
    }
    std::cout << std::endl;
  }
  std::cout << GetColorString(Reset);
}

void TetrisBoard::AddTetrominoToBoard(TetrisTypeEnum type,
                                      TetrisColorEnum color,
                                      std::pair<int, int> defaultLocation) {
  currentTetromino = std::make_unique<Tetromino>(type, color);
  currentTetromino->SetTetrisLocation(defaultLocation);
  std::vector<std::vector<bool>> tetrominoMatrix =
      currentTetromino->GetTetrisMatrix();
  for (int i = 0; i < tetrominoMatrix.size(); ++i) {
    for (int j = 0; j < tetrominoMatrix[0].size(); ++j) {
      if (tetrominoMatrix[i][j]) {
        int row = defaultLocation.first + i;
        int column = defaultLocation.second + j;
        board[row][column].first = true;
        board[row][column].second = color;
      }
    }
  }
}

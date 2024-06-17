#include "../header/tetromino.h"
#include <memory>
#include <utility>
#include <vector>

const int DRAW_LIMIT = 22;
const int ROW_SIZE = 42;
const int COLUMN_SIZE = 12;

class TetrisBoard {
public:
  TetrisBoard();
  ~TetrisBoard();

  void drawBoard();
  void InitializeBoarder();

  void UpdateBoard();

  // Create a tetromino of the specific type + color, add it to tetrisBoard
  void AddTetrominoToBoard(TetrisTypeEnum type, TetrisColorEnum color,
                           std::pair<int, int> defaultLocation = {35, 5});

private:
  std::vector<std::vector<std::pair<bool, TetrisColorEnum>>> board;
  int currentLevel;
  std::unique_ptr<Tetromino> currentTetromino = nullptr;
};

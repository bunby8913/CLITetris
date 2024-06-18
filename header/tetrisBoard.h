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

  // Boards are updated by moving the current Tetromino down by 1 block
  void UpdateBoard();

  // Create a tetromino of the specific type + color, add it to tetrisBoard
  void SpawnTetromino(TetrisTypeEnum type, TetrisColorEnum color,
                      std::pair<int, int> defaultLocation = {23, 5});

  void DrawTetrominoOnBoard();
  void EraseTetrominoOnBoard();
  void ProcessPlayerInput(int characterAscii);

  void MoveTetrominoLeft();
  void MoveTetrominoRight();

  void LockCurrentTetromino();

private:
  std::vector<std::vector<std::pair<bool, TetrisColorEnum>>> board;
  int currentLevel;
  std::unique_ptr<Tetromino> currentTetromino = nullptr;
};

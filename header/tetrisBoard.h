#include "../header/tetromino.h"
#include <memory>
#include <utility>
#include <vector>

const int DRAW_LIMIT = 20;
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

  void DropTetromino();

  void HoldTetromino();
  // Create a tetromino of the specific type + color, add it to tetrisBoard
  void SpawnTetromino(bool spawnFromHolding = false);

  std::vector<std::unique_ptr<Tetromino>> GenerateTetrominoBag();

  void DrawTetrominoOnBoard();
  void EraseTetrominoOnBoard();
  void ProcessPlayerInput(int characterAscii);

  void MoveTetrominoLeft();
  void MoveTetrominoRight();

  // Lock is also responsible to generate the next tetromino + end game logic
  // We should also check if we can remove lines here
  void LockCurrentTetromino();

  void CheckAndRemoveCompletedLines();

  void CheckIfGameShouldEnd();

  bool GetIsGameEnd() { return endGame; };
  void SetIsGameEnd(bool gameEnd) { endGame = gameEnd; };

  int GetCurrentLevel() { return level; };

  bool GetIsSoftDropping() { return isSoftDropping; };

  void SetisSoftDropping(bool newValue) { isSoftDropping = newValue; };

  void ResetBoard();

private:
  std::vector<std::vector<std::pair<bool, TetrisColorEnum>>> board;
  int currentLevel;
  std::vector<std::unique_ptr<Tetromino>> mainBag;
  std::vector<std::unique_ptr<Tetromino>> reserveBag;
  int bagCounter = 0;
  std::unique_ptr<Tetromino> currentTetromino = nullptr;
  std::unique_ptr<Tetromino> nextTetromino = nullptr;
  std::unique_ptr<Tetromino> holdTetromino = nullptr;
  bool canHold = true;
  std::pair<int, int> spawnLocation = {18, 5};
  int score = 0;
  int level = 1;
  bool endGame = false;
  int lastLinesCleared = 0;
  int hardDropLineCounter = 0;
  int totalClearedLineCounter = 0;
  bool isSoftDropping = false;
};

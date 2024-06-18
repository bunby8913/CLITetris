#ifndef TETROMINO_H
#define TETROMINO_H

#include "../header/color.h"
#include <utility>
#include <vector>
enum TetrisTypeEnum { I, J, L, O, S, T, Z };
enum TetrisRotationEnum { Up, Right, Down, Left };

class Tetromino {
public:
  Tetromino();
  Tetromino(TetrisTypeEnum type, TetrisColorEnum color);
  Tetromino(Tetromino &&) = default;
  Tetromino(const Tetromino &) = default;
  Tetromino &operator=(Tetromino &&) = default;
  Tetromino &operator=(const Tetromino &) = default;
  ~Tetromino();

  void GeneratePattern(TetrisTypeEnum type);

  void RotatePatternRight();
  void RotatePatternLeft();
  std::pair<int, int> GetTetrisLocation() const { return tetrisLocation; };
  void SetTetrisLocation(std::pair<int, int> location) {
    tetrisLocation = location;
  };
  std::vector<std::vector<bool>> GetTetrisMatrix() const {
    return tetrisPattern;
  };
  TetrisColorEnum GetTetrisColor() const { return tetrisColor; };

  int GetRightEdge() const { return rightMatrixEdge; };
  int GetTopEdge() const { return topMatrixEdge; };

private:
  // Store the location of the tetris
  // This should always be the location of the top-left corner of the matrix
  std::pair<int, int> tetrisLocation;
  TetrisRotationEnum tetrisRotation;
  int tetrisRotationIndex;
  TetrisColorEnum tetrisColor;
  // Pattern should always be contained in a 4 by 4 matrix
  std::vector<std::vector<bool>> tetrisPattern{4, std::vector<bool>(4)};

  // Indicate the tetromino covers this many blocks to the right
  int rightMatrixEdge;
  // Indicate the tetromino covers this many blocks from the bottom
  int topMatrixEdge;
};
#endif // !TETROMINO_H

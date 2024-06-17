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

private:
  // Store the location of the tetris
  // This should always be the location of the top-left corner of the matrix
  std::pair<int, int> tetrisLocation;
  TetrisRotationEnum tetrisRotation;
  int tetrisRotationIndex;
  TetrisColorEnum tetrisColor;
  // Pattern should always be contained in a 4 by 4 matrix
  std::vector<std::vector<bool>> tetrisPattern{4, std::vector<bool>(4)};
};
#endif // !TETROMINO_H

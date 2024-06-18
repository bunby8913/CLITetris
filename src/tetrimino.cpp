#include "../header/tetromino.h"
#include <string>
#include <vector>
Tetromino::Tetromino()
    : tetrisLocation(0, 0), tetrisRotation(TetrisRotationEnum::Up),
      tetrisColor(TetrisColorEnum::Reset), tetrisRotationIndex(0) {}

Tetromino::Tetromino(TetrisTypeEnum type, TetrisColorEnum color)
    : tetrisLocation(0, 0), tetrisRotation(TetrisRotationEnum::Up),
      tetrisColor(color), tetrisRotationIndex(0) {
  GeneratePattern(type);
}
Tetromino::~Tetromino() {}

void Tetromino::GeneratePattern(TetrisTypeEnum type) {

  std::vector<std::vector<bool>> pattern{4, std::vector<bool>(4)};
  tetrisPattern = pattern;
  switch (type) {
  case I:
    pattern[3][0] = true;
    pattern[3][1] = true;
    pattern[3][2] = true;
    pattern[3][3] = true;
    topMatrixEdge = 3;
    rightMatrixEdge = 3;
    break;
  case J:
    pattern[2][0] = true;
    pattern[3][0] = true;
    pattern[3][1] = true;
    pattern[3][2] = true;
    topMatrixEdge = 2;
    rightMatrixEdge = 2;
    break;
  case L:
    pattern[2][2] = true;
    pattern[3][0] = true;
    pattern[3][1] = true;
    pattern[3][2] = true;
    topMatrixEdge = 2;
    rightMatrixEdge = 2;
    break;
  case O:
    pattern[2][0] = true;
    pattern[3][0] = true;
    pattern[2][1] = true;
    pattern[3][1] = true;
    topMatrixEdge = 2;
    rightMatrixEdge = 1;
    break;
  case S:
    pattern[3][0] = true;
    pattern[3][1] = true;
    pattern[2][1] = true;
    pattern[2][2] = true;
    topMatrixEdge = 2;
    rightMatrixEdge = 2;
    break;
  case T:
    pattern[3][0] = true;
    pattern[3][1] = true;
    pattern[3][2] = true;
    pattern[2][1] = true;
    topMatrixEdge = 2;
    rightMatrixEdge = 2;
    break;
  case Z:
    pattern[2][0] = true;
    pattern[2][1] = true;
    pattern[3][1] = true;
    pattern[3][2] = true;
    topMatrixEdge = 2;
    rightMatrixEdge = 2;
    break;
  default:
    break;
  }

  tetrisPattern = pattern;
  // Use this to make sure the output is correct
  /*
   for (int i = 0; i < pattern.size(); ++i) {
     for (int j = 0; j < pattern[0].size(); ++j) {
       std::cout << pattern[i][j];
     }
     std::cout << std::endl;
   }
   */
}

void Tetromino::RotatePatternRight() {
  std::vector<std::vector<std::string>> newPattern;
  tetrisRotationIndex = (tetrisRotationIndex + 1) % 4;
  tetrisRotation = static_cast<TetrisRotationEnum>(tetrisRotationIndex);
}

void Tetromino::RotatePatternLeft() {
  std::vector<std::vector<std::string>> newPattern;
  tetrisRotationIndex = (tetrisRotationIndex + 1) % 4;
  tetrisRotation = static_cast<TetrisRotationEnum>(tetrisRotationIndex);
}

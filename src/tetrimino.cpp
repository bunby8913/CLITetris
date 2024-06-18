#include "../header/tetromino.h"
#include <iostream>
#include <random>
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
  tetrisRotationIndex = (tetrisRotationIndex + 1) % 4;
  tetrisRotation = static_cast<TetrisRotationEnum>(tetrisRotationIndex);
  std::vector<std::vector<bool>> tempPattern{4, std::vector<bool>(4)};
  int tempPatternColumn = 0;
  for (int i = tetrisPattern.size() - 1; i >= 0; --i) {
    for (int j = 0; j < tetrisPattern[0].size(); ++j) {
      tempPattern[j][(tetrisPattern.size() - 1) - i] = tetrisPattern[i][j];
    }
    ++tempPatternColumn;
  }
  LeftAlignPattern(tempPattern);
  BottomAlignPattern(tempPattern);
  tetrisPattern = tempPattern;
}

void Tetromino::RotatePatternLeft() {
  tetrisRotationIndex = (tetrisRotationIndex + 1) % 4;
  tetrisRotation = static_cast<TetrisRotationEnum>(tetrisRotationIndex);
  std::vector<std::vector<bool>> tempPattern{4, std::vector<bool>(4)};
  int tempPatternColumn = 0;
  for (int i = 0; i < tetrisPattern.size(); ++i) {
    for (int j = 0; j < tetrisPattern[0].size(); ++j) {
      tempPattern[(tetrisPattern.size() - 1) - j][tempPatternColumn] =
          tetrisPattern[i][j];
    }
    ++tempPatternColumn;
  }
  LeftAlignPattern(tempPattern);
  BottomAlignPattern(tempPattern);
  tetrisPattern = tempPattern;
}

void Tetromino::LeftAlignPattern(std::vector<std::vector<bool>> &pattern) {
  bool aligned = false;
  while (!aligned) {

    for (int i = 0; i < pattern.size(); ++i) {
      aligned = aligned || pattern[i][0];
    }
    if (!aligned) {
      LeftAlignByOne(pattern);
    }
  }
}

void Tetromino::LeftAlignByOne(std::vector<std::vector<bool>> &pattern) {
  std::vector<std::vector<bool>> tempPattern{4, std::vector<bool>(4)};
  for (int i = 0; i < pattern.size(); ++i) {
    for (int j = 0; j < pattern[0].size(); ++j) {
      if (pattern[i][j]) {
        if (j > 0)
          tempPattern[i][j - 1] = pattern[i][j];
      }
    }
  }
  pattern = tempPattern;
}

void Tetromino::BottomAlignPattern(std::vector<std::vector<bool>> &pattern) {

  bool aligned = false;
  while (!aligned) {

    for (int i = 0; i < pattern[0].size(); ++i) {
      aligned = aligned || pattern[pattern.size() - 1][i];
    }
    if (!aligned) {
      BottomAlignByOne(pattern);
    }
  }
}

void Tetromino::BottomAlignByOne(std::vector<std::vector<bool>> &pattern) {
  std::vector<std::vector<bool>> tempPattern{4, std::vector<bool>(4)};
  for (int i = 0; i < pattern.size(); ++i) {
    for (int j = 0; j < pattern[0].size(); ++j) {
      if (pattern[i][j]) {
        if (i < pattern.size() - 1)
          tempPattern[i + 1][j] = pattern[i][j];
      }
    }
  }
  pattern = tempPattern;
}

TetrisTypeEnum Tetromino::GetNextType() {
  // Use the random library to produce random number
  std::default_random_engine generator(std::random_device{}());
  std::uniform_int_distribution<int> distribution(0, TetrisTypeEnum::Z);
  int random = distribution(generator);
  std::cout << random << std::endl;

  return static_cast<TetrisTypeEnum>(random);
}

TetrisColorEnum Tetromino::GetColorBaseOnType(TetrisTypeEnum tetrisType) {
  TetrisColorEnum color;
  switch (tetrisType) {
  case I:
    color = TetrisColorEnum::Cyan;
    break;
  case J:
    color = TetrisColorEnum::Blue;
    break;
  case L:
    color = TetrisColorEnum::Orange;
    break;
  case O:
    color = TetrisColorEnum::Yellow;
    break;
  case S:
    color = TetrisColorEnum::Green;
    break;
  case T:
    color = TetrisColorEnum::Purple;
    break;
  case Z:
    color = TetrisColorEnum::Red;
    break;
  default:
    color = TetrisColorEnum::Reset;
  }
  return color;
}

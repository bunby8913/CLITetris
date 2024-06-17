#include <string>
enum TetrisColorEnum { Cyan, Yellow, Purple, Green, Red, Blue, Orange, Reset };
// Given the color enum, return the string that changes the color of ASCII
// character in CLI
std::string GetColorString(TetrisColorEnum color);

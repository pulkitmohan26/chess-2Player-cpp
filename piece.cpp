#include "piece.hpp"
#include <cctype>
#include <string>
using namespace std;

// Default constructor
Piece::Piece() : type(EMPTY), color(WHITE) {}

// Parameterized constructor
Piece::Piece(PieceType t, Color c) : type(t), color(c) {}

string Piece::symbol() const {
    if (type == EMPTY) return " ";
    char chars[] = {'K', 'Q', 'R', 'B', 'N', 'P'};
    char s = chars[type];
    return string(1, s);
}
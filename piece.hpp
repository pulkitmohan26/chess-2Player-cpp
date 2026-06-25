#pragma once
#include <string>

enum PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, EMPTY };
enum Color { WHITE, BLACK };

class Piece {
public:
    PieceType type;
    Color color;

    Piece();
    Piece(PieceType t, Color c);
    std::string symbol() const;
};

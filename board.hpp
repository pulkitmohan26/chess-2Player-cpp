#pragma once
#include "piece.hpp"
#include <vector>
#include <tuple>

class Board {
public:
    Piece board[8][8];
    Color turn;

    Board();

    void print() const;
    bool inBoard(int x, int y) const;
    bool move(int x1, int y1, int x2, int y2);

    // --- Add these lines ---
    std::vector<std::tuple<int, int, int, int>> getLegalMoves(Color color) const;
    std::vector<std::pair<int, int>> getLegalMovesForPiece(int x, int y) const;
    int evaluate() const;
    bool isKingInCheck(Color color) const;
    void moveWithoutTurnChange(int x1, int y1, int x2, int y2);
    bool isCheckmate(Color color) const;
    bool isStalemate(Color color) const;
    // --- End additions ---
};

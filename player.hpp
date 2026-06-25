#pragma once
#include "board.hpp"

class Player {
public:
    Color color;
    Player(Color c) : color(c) {}
    virtual bool makeMove(Board& board, std::string& moveNotation) = 0; // changed
};

class HumanPlayer : public Player {
public:
    HumanPlayer(Color c);
     bool makeMove(Board& board, std::string& moveNotation) override;
};

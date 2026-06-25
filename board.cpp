#include "board.hpp"
#include <iostream>
using namespace std;

#define RESET   "\033[0m"
#define B_WHITE "\033[47m"
#define B_BLACK "\033[100m"
#define FG_W    "\033[30m"   // Black text for white pieces
#define FG_B    "\033[31m"   // Red text for black pieces

Board::Board() : turn(WHITE) {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            board[i][j] = Piece();

    // Pawns
    for (int j = 0; j < 8; ++j) {
        board[1][j] = Piece(PAWN, BLACK);
        board[6][j] = Piece(PAWN, WHITE);
    }
    // Rooks
    board[0][0] = board[0][7] = Piece(ROOK, BLACK);
    board[7][0] = board[7][7] = Piece(ROOK, WHITE);
    // Knights
    board[0][1] = board[0][6] = Piece(KNIGHT, BLACK);
    board[7][1] = board[7][6] = Piece(KNIGHT, WHITE);
    // Bishops
    board[0][2] = board[0][5] = Piece(BISHOP, BLACK);
    board[7][2] = board[7][5] = Piece(BISHOP, WHITE);
    // Queens
    board[0][3] = Piece(QUEEN, BLACK);
    board[7][3] = Piece(QUEEN, WHITE);
    // Kings
    board[0][4] = Piece(KING, BLACK);
    board[7][4] = Piece(KING, WHITE);
}

void Board::print() const {
    cout << "   a  b  c  d  e  f  g  h  \n\n";
    for (int i = 0; i < 8; ++i) {
        cout << 8 - i << " ";
        for (int j = 0; j < 8; ++j) {
            cout << "|";
            string bg = ((i + j) % 2 == 0) ? B_WHITE : B_BLACK;
            string fg = (board[i][j].color == WHITE) ? FG_W : FG_B;
            cout << bg << fg << board[i][j].symbol() << " " << RESET;
        }
        cout << "|" << " " << 8 - i << "\n";
    }
    cout <<"\n   a  b  c  d  e  f  g  h  \n\n";
}


bool Board::inBoard(int x, int y) const {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

bool Board::move(int x1, int y1, int x2, int y2) {
    Piece& src = board[x1][y1];
    Piece& dst = board[x2][y2];
    if (src.color != turn) return false;
    auto moves = getLegalMovesForPiece(x1, y1);
    for (auto& [nx, ny] : moves) {
        if (nx == x2 && ny == y2) {
            dst = src;
            src = Piece();
            turn = (turn == WHITE) ? BLACK : WHITE;
            return true;
        }
    }
    return false;
}

vector<tuple<int, int, int, int>> Board::getLegalMoves(Color color) const {
    vector<tuple<int, int, int, int>> moves;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j].color == color) {
                auto pieceMoves = getLegalMovesForPiece(i, j);
                for (auto& [x2, y2] : pieceMoves) {
                    Board temp = *this;
                    temp.moveWithoutTurnChange(i, j, x2, y2); 
                    if (!temp.isKingInCheck(color)) {
                        moves.push_back({i, j, x2, y2});
                    }
                }
            }
        }
    }
    return moves;
}


vector<pair<int, int>> Board::getLegalMovesForPiece(int x, int y) const {
    vector<pair<int, int>> moves;
    Piece p = board[x][y];
    if (p.type == PAWN) {
        int dir = (p.color == WHITE) ? -1 : 1;
        int startRow = (p.color == WHITE) ? 6 : 1;

        int nx = x + dir;
        // Single move forward
        if (inBoard(nx, y) && board[nx][y].type == EMPTY)
            moves.emplace_back(nx, y);

        // Double move from start row
        if (x == startRow && board[nx][y].type == EMPTY) {
            int nx2 = x + 2 * dir;
            if (inBoard(nx2, y) && board[nx2][y].type == EMPTY)
                moves.emplace_back(nx2, y);
        }

        // Captures
        for (int dy : {-1, 1}) {
            int ny = y + dy;
            if (inBoard(nx, ny) && board[nx][ny].type != EMPTY && board[nx][ny].color != p.color)
                moves.emplace_back(nx, ny);
        }
    } else if (p.type == ROOK) {
        for (int dx : {-1, 1}) {
            for (int i = x + dx; inBoard(i, y); i += dx) {
                if (board[i][y].type == EMPTY) moves.emplace_back(i, y);
                else { if (board[i][y].color != p.color) moves.emplace_back(i, y); break; }
            }
        }
        for (int dy : {-1, 1}) {
            for (int j = y + dy; inBoard(x, j); j += dy) {
                if (board[x][j].type == EMPTY) moves.emplace_back(x, j);
                else { if (board[x][j].color != p.color) moves.emplace_back(x, j); break; }
            }
        }
    } else if (p.type == BISHOP) {
        for (int dx : {-1, 1}) {
            for (int dy : {-1, 1}) {
                int nx = x + dx, ny = y + dy;
                while (inBoard(nx, ny)) {
                    if (board[nx][ny].type == EMPTY) moves.emplace_back(nx, ny);
                    else {
                        if (board[nx][ny].color != p.color) moves.emplace_back(nx, ny);
                        break;
                    }
                    nx += dx; ny += dy;
                }
            }
        }
    } else if (p.type == QUEEN) {
        // Rook moves
        for (int dx : {-1, 1}) {
            for (int i = x + dx; inBoard(i, y); i += dx) {
                if (board[i][y].type == EMPTY) moves.emplace_back(i, y);
                else { if (board[i][y].color != p.color) moves.emplace_back(i, y); break; }
            }
        }
        for (int dy : {-1, 1}) {
            for (int j = y + dy; inBoard(x, j); j += dy) {
                if (board[x][j].type == EMPTY) moves.emplace_back(x, j);
                else { if (board[x][j].color != p.color) moves.emplace_back(x, j); break; }
            }
        }
        // Bishop moves
        for (int dx : {-1, 1}) {
            for (int dy : {-1, 1}) {
                int nx = x + dx, ny = y + dy;
                while (inBoard(nx, ny)) {
                    if (board[nx][ny].type == EMPTY) moves.emplace_back(nx, ny);
                    else {
                        if (board[nx][ny].color != p.color) moves.emplace_back(nx, ny);
                        break;
                    }
                    nx += dx; ny += dy;
                }
            }
        }
    } else if (p.type == KNIGHT) {
        for (int dx : {-2, -1, 1, 2}) {
            for (int dy : {-2, -1, 1, 2}) {
                if (abs(dx) != abs(dy)) {
                    int nx = x + dx, ny = y + dy;
                    if (inBoard(nx, ny) && (board[nx][ny].type == EMPTY || board[nx][ny].color != p.color))
                        moves.emplace_back(nx, ny);
                }
            }
        }
    } else if (p.type == KING) {
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx != 0 || dy != 0) {
                    int nx = x + dx, ny = y + dy;
                    if (inBoard(nx, ny) && (board[nx][ny].type == EMPTY || board[nx][ny].color != p.color))
                        moves.emplace_back(nx, ny);
                }
            }
        }
    }
    return moves;
}

int Board::evaluate() const {
    int score = 0;
    int pieceValue[] = {10000, 900, 500, 330, 320, 100, 0}; // K,Q,R,B,N,P,EMPTY
    int mobilityWhite = 0, mobilityBlack = 0;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j].color == WHITE)
                score += pieceValue[board[i][j].type];
            else if (board[i][j].color == BLACK)
                score -= pieceValue[board[i][j].type];
        }
    }

    // Mobility
    mobilityWhite = getLegalMoves(WHITE).size();
    mobilityBlack = getLegalMoves(BLACK).size();
    score += 5 * (mobilityWhite - mobilityBlack); // Adjust weight as desired

    // King safety (very basic: penalize being on edge/rank 1/8 or file a/h)
    int wx = -1, wy = -1, bx = -1, by = -1;
    for (int i = 0; i < 8; ++i) for (int j = 0; j < 8; ++j) {
        if (board[i][j].type == KING && board[i][j].color == WHITE) { wx = i; wy = j; }
        if (board[i][j].type == KING && board[i][j].color == BLACK) { bx = i; by = j; }
    }
    if (wx == 0 || wx == 7) score -= 20; // white king on edge
    if (wy == 0 || wy == 7) score -= 20;
    if (bx == 0 || bx == 7) score += 20; // black king on edge
    if (by == 0 || by == 7) score += 20;

    // Center control (bonus for pawns/pieces on d4,e4,d5,e5)
    for (auto sq : {make_pair(3, 3), make_pair(3, 4), make_pair(4, 3), make_pair(4, 4)}) {
        int i = sq.first, j = sq.second;
        if (board[i][j].color == WHITE) score += 10;
        if (board[i][j].color == BLACK) score -= 10;
    }

    // Optionally: Piece-square tables for knights/bishops/rooks/queen

    return score;
}
bool Board::isKingInCheck(Color color) const {
    // Find king position
    int kx = -1, ky = -1;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (board[i][j].type == KING && board[i][j].color == color)
                { kx = i; ky = j; }
    if (kx == -1) return true; // king missing
    // Check if king is attacked by any opponent piece
    Color opp = (color == WHITE) ? BLACK : WHITE;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j].color == opp) {
                auto moves = getLegalMovesForPiece(i, j);
                for (auto& [x, y] : moves)
                    if (x == kx && y == ky)
                        return true;
            }
        }
    }
    return false;
}

void Board::moveWithoutTurnChange(int x1, int y1, int x2, int y2) {
    Piece& src = board[x1][y1];
    Piece& dst = board[x2][y2];
    dst = src;
    src = Piece();
    // Do NOT update 'turn' here
}

bool Board::isCheckmate(Color color) const {
    return getLegalMoves(color).empty() && isKingInCheck(color);
}

bool Board::isStalemate(Color color) const {
    return getLegalMoves(color).empty() && !isKingInCheck(color);
}

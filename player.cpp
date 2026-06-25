#include "player.hpp"
#include <iostream>
#include <limits>
#include <unordered_map>
using namespace std;

// Helper: char to PieceType
PieceType charToPieceType(char c) {
    if (c == 'N') return KNIGHT;
    if (c == 'B') return BISHOP;
    if (c == 'R') return ROOK;
    if (c == 'Q') return QUEEN;
    if (c == 'K') return KING;
    return PAWN;
}

// Helper: promotion char to PieceType
PieceType promotionCharToType(char c) {
    if (c == 'N') return KNIGHT;
    if (c == 'B') return BISHOP;
    if (c == 'R') return ROOK;
    if (c == 'Q') return QUEEN;
    return QUEEN; // default to queen
}

HumanPlayer::HumanPlayer(Color c) : Player(c) {}

bool HumanPlayer::makeMove(Board& board, std::string& moveNotation) {
    while (true) {
        cout << "Enter move (e.g. e4, Nf3, exd5, O-O, e8Q): ";
        string moveStr;
        cin >> moveStr;

        // --- Castling (O-O or O-O-O or 0-0 or 0-0-0) ---
        if (moveStr == "O-O" || moveStr == "0-0") {
            // King-side castling
            int row = (color == WHITE) ? 7 : 0;
            if (
                board.board[row][4].type == KING && board.board[row][4].color == color &&
                board.board[row][7].type == ROOK && board.board[row][7].color == color &&
                board.board[row][5].type == EMPTY && board.board[row][6].type == EMPTY
                // You can add check conditions here
            ) {
                board.move(row, 4, row, 6); // King
                board.move(row, 7, row, 5); // Rook
                cout << "Castling king-side!\n";
                break;
            } else {
                cout << "Castling not allowed.\n";
                continue;
            }
        }
        if (moveStr == "O-O-O" || moveStr == "0-0-0") {
            // Queen-side castling
            int row = (color == WHITE) ? 7 : 0;
            if (
                board.board[row][4].type == KING && board.board[row][4].color == color &&
                board.board[row][0].type == ROOK && board.board[row][0].color == color &&
                board.board[row][1].type == EMPTY && board.board[row][2].type == EMPTY && board.board[row][3].type == EMPTY
                // You can add check conditions here
            ) {
                board.move(row, 4, row, 2); // King
                board.move(row, 0, row, 3); // Rook
                cout << "Castling queen-side!\n";
                break;
            } else {
                cout << "Castling not allowed.\n";
                continue;
            }
        }

        // --- Algebraic Notation Parsing ---
        PieceType pieceType = PAWN;
        int srcFile = -1, srcRank = -1;
        bool isCapture = false;
        PieceType promotionType = EMPTY;

        int len = moveStr.length();
        int i = 0;

        // 1. Piece type (first letter)
        if (isupper(moveStr[0]) && moveStr[0] != 'O') {
            pieceType = charToPieceType(moveStr[0]);
            ++i;
        }

        // 2. Optional disambiguation (file/rank)
        if (i < len-2 && isalpha(moveStr[i]) && islower(moveStr[i])) {
            srcFile = moveStr[i] - 'a';
            ++i;
        } else if (i < len-2 && isdigit(moveStr[i])) {
            srcRank = 8 - (moveStr[i] - '0');
            ++i;
        }

        // 3. Capture
        if (i < len && moveStr[i] == 'x') {
            isCapture = true;
            ++i;
        }

        // 4. Destination square (last two are always square)
        if (i > len-2) { cout << "Invalid format!\n"; continue; }
        int destFile = moveStr[len-2] - 'a';
        int destRank = 8 - (moveStr[len-1] - '0');

        // 5. Promotion (if there's still a char left)
        if (len > 2 && isupper(moveStr[len-1]) && isalpha(moveStr[len-1]) && len >= 4) {
            // e.g. e8Q, exd8N
            promotionType = promotionCharToType(moveStr[len-1]);
            destFile = moveStr[len-3] - 'a';
            destRank = 8 - (moveStr[len-2] - '0');
            // adjust parsing to allow for "exd8N" as well
            if (pieceType == PAWN && moveStr.find('x') != string::npos) {
                isCapture = true;
            }
        }

        // --- Find all matching legal moves ---
        auto moves = board.getLegalMoves(color);
        vector<tuple<int,int,int,int>> candidates;
        for (auto& m : moves) {
            int x1, y1, x2, y2;
            tie(x1, y1, x2, y2) = m;
            Piece p = board.board[x1][y1];

            // 1. Piece type match
            if (p.type != pieceType) continue;
            // 2. To square
            if (x2 != destRank || y2 != destFile) continue;
            // 3. Disambiguation
            if (srcFile != -1 && y1 != srcFile) continue;
            if (srcRank != -1 && x1 != srcRank) continue;
            // 4. Capture/non-capture
            if (isCapture && (board.board[x2][y2].type == EMPTY || board.board[x2][y2].color == color))
                continue;
            if (!isCapture && board.board[x2][y2].type != EMPTY)
                continue;
            // 5. Promotion (for pawn moves to last rank)
            if (promotionType != EMPTY && p.type == PAWN && (x2 == 0 || x2 == 7)) {
                // handled after move
            }
            candidates.push_back(m);
        }

        if (candidates.empty()) {
            cout << "Illegal move!\n";
            continue;
        }
        if (candidates.size() > 1) {
            cout << "Ambiguous move! Try specifying file/rank.\n";
            continue;
        }

        // Play the move
        int x1, y1, x2, y2;
        tie(x1, y1, x2, y2) = candidates[0];
        board.move(x1, y1, x2, y2);

        // Pawn Promotion: Replace pawn with promoted piece if applicable
        if (promotionType != EMPTY) {
            if (board.board[x2][y2].type == PAWN && (x2 == 0 || x2 == 7)) {
                board.board[x2][y2] = Piece(promotionType, color);
            }
        } else if (pieceType == PAWN && (x2 == 0 || x2 == 7)) {
            // If no explicit promotion, promote to Queen by default
            board.board[x2][y2] = Piece(QUEEN, color);
        }
        moveNotation=moveStr;
        break;
    }
    return true;
}

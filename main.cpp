#include "board.hpp"
#include "player.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
using namespace std;
using namespace std::chrono;

int main() {
    int timeWhite, timeBlack, incWhite, incBlack;

    // Ask user for times and increments
    cout << "Enter timer for White (seconds): ";
    cin >> timeWhite;
    cout << "Enter increment for White (seconds): ";
    cin >> incWhite;

    cout << "Enter timer for Black (seconds): ";
    cin >> timeBlack;
    cout << "Enter increment for Black (seconds): ";
    cin >> incBlack;

    vector<string> pgnMoves;
    Board board;
    HumanPlayer white(WHITE);
    HumanPlayer black(BLACK);

    while (true) {
        board.print();

        cout << "White time: " << timeWhite << "s | Black time: " << timeBlack << "s\n";

        if (board.isCheckmate(board.turn)) {
            cout << ((board.turn == WHITE) ? "Black" : "White") << " wins! Checkmate.\n";
            break;
        }
        if (board.isStalemate(board.turn)) {
            cout << "Game drawn by stalemate.\n";
            break;
        }
        if (timeWhite <= 0) {
            cout << "White loses on time!\n";
            break;
        }
        if (timeBlack <= 0) {
            cout << "Black loses on time!\n";
            break;
        }

        string moveStr;
        auto start = steady_clock::now();

        if (board.turn == WHITE)
            white.makeMove(board, moveStr);
        else
            black.makeMove(board, moveStr);

        auto end = steady_clock::now();
        int elapsed = duration_cast<seconds>(end - start).count();

        // Decrement, add increment as per move played
        if (board.turn == BLACK) {
            timeWhite -= elapsed;
            timeWhite += incWhite;
        } else {
            timeBlack -= elapsed;
            timeBlack += incBlack;
        }

        pgnMoves.push_back(moveStr);

        if (board.isKingInCheck(board.turn)) {
            cout << "Check!" << endl;
        }
    }

    // Output PGN at the end
    cout << "\nPGN Moves:\n";
    for (size_t i = 0; i < pgnMoves.size(); ++i) {
        if (i % 2 == 0) cout << (i / 2 + 1) << ". ";
        cout << pgnMoves[i] << " ";
    }
    cout << endl;

    return 0;
}

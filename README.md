# ♟️ ChessMate: Terminal-Based 2-Player Chess Engine

[![C++](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/platform-Terminal-lightgrey.svg)]()
[![Game Mode](https://img.shields.io/badge/mode-2_Player-green.svg)]()
[![License](https://img.shields.io/badge/license-MIT-lightgrey.svg)](LICENSE)
[![Repo](https://img.shields.io/badge/github-view_repo-black?logo=github)](https://github.com/jainDivyansh0211/chess-2Player-cpp)

ChessMate is a C++17 terminal-based chess game for two human players, complete with piece movement, game rules, castling, promotion, check/checkmate/stalemate detection, and even a PGN-style move log — all rendered beautifully in ASCII.

---

## 🎯 Features

- ♞ **Complete Chess Rules** – Pawns, Castling, En Passant (soon), Promotion, Checkmate/Stalemate detection.
- 🧠 **Move Validation Engine** – Prevents illegal moves, captures, or moving into check.
- 🎨 **ASCII Art Board Renderer** – Colored terminal UI with piece symbols.
- 📝 **PGN-Like Move Logger** – Keeps track of moves like `1. e4 e5 2. Nf3 Nc6 ...`.
- ⏱️ **Custom Timers + Increments** – Per-player configurable clock with support for Fischer-style increments.
- 🏁 **Game End Detection** – Supports checkmate, stalemate, and timeout losses.

---

## 📷 Preview

> Example gameplay screenshots:

![Preview 1](chess_preview_1.png)
![Preview 2](chess_preview_2.png)
![Preview 3](chess_preview_3.png)
![Preview 4](chess_preview_4.png)

---

## 🚀 Getting Started

### 🧰 Requirements

- A C++17 compatible compiler (`g++` or `clang++`)
- Terminal (Linux/macOS or WSL on Windows)

### 🛠️ Build & Run

```bash
# Clone the repo
git clone https://github.com/jainDivyansh0211/chess-2Player-cpp
cd chess-2Player-cpp

# Compile
g++ -std=c++17 main.cpp board.cpp piece.cpp player.cpp -o chess

# Run
./chess
```

---

## 🕹️ How to Play

1. On launching, enter time (in seconds) and increment for both players.
2. Move using standard **algebraic notation**:
   - `e4`, `Nf3`, `exd5`, `O-O`, `O-O-O`, `e8Q` (promotion)
3. The game automatically alternates turns, tracks time, and prints status.
4. At the end, PGN-style move history is shown.

---

## 🧠 Code Structure

| File           | Description                                |
|----------------|--------------------------------------------|
| `main.cpp`     | Main game loop with timers and PGN         |
| `board.hpp/.cpp` | Board logic, movement rules, evaluation |
| `piece.hpp/.cpp` | Piece definition & symbols               |
| `player.hpp/.cpp` | Player input parsing, castling, promotion |
| `chess.exe`    | Compiled binary for direct execution       |

---

## 🧪 Sample Move Flow

```text
Enter move (e.g. e4, Nf3, exd5, O-O, e8Q): e4
Check!
Enter move (e.g. e4, Nf3, exd5, O-O, e8Q): e5
Enter move (e.g. e4, Nf3, exd5, O-O, e8Q): Nf3
...
```

PGN Output:
```
1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 ...
```

## 👨‍💻 Author

**Divyansh Jain**  
🎓 B.Tech, IIT Kharagpur '27  
📧 [div0211jain@gmail.com](mailto:div0211jain@gmail.com)  
🔗 [GitHub Profile](https://github.com/jainDivyansh0211)

---

## 📄 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.


---

# Mummy Maze Game - Beta 1.0-A

## Overview
Mummy Maze is a classic maze escape game where you (the player) must navigate through a randomly generated maze to reach the exit while avoiding a mummy that hunts you down. Each turn, you can move up, down, left, or right, while the mummy moves twice per turn, making it a fast-paced chase.

## Features
- **Procedurally Generated Mazes**: Each game starts with a unique, guaranteed-solvable maze created using BFS-based connectivity validation.
- **Dynamic Difficulty**: The mummy's movement AI is simple but aggressive, moving toward the player's position when possible.
- **Player Freedom**: Choose to move or skip your turn (`j` key) to strategize.
- **Cross-Platform Support**: Works on Windows, macOS, and Linux.
- **Stable Map Generation**: Includes a safety mechanism to prevent infinite loops during map generation.

## How to Play
1. Run the compiled executable.
2. A maze will appear with the following symbols:
   - `p`: Player (you)
   - `m`: Mummy (enemy)
   - `e`: Exit (your goal)
   - `#`: Wall (impassable)
   - `.`: Empty space (walkable)
3. Enter a movement command each turn:
   - `w` / `W`: Move Up
   - `s` / `S`: Move Down
   - `a` / `A`: Move Left
   - `d` / `D`: Move Right
   - `j` / `J`: Skip (stay in place)
4. Reach the exit (`e`) to win, but avoid the mummy (`m`) or it's game over!

## Game Rules
- The mummy moves twice for every one player move.
- If the mummy catches you, you lose.
- If you reach the exit, you win.
- Walls block both the player and the mummy.

## Controls
| Key | Action |
|-----|--------|
| `W` / `w` | Move Up |
| `S` / `s` | Move Down |
| `A` / `a` | Move Left |
| `D` / `d` | Move Right |
| `J` / `j` | Skip Turn |

## Installation & Compilation

### Requirements
- C++17 or later
- A C++ compiler (GCC, Clang, or MSVC)

### Compile
```bash
# On Linux/macOS
g++ -std=c++17 -o mummy_maze mummy_maze.cpp

# On Windows (using MinGW or MSVC)
g++ -std=c++17 -o mummy_maze.exe mummy_maze.cpp
```

### Run
```bash
# On Linux/macOS
./mummy_maze

# On Windows
mummy_maze.exe
```

## Version History

### Beta 1.0-A (Current)
- Fixed potential infinite loop in map generation by adding a maximum attempt counter.
- On generation failure, the program now exits gracefully with an error message.
- Maintained all existing gameplay mechanics.

### Previous (Beta 0.x)
- Initial maze generation with BFS connectivity.
- Basic mummy chase AI.
- Cross-platform screen clearing.

## Known Issues
- The mummy's AI is simplistic and may not always find the shortest path around walls.
- Input handling is not fully robust; invalid commands still consume a player turn.
- Random number generation uses `rand()` and may produce similar patterns within the same second.

## Future Improvements
- Implement BFS-based pathfinding for the mummy to make it more challenging.
- Add input validation to prevent accidental turn loss.
- Introduce difficulty levels (adjust wall density, mummy speed, etc.).
- Add a scoring system based on time taken or turns used.
- Implement save/load functionality for in-progress games.

## Troubleshooting

### "Failed to generate map" Error
This occurs if the random maze generator cannot create a valid layout within 10,000 attempts. This is extremely rare under normal conditions. Simply run the program again to generate a new maze.

### Game Does Not Display Correctly
Ensure your terminal window is at least 20 columns wide and 10 rows tall. The maze is displayed using simple text characters.

### Compilation Errors
Make sure you are using C++17 or later. If your compiler is older, update it or use `-std=c++17` flag.

## Contact
For bugs, feature requests, or contributions, please create an issue on the project repository or contact the development team.

---

**Enjoy the chase! 🏃‍♂️🧟**
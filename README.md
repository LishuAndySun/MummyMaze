# Mummy Maze Escape

**Version: Beta 1.0-C**

A classic console-based maze escape game written in C++. You play as a
player (`p`) trapped in a randomly generated maze, trying to reach the
exit (`e`) while being hunted by a relentless mummy (`m`) that moves
twice for every move you make.

---

## 🎮 Gameplay

- A **random maze** is generated every run (validated with BFS to
  guarantee it is playable and connected enough).
- You start at a random position; the **exit** is always placed far away
  (minimum distance of 6 steps) and the **mummy** spawns at least 5 steps
  from you — no instant-kill starts.
- The mummy **moves twice per turn** after every player move.
- Reach the exit before the mummy catches you!

### Special Rule (new in Beta 1.0-C)

> **If the mummy ever steps onto the exit cell, the mummy is instantly
> defeated and YOU win.**
> Use this to your advantage: bait the mummy into chasing you along a
> path that crosses the exit!

### Win / Lose Conditions

| Condition | Result |
|---|---|
| You reach the exit (`e`) | 🏆 You escaped — you win |
| The mummy steps on the exit (`e`) | 🏆 Mummy trapped — you win |
| The mummy reaches you (`p`) | 💀 Caught — game over |

---

## ⌨️ Controls

| Key | Action |
|---|---|
| `w` | Move up |
| `s` | Move down |
| `a` | Move left |
| `d` | Move right |
| `j` | Skip this turn (mummy still moves!) |

Input is case-insensitive (`W/A/S/D/J` also work).

---

## 🧟 The Mummy AI

Since **Beta 1.0-C**, the mummy uses **BFS (Breadth-First Search)
pathfinding**. Each move, it computes the true shortest path to the
player and takes the first step along that path. It can now route around
walls instead of getting stuck — no wall will save you.

---

## 🔨 Building

**Requirements:** A C++17 compatible compiler (structured bindings are
used internally).

### Linux / macOS

```bash
g++ -std=c++17 -O2 -o mummy_maze main.cpp
./mummy_maze
```

### Windows (MinGW)

```bash
g++ -std=c++17 -O2 -o mummy_maze.exe main.cpp
mummy_maze.exe
```

### Visual Studio

Create a console project, add `main.cpp`, and set
**C++ Language Standard → ISO C++17 (/std:c++17)** in project properties.

No external dependencies are required — only the C++ standard library.

---

## 🗺️ Map Legend

| Symbol | Meaning |
|---|---|
| `p` | Player (you) |
| `m` | Mummy (hunter) |
| `e` | Exit / treasure |
| `#` | Wall (impassable) |
| `.` | Open floor |

---

## ⚙️ Configuration

Tweak these constants at the top of `main.cpp`:

```cpp
const int MAZER = 10, MAZEC = 20;      // Maze dimensions (rows x columns)
const double WALL_DENSITY = 0.13;      // Probability of a wall per interior cell
```

- Increase `WALL_DENSITY` for a harder maze (more walls, tighter corridors).
- Enlarge `MAZER` / `MAZEC` for a bigger arena (update the `maze`
  buffer size accordingly).

---

## 📋 Changelog

### Beta 1.0-C

- **Fixed (Issue [#13](https://github.com/LishuAndySun/MummyMaze/issues/13)):** Mummy AI rewritten with BFS shortest-path
  pathfinding. The mummy no longer gets permanently stuck behind walls
  and can pursue the player around obstacles.
- **Fixed (Issue [#12](https://github.com/LishuAndySun/MummyMaze/issues/12)):** New game rule — if the mummy steps onto the exit
  cell, the game ends immediately with a **player victory**. This removes
  the ambiguity where the mummy could occupy the exit and the win/lose
  conditions could conflict.
# Mummy Maze Escape

**Version: Beta 1.0-F**

A classic console-based maze escape game written in C++. You play as a
player (`P`) trapped in a randomly generated maze, trying to reach the
exit (`E`) while being hunted by a relentless mummy (`M`) that moves
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
| You reach the exit (`E`) | 🏆 You escaped — you win |
| The mummy steps on the exit (`E`) | 🏆 Mummy trapped — you win |
| The mummy reaches you (`P`) | 💀 Caught — game over |

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

### Invalid Input Handling (new in Beta 1.0-F)

> Typing an unrecognized key **no longer wastes your turn**. The game
> will prompt you to re-enter a valid command, and the mummy will not
> move until you do. A mistyped key can no longer gift the mummy two
> free moves.

---

## 🧟 The Mummy AI

The mummy uses **BFS (Breadth-First Search) pathfinding**. Each move, it
computes the true shortest path to the player and takes the first step
along that path. It can route around walls instead of getting stuck —
no wall will save you.

---

## 🗺️ Map Generation

The maze is generated randomly and validated with BFS to ensure it is
fully playable:

- Enough open, connected space (at least 45% of interior cells reachable).
- Player-to-exit shortest distance of at least 6 steps.
- Mummy spawns at least 5 steps away from the player.

Since **Beta 1.0-F**, generation attempts are capped: the wall density is
gradually relaxed after repeated failures, and if all attempts fail, a
guaranteed-solvable fallback maze is used — the game can never hang in
an infinite generation loop.

---

## 🔨 Building

**Requirements:** A C++14 compatible compiler.

### Linux / macOS

```bash
g++ -std=c++14 -O2 -o mummy_maze main.cpp
./mummy_maze
Windows (MinGW)

Visual Studio

Create a console project, add main.cpp, and set C++ Language Standard → ISO C++14 (/std:c++14) in project properties.

No external dependencies are required — only the C++ standard library.

🗺️ Map Legend

Symbol,Meaning
P,Player (you)
M,Mummy (hunter)
E,Exit / treasure
X,Mummy standing on the exit (mummy trapped)
#,Wall (impassable)
.,Open floor

⚙️ Configuration

Tweak these constants at the top of main.cpp:

  • Increase WallDensity for a harder maze (more walls, tighter corridors).
  • Enlarge MazeRows / MazeCols for a bigger arena (update the Maze buffer size accordingly).

📋 Changelog

Beta 1.0-F

  • Fixed (Issue [#16](https://github.com/LishuAndySun/MummyMaze/issues/16)): Invalid inputs are no longer treated as a "skip" turn. Unrecognized keys now trigger a re-prompt instead of silently letting the mummy take two free moves.
  • Fixed (Issue [#17](https://github.com/LishuAndySun/MummyMaze/issues/17)): Map generation now has a maximum retry limit. If random generation cannot produce a valid maze within the limit, the wall density is progressively relaxed and a deterministic, guaranteed- solvable fallback maze is generated — eliminating a potential infinite loop / hang.
  • Build standard requirement lowered to C++14 (structured bindings removed; all pair access now uses direct member syntax).
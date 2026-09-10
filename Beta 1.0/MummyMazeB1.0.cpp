#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <limits>

#ifdef _WIN32
#define NOMINMAX // Prevent windows.h from defining min/max macros
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

//*-------------------------------------------------------*//
// Maze map buffer
char Maze[30][50];

int PlayerRow, PlayerCol;       // Player current position
int MummyRow, MummyCol;         // Mummy current position
int ExitRow, ExitCol;           // Exit/treasure position
const int MazeRows = 10;        // Maze row count
const int MazeCols = 20;        // Maze column count
const double WallDensity = 0.13; // Density of internal walls
//*---------------------CONFIG AREA ENDING-----------------*//

bool GameOver();
void ShowMap();
void MovePlayer(char Direction); // W = Up, S = Down, A = Left, D = Right
void SkipPlayer();               // Skip player's move for this turn
void MoveMummy();
void ClearScreen();
void GenerateMap();

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Generate random valid maze using BFS
    GenerateMap();

    char InputDirection;

    while (!GameOver())
    {
        ClearScreen();
        ShowMap();
        cout << "Enter [w|a|s|d] to move or [j] to skip: ";
        cin >> InputDirection;
        
        // Flush remaining newline characters in buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (InputDirection == 'j' || InputDirection == 'J')
        {
            SkipPlayer();
        }
        else
        {
            MovePlayer(InputDirection);
        }

        // The mummy moves twice per turn
        for (int StepIndex = 0; StepIndex < 2; ++StepIndex)
        {
            if (GameOver()) break; // Stop moving if game has already ended
            MoveMummy();
            if (GameOver()) break; // Immediate check after moving
        }
    }
    
    ClearScreen();
    ShowMap();

    // Determine win or lose condition
    if (MummyRow == ExitRow && MummyCol == ExitCol)
    {
        cout << "The mummy stepped on the exit and was trapped! You win!" << endl;
    }
    else if (PlayerRow == ExitRow && PlayerCol == ExitCol)
    {
        cout << "Congratulations! You escaped!" << endl;
    } 
    else if (PlayerRow == MummyRow && PlayerCol == MummyCol)
    {
        cout << "You were caught by the mummy! Game Over!" << endl;
    }

#ifdef _WIN32
    Sleep(5000);
#else
    sleep(5);
#endif
    return 0;
}

// Function to skip the player's movement for this turn
void SkipPlayer()
{
    // Player remains at current position; mummy moves after
}

// Generates a solvable and playable map randomly using BFS
void GenerateMap()
{
    const int DeltaRow[] = {-1, 1, 0, 0};
    const int DeltaCol[] = {0, 0, -1, 1};

    while (true)
    {
        // 1. Initialize boundary and random interior walls
        for (int Row = 0; Row < MazeRows; ++Row)
        {
            for (int Col = 0; Col < MazeCols; ++Col)
            {
                if (Row == 0 || Row == MazeRows - 1 || Col == 0 || Col == MazeCols - 1)
                {
                    Maze[Row][Col] = '#';
                }
                else
                {
                    double RandomRatio = static_cast<double>(rand()) / RAND_MAX;
                    Maze[Row][Col] = (RandomRatio < WallDensity) ? '#' : '.';
                }
            }
        }

        // 2. Randomly pick a free space for Player
        vector<pair<int, int>> FreeCells;
        for (int Row = 1; Row < MazeRows - 1; ++Row)
        {
            for (int Col = 1; Col < MazeCols - 1; ++Col)
            {
                if (Maze[Row][Col] == '.')
                {
                    FreeCells.push_back({Row, Col});
                }
            }
        }

        if (FreeCells.empty()) continue;

        int PlayerIndex = rand() % FreeCells.size();
        PlayerRow = FreeCells[PlayerIndex].first;
        PlayerCol = FreeCells[PlayerIndex].second;

        // 3. BFS from player position to find reachable cells and shortest distances
        vector<vector<int>> DistanceMap(MazeRows, vector<int>(MazeCols, -1));
        queue<pair<int, int>> SearchQueue;

        DistanceMap[PlayerRow][PlayerCol] = 0;
        SearchQueue.push({PlayerRow, PlayerCol});

        vector<pair<int, pair<int, int>>> ReachableCells; // Stores {Distance, {Row, Col}}

        while (!SearchQueue.empty())
        {
            // C++14 standard: access pair members directly
            pair<int, int> CurrentCell = SearchQueue.front();
            SearchQueue.pop();
            int CurrentRow = CurrentCell.first;
            int CurrentCol = CurrentCell.second;

            ReachableCells.push_back({DistanceMap[CurrentRow][CurrentCol], {CurrentRow, CurrentCol}});

            for (int DirectionIndex = 0; DirectionIndex < 4; ++DirectionIndex)
            {
                int NextRow = CurrentRow + DeltaRow[DirectionIndex];
                int NextCol = CurrentCol + DeltaCol[DirectionIndex];

                if (NextRow >= 0 && NextRow < MazeRows && NextCol >= 0 && NextCol < MazeCols)
                {
                    if (Maze[NextRow][NextCol] == '.' && DistanceMap[NextRow][NextCol] == -1)
                    {
                        DistanceMap[NextRow][NextCol] = DistanceMap[CurrentRow][CurrentCol] + 1;
                        SearchQueue.push({NextRow, NextCol});
                    }
                }
            }
        }

        // 4. Validate connectivity (ensure map is not too narrow or blocked)
        int TotalInnerCells = (MazeRows - 2) * (MazeCols - 2);
        if (static_cast<int>(ReachableCells.size()) < TotalInnerCells * 0.45)
        {
            continue; // Not enough connected space, regenerate
        }

        // 5. Select Exit: Pick among the farthest reachable cells
        sort(ReachableCells.begin(), ReachableCells.end());
        int MaxDistance = ReachableCells.back().first;
        if (MaxDistance < 6) continue; // Ensure player and exit are not too close

        vector<pair<int, int>> ExitCandidates;
        for (const auto& ReachableItem : ReachableCells)
        {
            if (ReachableItem.first >= MaxDistance - 2) // Within top distance bracket
            {
                ExitCandidates.push_back(ReachableItem.second);
            }
        }

        int ExitIndex = rand() % ExitCandidates.size();
        ExitRow = ExitCandidates[ExitIndex].first;
        ExitCol = ExitCandidates[ExitIndex].second;

        // 6. Select Mummy: Must be reachable, Distance >= 5, and != Exit
        vector<pair<int, int>> MummyCandidates;
        for (const auto& ReachableItem : ReachableCells)
        {
            int CellDistance = ReachableItem.first;
            int CellRow = ReachableItem.second.first;
            int CellCol = ReachableItem.second.second;

            if (CellDistance >= 5 && !(CellRow == ExitRow && CellCol == ExitCol))
            {
                MummyCandidates.push_back({CellRow, CellCol});
            }
        }

        if (MummyCandidates.empty()) continue;

        int MummyIndex = rand() % MummyCandidates.size();
        MummyRow = MummyCandidates[MummyIndex].first;
        MummyCol = MummyCandidates[MummyIndex].second;

        break; // Successfully generated a valid layout
    }
}

// Clear console screen across different platforms
void ClearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Dynamically render the map based on current coordinates
void ShowMap()
{
    for (int Row = 0; Row < MazeRows; ++Row)
    {
        for (int Col = 0; Col < MazeCols; ++Col)
        {
            // 1. Player overlaps with Exit or is at regular position -> 'P'
            if (Row == PlayerRow && Col == PlayerCol)
            {
                cout << 'P';
            }
            // 2. Mummy overlaps with Exit -> 'X'
            else if (Row == MummyRow && Col == MummyCol && Row == ExitRow && Col == ExitCol)
            {
                cout << 'X';
            }
            // 3. Exit position -> 'E'
            else if (Row == ExitRow && Col == ExitCol)
            {
                cout << 'E';
            }
            // 4. Mummy position -> 'M'
            else if (Row == MummyRow && Col == MummyCol)
            {
                cout << 'M';
            }
            // 5. Default map tile
            else
            {
                cout << Maze[Row][Col];
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Check game termination conditions
bool GameOver()
{
    if ((PlayerRow == MummyRow && PlayerCol == MummyCol) || 
        (PlayerRow == ExitRow && PlayerCol == ExitCol) || 
        (MummyRow == ExitRow && MummyCol == ExitCol))
    {
        return true;
    }
    return false;
}

void MovePlayer(char Direction)
{
    int TargetRow = PlayerRow;
    int TargetCol = PlayerCol;

    if (Direction == 'w' || Direction == 'W') TargetRow--;
    if (Direction == 's' || Direction == 'S') TargetRow++;
    if (Direction == 'a' || Direction == 'A') TargetCol--;
    if (Direction == 'd' || Direction == 'D') TargetCol++;
    
    // Check if the target position is within bounds and not a wall
    if (TargetRow >= 0 && TargetRow < MazeRows && 
        TargetCol >= 0 && TargetCol < MazeCols && 
        Maze[TargetRow][TargetCol] != '#')
    {
        PlayerRow = TargetRow;
        PlayerCol = TargetCol;
    }
}

// BFS Pathfinding for Mummy AI
void MoveMummy()
{
    // Already at player position
    if (MummyRow == PlayerRow && MummyCol == PlayerCol) return;

    const int DeltaRow[] = {-1, 1, 0, 0};
    const int DeltaCol[] = {0, 0, -1, 1};

    // BFS from current mummy position to find shortest path to player
    queue<pair<int, int>> PathQueue;
    vector<vector<int>> DistanceMap(MazeRows, vector<int>(MazeCols, -1));
    vector<vector<pair<int, int>>> ParentMap(MazeRows, vector<pair<int, int>>(MazeCols, {-1, -1}));

    PathQueue.push({MummyRow, MummyCol});
    DistanceMap[MummyRow][MummyCol] = 0;

    bool PathFound = false;
    while (!PathQueue.empty())
    {
        // C++14 standard: access pair members directly
        pair<int, int> CurrentCell = PathQueue.front();
        PathQueue.pop();
        int CurrentRow = CurrentCell.first;
        int CurrentCol = CurrentCell.second;

        if (CurrentRow == PlayerRow && CurrentCol == PlayerCol)
        {
            PathFound = true;
            break;
        }

        for (int DirectionIndex = 0; DirectionIndex < 4; ++DirectionIndex)
        {
            int NextRow = CurrentRow + DeltaRow[DirectionIndex];
            int NextCol = CurrentCol + DeltaCol[DirectionIndex];

            if (NextRow >= 0 && NextRow < MazeRows && NextCol >= 0 && NextCol < MazeCols &&
                Maze[NextRow][NextCol] != '#' && DistanceMap[NextRow][NextCol] == -1)
            {
                DistanceMap[NextRow][NextCol] = DistanceMap[CurrentRow][CurrentCol] + 1;
                ParentMap[NextRow][NextCol] = {CurrentRow, CurrentCol};
                PathQueue.push({NextRow, NextCol});
            }
        }
    }

    if (!PathFound) return; // No accessible path to player

    // Trace back from Player to Mummy to determine the immediate next step
    pair<int, int> PathCursor = {PlayerRow, PlayerCol};
    while (ParentMap[PathCursor.first][PathCursor.second] != make_pair(MummyRow, MummyCol))
    {
        PathCursor = ParentMap[PathCursor.first][PathCursor.second];
    }

    // Move to the next step along the shortest path
    MummyRow = PathCursor.first;
    MummyCol = PathCursor.second;
}
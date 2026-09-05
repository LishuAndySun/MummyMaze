#include <bits/stdc++.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
using namespace std;

//*-------------------------------------------------------*//
// The maze map buffer
char maze[30][50];

int prow, pcol;             // Player initial position
int mrow, mcol;             // Mummy initial position
int erow, ecol;             // Exit/treasure position
const int MAZER = 10, MAZEC = 20; // Maze dimensions
const double WALL_DENSITY = 0.13; // Wall density
//*---------------------CONFIG AREA ENDING-----------------*//

bool gameover();
void showmap();
void pmove(char direction); // w = up, s = down, a = left, d = right
void mmove();
void clearscreen();
void generate_map();

int main()
{
    srand(static_cast<unsigned int>(time(NULL)));
    
    // Generate random valid maze using BFS
    generate_map();

    char dire;

    while(!gameover())
    {
        clearscreen();
        showmap();
        cout << "Enter [w|a|s|d] to move: ";
        cin >> dire;
        
        // fix issue #2
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        pmove(dire);

        // The mummy moves twice per turn
        for(int i = 0; i < 2; i++)
        {
            if(gameover()) break; // Stop moving if game has already ended
            mmove();
        }
    }
    clearscreen();
    showmap();

    // Determine win or lose
    if(prow == erow && pcol == ecol)
    {
        cout << "Congratulations! You escaped!" << endl;
    } else if(prow == mrow && pcol == mcol)
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

// Generates a solvable and playable map randomly using BFS
void generate_map()
{
    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};

    while(true)
    {
        // 1. Initialize boundary and random interior walls
        for(int i = 0; i < MAZER; i++)
        {
            for(int j = 0; j < MAZEC; j++)
            {
                if(i == 0 || i == MAZER - 1 || j == 0 || j == MAZEC - 1)
                {
                    maze[i][j] = '#';
                }
                else
                {
                    double r = (double)rand() / RAND_MAX;
                    maze[i][j] = (r < WALL_DENSITY) ? '#' : '.';
                }
            }
        }

        // 2. Randomly pick a free space for Player (P)
        vector<pair<int, int>> free_cells;
        for(int i = 1; i < MAZER - 1; i++)
        {
            for(int j = 1; j < MAZEC - 1; j++)
            {
                if(maze[i][j] == '.')
                {
                    free_cells.push_back({i, j});
                }
            }
        }

        if(free_cells.empty()) continue;

        int p_idx = rand() % free_cells.size();
        prow = free_cells[p_idx].first;
        pcol = free_cells[p_idx].second;

        // 3. BFS from player position to find reachable cells and shortest distances
        vector<vector<int>> dist(MAZER, vector<int>(MAZEC, -1));
        queue<pair<int, int>> q;

        dist[prow][pcol] = 0;
        q.push({prow, pcol});

        vector<pair<int, pair<int, int>>> reachable; // Stores {distance, {r, c}}

        while(!q.empty())
        {
            auto [cr, cc] = q.front();
            q.pop();

            reachable.push_back({dist[cr][cc], {cr, cc}});

            for(int d = 0; d < 4; d++)
            {
                int nr = cr + dr[d];
                int nc = cc + dc[d];

                if(nr >= 0 && nr < MAZER && nc >= 0 && nc < MAZEC)
                {
                    if(maze[nr][nc] == '.' && dist[nr][nc] == -1)
                    {
                        dist[nr][nc] = dist[cr][cc] + 1;
                        q.push({nr, nc});
                    }
                }
            }
        }

        // 4. Validate connectivity (ensure map is not too narrow or blocked)
        int total_inner_cells = (MAZER - 2) * (MAZEC - 2);
        if((int)reachable.size() < total_inner_cells * 0.45)
        {
            continue; // Not enough connected space, regenerate
        }

        // 5. Select Exit (E): Pick among the farthest reachable cells
        sort(reachable.begin(), reachable.end());
        int max_dist = reachable.back().first;
        if(max_dist < 6) continue; // Player and exit should not be too close

        vector<pair<int, int>> exit_candidates;
        for(const auto& item : reachable)
        {
            if(item.first >= max_dist - 2) // Within top distance bracket
            {
                exit_candidates.push_back(item.second);
            }
        }

        int e_idx = rand() % exit_candidates.size();
        erow = exit_candidates[e_idx].first;
        ecol = exit_candidates[e_idx].second;

        // 6. Select Mummy (M): Must be reachable, dist >= 5 to prevent instant kill, and != Exit
        vector<pair<int, int>> mummy_candidates;
        for(const auto& item : reachable)
        {
            int d = item.first;
            int r = item.second.first;
            int c = item.second.second;

            if(d >= 5 && !(r == erow && c == ecol))
            {
                mummy_candidates.push_back({r, c});
            }
        }

        if(mummy_candidates.empty()) continue;

        int m_idx = rand() % mummy_candidates.size();
        mrow = mummy_candidates[m_idx].first;
        mcol = mummy_candidates[m_idx].second;

        break; // Successfully generated a valid layout
    }
}

// multi-platform support for Windows, Mac and Linux.
void clearscreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Dynamically render the map based on current coordinates
void showmap()
{
    for(int i = 0; i < MAZER; i++)
    {
        for(int j = 0; j < MAZEC; j++)
        {
            // Priority: Draw player, then mummy, then exit, finally the map structure
            if (i == prow && j == pcol)
            {
                cout << 'p';
            }
            else if (i == mrow && j == mcol)
            {
                cout << 'm';
            }
            else if (i == erow && j == ecol)
            {
                cout << 'e';
            }
            else
            {
                cout << maze[i][j];
            }
        }
        cout << endl;
    }
    cout << endl;
}

bool gameover()
{
    if((prow == mrow && pcol == mcol) || (prow == erow && pcol == ecol))
    {
        return true;
    }
    return false;
}

void pmove(char direction)
{
    int nrow = prow, ncol = pcol;
    if(direction == 'w') nrow--;
    if(direction == 's') nrow++;
    if(direction == 'a') ncol--;
    if(direction == 'd') ncol++;
    
    // Check if the target position is inside and not a wall
    if(nrow >= 0 && nrow < MAZER && ncol >= 0 && ncol < MAZEC && maze[nrow][ncol] != '#')
    {
        prow = nrow;
        pcol = ncol;
    }
}

// Update mummy coordinates based on rules
void mmove()
{
    int next_row = mrow;
    int next_col = mcol;

    if(mrow != prow) 
    {
        if(mcol > pcol && maze[mrow][mcol-1] != '#')
            next_col--;
        else if(mcol < pcol && maze[mrow][mcol+1] != '#')
            next_col++;
        // If horizontal move is blocked or unnecessary, attempt vertical movement
        else if(mrow > prow && maze[mrow-1][mcol] != '#')
            next_row--;
        else if(mrow < prow && maze[mrow+1][mcol] != '#')
            next_row++;
    }
    else // If in the same row, horizontal movement
    {
        if(mcol > pcol && maze[mrow][mcol-1] != '#')
            next_col--;
        else if(mcol < pcol && maze[mrow][mcol+1] != '#')
            next_col++;
    }
    
    mrow = next_row;
    mcol = next_col;
}

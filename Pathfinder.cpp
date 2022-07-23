	#include <string>
    #include <vector>
    #include <sstream>
    #include <array>
    #include <fstream>
    #include "Pathfinder.h"
    #include <cstdlib>
    #include <algorithm>
    
    string Pathfinder::toString() const {
	    stringstream ss;
        for(int lyr = 0; lyr < LAYER_SIZE; lyr++) {
            for(int row = 0; row < ROW_SIZE; row++) {
                for(int col = 0; col < COL_SIZE; col++) {
                    if (col < 4)
                        ss << maze_grid[lyr][row][col] << " ";
                    else
                        ss << maze_grid[lyr][row][col];
                }
                ss << endl;
            }
            if(lyr <4)
                ss << endl;
        }
	    return ss.str();
    }

	/*
	* createRandomMaze
	*
	* Generates a random maze and stores it as the current maze.
	*
	* The generated maze must contain a roughly equal number of 1s and 0s and must have a 1
	* in the entrance cell (0, 0, 0) and in the exit cell (4, 4, 4).  The generated maze may be
	* solvable or unsolvable, and this method should be able to produce both kinds of mazes.
	*/
	void Pathfinder::createRandomMaze() {
        for(int lyr=0; lyr < LAYER_SIZE; lyr++) {
            for(int row=0; row < ROW_SIZE; row++) {
                for(int col=0; col < COL_SIZE; col++) {
                    int num = rand() % 2;
                    if (lyr==0 && row==0 && col==0) {
                        maze_grid[lyr][row][col] = BACKGROUND;
                    }
                    else if (lyr==(LAYER_SIZE-1) && row==(ROW_SIZE-1) && col==(COL_SIZE-1)) {
                        maze_grid[lyr][row][col] = BACKGROUND;
                    }
                    else {
                        maze_grid[lyr][row][col] = num;
                    }
                    cout << maze_grid[lyr][row][col] << " ";
                }
                cout << endl;
            }
            cout << endl;
        }
    }
	//-----------------------------------------------------------------------------------------

	//Part 2-----------------------------------------------------------------------------------
	/*
	* importMaze
	*
	* Reads in a maze from a file with the given file name and stores it as the current maze.
	* Does nothing if the file does not exist or if the file's data does not represent a valid
	* maze.
	*
	* The file's contents must be of the format described above to be considered valid.
	*
	* Parameter:	file_name
	*				The name of the file containing a maze
	* Returns:		bool
	*				True if the maze is imported correctly; false otherwise
	*/
	bool Pathfinder::importMaze(string file_name) {
		cout << "importMaze from "<<file_name<<endl;

        for(int i=0; i<LAYER_SIZE; i++) {
            for (int j=0;j<ROW_SIZE;j++) {
                for(int k=0;k<COL_SIZE;k++) {
                    maze_grid[i][j][k] = BACKGROUND;
                }
            }
        }
        
		ifstream file (file_name.c_str());
		if (file.is_open()) {
            //cout << "Imported maze" << endl;
			string line;
            int test_grid[LAYER_SIZE][ROW_SIZE][COL_SIZE];
			int row_count = 0;
            for(int lyr=0; lyr < LAYER_SIZE; lyr++) {
                for(int row = 0; row < ROW_SIZE; row++) {
                    //getline(file, line);
                    //if(line.empty()) {getline(file, line);}
                    //stringstream ss(line);
                    for(int col = 0; col < COL_SIZE; col++) {
                        int value;
                        if(file.eof())
                            return false;
                        file >> value;
                        if (value > 1 || value < 0) {
                            cout << "This file contains numbers other than 1 and 0" << endl;
                            return false;
                        }
                        // else if () {
                        //     cout << "This file contains nonnumeric values" << endl;
                        //     return false;
                        // }
                        test_grid[lyr][row][col] = value;   
                    }
                }
            }
            if(!file.eof())
                return false;

            if(test_grid[0][0][0] != 1 || test_grid[4][4][4] != 1) {
                cout << "This maze does not have a proper entrance and exit" << endl;
                return false;
            }
            for(int i=0; i<LAYER_SIZE; i++) {
                for (int j=0;j<ROW_SIZE;j++) {
                    for(int k=0;k<COL_SIZE;k++) {
                        maze_grid[i][j][k] = test_grid[i][j][k];
                    }
                }
            }
            return(true);
		}
	    else {
            cout << "Exiting importMaze" << endl;
            return(false);
        }
        
    }
	//-----------------------------------------------------------------------------------------

	//Part 3-----------------------------------------------------------------------------------
	/*
	* solveMaze
	*
	* Attempts to solve the current maze and returns a solution if one is found.
	*
	* A solution to a maze is a list of coordinates for the path from the entrance to the exit
	* (or an empty vector if no solution is found). This list cannot contain duplicates, and
	* any two consecutive coordinates in the list can only differ by 1 for only one
	* coordinate. The entrance cell (0, 0, 0) and the exit cell (4, 4, 4) should be included
	* in the solution. Each string in the solution vector must be of the format "(x, y, z)",
	* where x, y, and z are the integer coordinates of a cell.
	*
	* Understand that most mazes will contain multiple solutions
	*
	* Returns:		vector<string>
	*				A solution to the current maze, or an empty vector if none exists
	*/
	vector<string> Pathfinder::solveMaze() {
        solution.clear();
        int solve_grid[5][5][5];
        for(int i=0; i<LAYER_SIZE; i++) {
            for (int j=0;j<ROW_SIZE;j++) {
                for(int k=0;k<COL_SIZE;k++) {
                    solve_grid[i][j][k] = maze_grid[i][j][k];
                }
            }
        }

        find_maze_path(solve_grid, 0, 0, 0);

        // for (int i=0; i <solution.size(); i++) {
        //     cout << solution[i] << endl;
        // }
        reverse(solution.begin(),solution.end());

        return solution;
    }

    bool Pathfinder::find_maze_path(int grid[5][5][5], int l, int r, int c) {
        //cout << "find_maze_path ["<<l<<"]["<<r<<"]["<<c<<"]"<<endl;
        //cout << this->toString();
        if (l < 0 || r < 0 || c < 0 || l >= LAYER_SIZE || r >= ROW_SIZE || c >= COL_SIZE )
            return false;      // Cell is out of bounds.
        else if (grid[l][r][c] != BACKGROUND)
            return false;      // Cell is on barrier or dead end.
        else if (r == ROW_SIZE - 1 && c == COL_SIZE - 1 && l == LAYER_SIZE - 1) {
            grid[l][r][c] = PATH;         // Cell is on path
            solution.push_back("("+to_string(c)+", "+to_string(r)+", "+to_string(l)+")");
            return true;               // and is maze exit.
        }
        else { 
            // Recursive case.
            // Attempt to find a path from each neighbor.
            // Tentatively mark cell as on path.
            grid[l][r][c] = PATH;
            if (find_maze_path(grid, l, r - 1, c) // Up
                || find_maze_path(grid, l, r + 1, c) // Down
                || find_maze_path(grid, l, r, c - 1) // Left
                || find_maze_path(grid, l, r, c + 1) 
                || find_maze_path(grid, l - 1, r, c)
                || find_maze_path(grid, l + 1, r, c)) { // Right
                solution.push_back("("+to_string(c)+", "+to_string(r)+", "+to_string(l)+")");
                return true;
            }
            else {
                grid[l][r][c] = TEMPORARY;  // Dead end.
                return false;
            }
        }
    }
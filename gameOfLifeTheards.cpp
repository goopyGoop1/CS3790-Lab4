#include <iostream>        // Provides standard input-output functionality
#include <vector>          // Provides the vector container
#include <thread>          // Enables multithreading
#include <mutex>           // Provides mutual exclusion for thread-safe operations
#include <cstdlib>         // Provides standard library functions like rand() and srand()
#include <ctime>           // Used for seeding random number generation with time
#include <unistd.h>        // Provides access to the sleep() function and system calls
using namespace std;       // Allows standard library names to be used without std::

const int ALIVE = 1;       // Represents a live cell
const int DEAD = 0;        // Represents a dead cell

mutex mtx;                 // Mutex for thread-safe operations (not fully utilized in the code)

struct cell {              // Structure to represent a cell in the grid
    int x, y;              // x and y coordinates of the cell
};

int numOfNeighbors(const vector<vector<int>> &board, int x, int y, int M) {
    int liveNeighbors = 0; 				// Counter for live neighbors
    for (int i = -1; i <= 1; i++) {               // Iterate over the 3x3 neighborhood
        for (int j = -1; j <= 1; j++) {           // Nested loop for rows and columns
            if (i == 0 && j == 0) continue;       // Skip the cell itself
            int neighborX = x + i;               // Compute neighbor's x-coordinate
            int neighborY = y + j;               // Compute neighbor's y-coordinate
            if (neighborX >= 0 && neighborX < M && neighborY >= 0 && neighborY < M && board[neighborX][neighborY] == ALIVE) {
                liveNeighbors++;                 // Increment if neighbor is alive
            }
        }
    }
    return liveNeighbors;                        // Return the count of live neighbors
}

void computeRegion(vector<vector<int>> &board, vector<vector<int>> &newBoard, int startRow, int endRow, int startCol, int endCol, int M) {
    for (int i = startRow; i <= endRow; i++) {             // Loop through assigned rows
        for (int j = startCol; j <= endCol; j++) {         // Loop through assigned columns
            int liveNeighbors = numOfNeighbors(board, i, j, M); // Count live neighbors
            if (board[i][j] == ALIVE) {                        // Check if cell is alive
                newBoard[i][j] = (liveNeighbors == 2 || liveNeighbors == 3) ? ALIVE : DEAD; // Apply Game of Life rules
            } else {                                           // If cell is dead
                newBoard[i][j] = (liveNeighbors == 3) ? ALIVE : DEAD;                       // Apply Game of Life rules
            }
        }
    }
}

void display(const vector<vector<int>> &board, int M) {
    // mtx.lock();  // (Commented out) Lock mutex for thread-safe display

    system("clear");   // Clear the console screen (Unix-based systems)

    for (int i = 0; i < M; i++) {                 // Iterate over rows
        for (int j = 0; j < M; j++) {             // Iterate over columns
            cout << (board[i][j] == ALIVE ? "*" : " "); // Display '*' for ALIVE, ' ' for DEAD
        }
        cout << endl;                             // Newline after each row
    }
    // mtx.unlock();  // (Commented out) Unlock mutex after display

    sleep(1);             // Pause for 1 second to show the next generation
}

int main(int argc, char *argv[]) {
    if (argc != 4) {      // Ensure three command-line arguments are provided
        cerr << "Usage: " << argv[0] << " M N MAX" << endl; // Print usage instructions
        return 1;         // Exit with error code
    }

    int M = stoi(argv[1]); // Size of the grid
    int N = stoi(argv[2]); // Number of threads in each dimension
    int MAX = stoi(argv[3]); // Number of generations to simulate

    srand(time(0));        // Seed random number generator with current time

    vector<vector<int>> board(M, vector<int>(M, DEAD)); // Initialize MxM board with DEAD cells

    for (int i = 0; i < M; i++) {           // Populate the board randomly
        for (int j = 0; j < M; j++) {
            board[i][j] = (rand() % 2 == 0) ? ALIVE : DEAD; // Randomly assign cells as ALIVE or DEAD
        }
    }

    int region = M / N;     // Determine the size of each thread's assigned region

    for (int gen = 0; gen < MAX; gen++) {  // Loop for the number of generations
        vector<thread> threads;            // Container to hold threads
        vector<vector<int>> nextBoard = board; // Create a copy of the current board

        for (int i = 0; i < N; i++) {        // Divide the board into N regions along rows
            for (int j = 0; j < N; j++) {    // Divide the board into N regions along columns
                int startRow = i * region;     // Calculate starting row
                int endRow = startRow + region - 1; // Calculate ending row
                int startCol = j * region;     // Calculate starting column
                int endCol = startCol + region - 1; // Calculate ending column

                threads.emplace_back(thread(computeRegion, ref(board), ref(nextBoard), startRow, endRow, startCol, endCol, M));
                // Create a thread to process a region
            }
        }

        for (auto &t : threads) {           // Wait for all threads to complete
            t.join();
        }

        display(nextBoard, M);              // Display the new board
       
        board = nextBoard;                  // Update the board for the next generation
    }
    
    system("clear");   // Clear the screen after the simulation ends

    return 0;          
}
  


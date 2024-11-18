# CS3790-Lab4 Chad
Game of Life w/Threads

## Overview
This program implements **Conway's Game of Life** using **multithreading** in C++. The simulation divides the grid into regions, assigning each to a separate thread to compute the state of cells for the next generation. The program visualizes the evolution of the grid over multiple generations in the console.

## Features
- **Multithreading**: Parallel computation of cell states for improved performance on larger grids.
- **Dynamic Grid Size**: User can specify the size of the grid.
- **Customizable Parameters**: Control the number of threads and the number of generations.
- **Random Initialization**: Grid cells are randomly assigned as alive or dead at the start.
- **Real-Time Visualization**: Displays each generation with a pause for better visualization.

## Prerequisites
- **C++ Compiler**: Ensure you have a C++11 (or newer) compliant compiler.
- **Unix-based System**: The program uses the `system("clear")` command for console clearing and `unistd.h` for sleeping, which are Unix-specific.

## Compilation
To compile the program, use a C++ compiler like `g++`:

```bash
g++ -std=c++11 -pthread -o gameOfLife gameOfLifeTheards.cpp
```

## Usage
Run the program with the following syntax:

```bash
./gameOfLife M N MAX
```

- `M`: Size of the grid (MxM).
- `N`: Number of threads in each dimension (total threads = N x N).
- `MAX`: Number of generations to simulate.

### Example
To simulate a 10x10 grid with 4 threads (2x2 regions) for 50 generations:

```bash
./gameOfLife 10 2 50
```

## How It Works
1. **Initialization**:
   - The program initializes an MxM grid where each cell is either alive (`*`) or dead (` `).
   - The initial state is generated randomly.

2. **Simulation**:
   - The grid is divided into regions, and threads compute the next state for their assigned regions.
   - The rules of Conway's Game of Life are applied:
     - Any live cell with 2 or 3 neighbors survives.
     - Any dead cell with exactly 3 neighbors becomes alive.
     - All other cells die or remain dead.
   - Threads update a new grid in parallel, which replaces the current grid after each generation.

3. **Display**:
   - Each generation is displayed in the console with a 1-second pause.

4. **Termination**:
   - After simulating `MAX` generations, the program clears the console and exits.

## Code Structure
- **`numOfNeighbors`**: Counts live neighbors for a cell.
- **`computeRegion`**: Computes the next state of a subregion of the grid.
- **`display`**: Clears the console and displays the current state of the grid.
- **`main`**: Handles initialization, threading, and overall simulation flow.

## Limitations
- Designed for Unix-based systems; may require modifications for Windows (e.g., replace `system("clear")`).
- No dynamic load balancing; thread regions are statically assigned.



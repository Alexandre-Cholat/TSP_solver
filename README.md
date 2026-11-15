# TSP_solver

A C++20 command-line solver for the Symmetric Traveling Salesman Problem (TSP) that parses plain and gzip-compressed TSPLIB benchmark instances and generates standard TSPLIB `.tour` files.

Alongside a deterministic baseline solver, the project implements an **Ant Colony Optimization (ACO)** metaheuristic. Inspired by the foraging behavior of ants, a swarm of artificial agents iteratively constructs complete Hamiltonian tours across the graph. At each step, an ant selects its next unvisited city probabilistically based on a desirability weight combining pheromone trail intensity and heuristic visibility ($\tau_{ij} / d_{ij}$), naturally favoring shorter edges and established routes. After all ants complete a tour in a given iteration, pheromone trails are deposited along each ant's path inversely proportional to its total tour length ($1 / L$). Over successive iterations, shorter tours accumulate stronger pheromone concentrations, guiding the swarm toward high-quality solutions while retaining stochastic exploration to avoid local optima.

## Benchmark Instances (TSPLIB)

You can download standard TSP problem instances and known optimal tours to test the solver from the official TSPLIB repository:

* **TSPLIB95 Documentation & Instances:** https://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/

The solver supports symmetric TSP files (both `.tsp` and `.tsp.gz`) formatted with:
* `NODE_COORD_SECTION` (2D Euclidean coordinates)
* `EDGE_WEIGHT_SECTION` (lower-triangular explicit edge weight matrices)

## Prerequisites

* **C++ Compiler:** GCC 10+ or Clang 11+ with C++20 support
* **CMake:** Version 3.14 or higher
* **zlib:** Compression library for reading `.tsp.gz` archives (`sudo apt install zlib1g-dev` on Ubuntu/Debian)
* **Python 3:** Required for running the `eval.py` evaluation script

## Building

Compile the solver and unit test suite in Release mode (`-O3`):

```bash
mkdir -p build
cd build
cmake ..
make
```

*(Alternatively, run `bash build-TSPsolver.sh` from the project root if using the build script.)*

To run the GoogleTest suite:

```bash
./build/tests
```

## Usage

Run the executable from the project root by specifying the input instance path, the desired output `.tour` path, and the solver method (`-m`):

```bash
./build/TSPsolver <instance_path> <output_path> [-m <algorithm>]
```

### Available Algorithms (`-m`)

* `ants`: Runs the Ant Colony Optimization swarm solver (`AntSwarmSolver`).
* `trivial`: Generates a sequential baseline tour visiting nodes `1` through `N` in order (`TrivialSolver`, default if `-m` is omitted).

### Examples

Solve an instance using Ant Colony Optimization:
```bash
./build/TSPsolver pa561.tsp.gz pa561.tsp.tour -m ants
```

Generate a baseline tour using the trivial solver:
```bash
./build/TSPsolver pa561.tsp.gz pa561.tsp.tour -m trivial
```

## Evaluating Tours (`eval.py`)

Use the included `eval.py` script to verify that a generated `.tour` file forms a valid Hamiltonian cycle (visiting every city `1..N` exactly once), compute its total tour distance, and compare your solver's performance against optimal TSPLIB reference tours:

```bash
python3 eval.py <instance_path> <tour_path>
```

For example:
```bash
python3 eval.py pa561.tsp.gz pa561.tsp.tour
```

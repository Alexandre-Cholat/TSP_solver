# TSP_solver

A **C++20** command-line solver for the Symmetric Traveling Salesman Problem (TSP) that parses [TSPLIB](https://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/) benchmark instances (`NODE_COORD_SECTION` with `EUC_2D`, `GEO`, and `ATT` metrics, as well as `EDGE_WEIGHT_SECTION` lower-triangular matrices) and outputs valid TSPLIB `.tour` files.

## Solvers

* **Trivial (`trivial`)**: A deterministic baseline solver that visits vertices sequentially ($1 \rightarrow 2 \rightarrow \dots \rightarrow N \rightarrow 1$).
* **Held-Karp (`held-karp`)**: An exact dynamic programming solver using bitmask subset representation ($O(n^2 2^n)$ time, $O(n 2^n)$ space) to compute provably optimal tours for small instances ($n \le 20$).
* **Ant Colony Optimization (`aco` / `AntSwarmSolver`)**: A stochastic metaheuristic inspired by ant foraging behavior. A swarm of artificial agents iteratively constructs complete Hamiltonian tours across a flattened 1D lower-triangular distance matrix. At each step, an ant selects its next unvisited city $j$ from city $i$ probabilistically using a desirability weight combining pheromone intensity $\tau_{ij}$ and heuristic visibility $\eta_{ij} = 1 / d_{ij}$:

  $$w_{ij} = (\tau_{ij})^{\alpha} \cdot \left(\frac{1}{d_{ij}}\right)^{\beta}$$

  After each iteration, existing pheromone trails undergo evaporation ($\rho = 0.10$) and each ant deposits new pheromone along its tour inversely proportional to its total tour length ($\Delta \tau_{ij} = 1 / L$). Over successive iterations, shorter routes accumulate stronger pheromone concentrations, guiding the swarm toward near-optimal solutions on larger instances while maintaining exploration to escape local optima.

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

```bash
bash build-TSPsolver.sh
```

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
source .venv/bin/activate
cd build
cmake -DBUILD_PYTHON=ON ..
cmake --build .
cd ..
python src/python/eval.py
```

* **Python Evaluation (`pybind11`)**: Exposes `LowerTriMatrix` and all C++ solvers to `src/python/eval.py` for runtime scaling benchmarks and $(\alpha, \beta)$ hyperparameter sensitivity analysis.

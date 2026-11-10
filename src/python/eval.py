import sys
import time
from pathlib import Path
import random
# to run : 
# .venv/bin/activate
# cd build
# cmake -DBUILD_PYTHON=ON ..
# cmake --build .
# python src/python/eval.py

# TODO:     • Compare the quality of the solutions obtained by each algorithm (i.e., the length of the tour)
        # • Analyze the scalability of each algorithm as the size of the instances increases.
        # • Analyze the behavior of the algorithms on specific instances using visualizations tools.

# eval choice: use parser or directly pass lower triangular matrix to the solvers?

# Locate the 'build' directory relative to this file (src/python/eval.py -> ../../build)
PROJECT_ROOT = Path(__file__).resolve().parent.parent.parent
BUILD_DIR = PROJECT_ROOT / "build"
sys.path.insert(0, str(BUILD_DIR))

import tsp_bindings

# use instance_generator.py to generate instances?
def generate_random_matrix(n_nodes: int, seed: int = 42) -> tsp_bindings.LowerTriMatrix:
    """Generates a C++ LowerTriMatrix with random weights for N nodes."""
    random.seed(seed)
    num_weights = (n_nodes * (n_nodes - 1)) // 2
    weights = [random.uniform(10.0, 100.0) for _ in range(num_weights)]
    
    # Instantiate the bound C++ LowerTriMatrix class directly
    return tsp_bindings.LowerTriMatrix(n_nodes, weights)


def compute_tour_cost(matrix: tsp_bindings.LowerTriMatrix, tour: list[int]) -> float:
    """Computes total tour length using the C++ LowerTriMatrix.get_distance method."""
    if len(tour) <= 1:
        return 0.0
    cost = 0.0
    for i in range(len(tour) - 1):
        cost += matrix.get_distance(tour[i], tour[i + 1])
    cost += matrix.get_distance(tour[-1], tour[0])
    return cost


def measure_solver(solver, matrix: tsp_bindings.LowerTriMatrix) -> tuple[float, float, list[int]]:
    """
    Measures execution time and tour cost for any bound C++ solver instance.
    
    Returns:
        (elapsed_seconds, tour_cost, tour_list)
    """
    start_time = time.perf_counter()
    tour = solver.solve(matrix)
    end_time = time.perf_counter()
    
    elapsed_sec = end_time - start_time
    cost = compute_tour_cost(matrix, tour)
    return elapsed_sec, cost, tour


def main():
    n_nodes = 12
    matrix = generate_random_matrix(n_nodes)

    # Instantiate your C++ solvers directly in Python
    solvers = {
        "Trivial": tsp_bindings.TrivialSolver(),
        "AntSwarm": tsp_bindings.AntSwarmSolver(),
        "HeldKarp": tsp_bindings.HeldKarpSolver(),
    }

    print(f"--- Benchmark (N = {n_nodes}) ---")
    for name, solver in solvers.items():
        elapsed, cost, tour = measure_solver(solver, matrix)
        print(f"{name:<10} | Time: {elapsed * 1000:8.3f} ms | Cost: {cost:8.1f} | Tour: {tour}")


if __name__ == "__main__":
    main()
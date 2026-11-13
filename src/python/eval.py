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

# alt: use instance_generator.py to generate instances?
def generate_random_matrix(n_nodes: int, seed: int = 42) -> tsp_bindings.LowerTriMatrix:
    """Generates a C++ LowerTriMatrix with random weights for N nodes."""
    random.seed(seed)
    num_weights = (n_nodes * (n_nodes - 1)) // 2
    weights = [random.uniform(10.0, 100.0) for _ in range(num_weights)]
    
    # Instantiate LowerTriMatrix class
    return tsp_bindings.LowerTriMatrix(n_nodes, weights)

def generate_n_random_matrix(n_nodes: int, n_instances: int, seed: int = 42) -> list[tsp_bindings.LowerTriMatrix]:
    """Generates a list of C++ LowerTriMatrix instances with random weights for N nodes."""
    random.seed(seed)
    matrices = []
    for _ in range(n_instances):
        num_weights = (n_nodes * (n_nodes - 1)) // 2
        weights = [random.uniform(10.0, 100.0) for _ in range(num_weights)]
        matrices.append(tsp_bindings.LowerTriMatrix(n_nodes, weights))
    return matrices


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

def measure_solver_multiple(solver, matrices: list[tsp_bindings.LowerTriMatrix]) -> tuple[float, float, list[list[int]]]:
    """
    Measures execution time and tour cost for any bound C++ solver instance over multiple matrices.
    
    Returns:
        (total_elapsed_seconds, total_tour_cost, list_of_tours)
    """
    total_elapsed_sec = 0.0
    total_cost = 0.0
    tours = []
    
    for matrix in matrices:
        elapsed_sec, cost, tour = measure_solver(solver, matrix)
        total_elapsed_sec += elapsed_sec
        total_cost += cost
        tours.append(tour)
    
    return total_elapsed_sec, total_cost, tours

def simple_test():
    n_nodes = 5
    matrix = generate_random_matrix(n_nodes)

    # Instantiate solvers
    solvers = {
        "Trivial": tsp_bindings.TrivialSolver(),
        "AntSwarm": tsp_bindings.AntSwarmSolver(),
        "HeldKarp": tsp_bindings.HeldKarpSolver(),
    }

    print(f"--- Simple Test (N = {n_nodes}) ---")
    for name, solver in solvers.items():
        elapsed, cost, tour = measure_solver(solver, matrix)
        print(f"{name:<10} | Time: {elapsed * 1000:8.3f} ms | Cost: {cost:8.1f} | Tour: {tour}")

def benchmark_multiple_instances(n_nodes: int, n_instances: int):
    matrices = generate_n_random_matrix(n_nodes, n_instances)

    # Instantiate solvers
    solvers = {
        "Trivial": tsp_bindings.TrivialSolver(),
        "AntSwarm": tsp_bindings.AntSwarmSolver(),
        "HeldKarp": tsp_bindings.HeldKarpSolver(),
    }

    print(f"--- Benchmark Multiple Instances (N = {n_nodes}, Instances = {n_instances}) ---")
    for name, solver in solvers.items():
        total_elapsed, total_cost, tours = measure_solver_multiple(solver, matrices)
        avg_elapsed = total_elapsed / n_instances
        avg_cost = total_cost / n_instances
        print(f"{name:<10} | Avg Time: {avg_elapsed * 1000:8.3f} ms | Avg Cost: {avg_cost:8.1f}")


def main():
    n_nodes = 12
    matrix = generate_random_matrix(n_nodes)

    # Instantiate solvers
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
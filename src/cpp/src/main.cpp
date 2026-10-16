#include <iostream>
#include <string>
#include <vector>

// placeholder to satisfy the build-TSPsolver.sh compilation requirement
int main(int argc, char* argv[]) {
    // The executable takes two mandatory positional arguments
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <instance_path> <output_path> [-m <algorithm>]\n";
        return 1;
    }

    std::string instance_path = argv[1];
    std::string output_path = argv[2];
    std::string method = "trivial"; // Default method specified by the project

    // Parse with the optional -m algorithm flag
    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-m" && i + 1 < argc) {
            method = argv[i + 1];
        }
    }

    std::cout << "TSPsolver called.\n"
              << "Instance: " << instance_path << "\n"
              << "Output: " << output_path << "\n"
              << "Method: " << method << std::endl;

    return 0;
}
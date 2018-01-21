#include "ecosystem.hh"
#include "solver.hh"
#include <fstream>
#include <string>

using namespace std;

/* Prints out a usage statement. */
void usage(char *name) {
    cout << "Usage:\n    " << name << " [-g] biome iterations\n or\n    " 
         << name << " -s biome iterations generation_size num_kept generations"
         << "\nWhere -s is used to call the genetic algorithm to try to find "
         << "stable population parameters, or -g (or the default) will run an "
         << "ecosystem simulation with the current populations.\n"; 
}

int main(int argc, char **argv) {
    string biome;
    int iterations;
    if (argc == 3 || argc == 4) {
        if (argc == 4 && (string)argv[1] != "-g") {
            usage(argv[0]);
            return 1;
        }
        biome = argv[argc - 2];
        iterations = atoi(argv[argc - 1]);
        Ecosystem eco(biome);
        ofstream file("out.csv");
        eco.WriteNames(file);
        for (int i = 0; i < iterations; i++) {
            eco.Update();
            eco.Output(file);
        }
    }
    else if (argc == 7) {
        if ((string)argv[1] != "-s") {
            usage(argv[0]);
            return 1;
        }
        biome = argv[2];
        iterations = atoi(argv[3]);
        int gen_size = atoi(argv[4]);
        int num_kept = atoi(argv[5]);
        int generations = atoi(argv[6]);
       Solver solver(biome);
        solver.Solve(iterations, gen_size, num_kept, generations);
        
    }
    else {
        usage(argv[0]);
        return 1;
    }

/*
    Ecosystem eco("grassland");
    int iterations = 20;
    ofstream file("out.csv");
    eco.WriteNames(file);
    for (int i = 0; i < iterations; i++) {
        eco.Update();
        eco.Output(file);
    }
*/
    return 0;
}

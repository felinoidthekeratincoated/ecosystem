#include "ecosystem.hh"
#include "solver.hh"
#include <fstream>

int main(int argc, char ** argv) {

    Solver solver;
    solver.Solve();

/*
    Ecosystem eco;
    int iterations = 1000000;
    ofstream file("out.csv");
    for (int i = 0; i < iterations; i++) {
        eco.Update();
        if (i % 100 == 0) {
            eco.Output(file);
        }
    }
*/
    return 0;
}

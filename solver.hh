#ifndef SOLVER_HH
#define SOLVER_HH

#include "population.hh"
#include "ecosystem.hh"
#include <random>
#include <vector>

using namespace std;

/* A class for finding initial conditions that result in a stable ecosystem. */
class Solver {
private:
    std::vector<Population> min_pops;
    std::vector<Population> max_pops;
    std::vector<double> min_goal;
    std::vector<double> max_goal;
    std::vector<double> resources;
    std::default_random_engine engine;
    Population randomPop(const Population &low, const Population &high);
    double randrange(double low, double high);
    std::vector<Population> generate();
    double getError(const Ecosystem &eco);
    void moveBounds(const std::vector<std::vector<Population>> &new_bounds);
    vector<vector<Population>> &keepBest(vector<vector<Population>> &pops,
        vector<double> &errors, int keep);
public:
    Solver(string biome);
    void Solve(int iterations, unsigned int num_pops, int num_kept, 
        int generations);
};

#endif

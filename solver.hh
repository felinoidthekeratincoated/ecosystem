#ifndef SOLVER_HH
#define SOLVER_HH

#include "population.hh"
#include "ecosystem.hh"
#include <random>
#include <vector>

/* A class for finding initial conditions that result in a stable ecosystem. */
class Solver {
private:
    int iterations;
    std::vector<Population> min_pops;
    std::vector<Population> max_pops;
    std::default_random_engine engine;
    Population randomPop(const Population &low, const Population &high);
    double randrange(double low, double high);
    std::vector<Population> generate();
    double getError(const Ecosystem &eco);
public:
    Solver();
    void Solve();
};

#endif

#include "solver.hh"
#include <cassert>
#include <fstream>
#include <ctime>

using namespace std;

/* Generate a population with random attributes. Populations low and high
provide the bounds for each attribute. */
Population Solver::randomPop(const Population &low, const Population &high) {
    assert(low.species == high.species);
    assert(low.resources.size() == high.resources.size());
    assert(low.prey.size() == high.prey.size());
    Population rand = low;
    rand.size = randrange(low.size, high.size);
    rand.change_rate = randrange(low.change_rate, high.change_rate);
    for (unsigned int i = 0; i < low.resources.size(); i++) {
        assert(low.resources[i].type == high.resources[i].type);
        rand.resources[i].consumption = randrange(low.resources[i].consumption,
                high.resources[i].consumption);
        rand.resources[i].affinity = randrange(low.resources[i].affinity,
                high.resources[i].affinity);
    }
    for (unsigned int i = 0; i < low.prey.size(); i++) {
        assert(low.prey[i].species == high.prey[i].species);
        rand.prey[i].consumption = randrange(low.prey[i].consumption,
                high.prey[i].consumption);
        rand.prey[i].catch_rate = randrange(low.prey[i].catch_rate,
                high.prey[i].catch_rate);
    }
    
    return rand;
}

/* Returns a random double between low and high. */
double Solver::randrange(double low, double high) {
    assert(low <= high);
    uniform_real_distribution<double> unif(low, high);
    return unif(engine);
}

/* Gets a rating of how badly an ecosystem is doing at keeping the populations
   at the target levels. */
double Solver::getError(const Ecosystem &eco) {
    double error = 0;
    for (unsigned int i = 0; i < min_goal.size(); i++) {
        double size = eco.Get((Species)i).size;
        double over = size / max_goal[i].size;
        double under = 0;
        if (max_goal[i].size != 0) {
            under = min_goal[i].size / size;
        }
        error += max(over * over, under * under);
    }
    return error;
}

/* Change the max and min so population sets like the ones given are more
likely to occur. */
void Solver::moveBounds(const vector<vector<Population>> &new_bounds) {
    assert(new_bounds.size() > 0);
    min_pops = new_bounds[0];
    max_pops = new_bounds[0];
    for (unsigned int i = 1; i < new_bounds.size(); i++) {
        for (unsigned int j = 0; j < min_pops.size(); j++) {
            min_pops[j].setMin(new_bounds[i][j]);
            max_pops[j].setMax(new_bounds[i][j]);
        }
    }
}

/* Remove all but the best population sets, measured by the error vector. 
Modifies the vector of population vectors in place. */
vector<vector<Population>> &Solver::keepBest(vector<vector<Population>> &pops,
        vector<double> &errors, int keep) {
    assert(pops.size() == errors.size());
    assert(keep < (int)pops.size());
    for (unsigned int i = keep; i < pops.size(); i++) {
        for (int j = 0; j < keep; j++) {
            if (errors[i] < errors[j]) {
                // Switch their positions
                double temp = errors[i];
                errors[i] = errors[j];
                errors[j] = temp;
                vector<Population> temppop = pops[i];
                pops[i] = pops[j];
                pops[j] = temppop;
                // And redo in case the displaced one really does belong
                // at the top
                i--;
                break;
            }
        }
    }
    // Remove all but the best ones from pops
    pops.erase(pops.begin() + keep, pops.end());
    errors.erase(errors.begin() + keep, errors.end());
    assert(pops.size() == errors.size());
    return pops;
}

/* Constructor. */
Solver::Solver() {
    // Seed the random number generators
    srand(time(nullptr));
    engine.seed(rand());

    iterations = 4000;

    string suffix;
    string pop_folder;

    vector<string> filenames = Ecosystem::GetFilenames(pop_folder, suffix);

    min_pops = Population::loadPops(pop_folder, filenames, "-min" + suffix);
    min_goal = min_pops;
    max_pops = Population::loadPops(pop_folder, filenames, "-max" + suffix);
    max_goal = max_pops;
    assert(min_pops.size() == max_pops.size());
}

vector<Population> Solver::generate() {
    vector<Population> answer;
    for (unsigned int i = 0; i < min_pops.size(); i++) {
        answer.push_back(randomPop(min_pops[i], max_pops[i]));
    }
    return answer;
}

/* For now, this generates random populations, runs the resulting ecosystem,
and outputs the result to a file, as well as printing a measurement of how
badly that ecosystem did. */
void Solver::Solve() {
    unsigned int num_pops = 40;
    int num_kept = 10;
    int generations = 8;
    vector<vector<Population>> test;
    vector<double> errors;
    assert(test.size() == 0);

    for (int i = 0; i < generations; i++) {
        while (test.size() < num_pops) {
            assert(test.size() == errors.size());
            test.push_back(generate());
            Ecosystem eco(test.back());
            double error = 0;
            for (int k = 0; k < iterations; k++) {
                eco.Update();
                error += getError(eco);
            }
            // Set the population to the ending population
            for (unsigned int k = 0; k < test.back().size(); k++) {
                test.back()[k].size = eco.Get((Species)k).size;
            }
            errors.push_back(error);
            assert(test.size() == errors.size());
        }
        assert(test.size() == errors.size());
        test = keepBest(test, errors, num_kept);
        moveBounds(test);
        cout << "generation " << i << " done.\n";
    }

    test = keepBest(test, errors, 1);
    Ecosystem eco(test[0]);
    eco.Save();
    ofstream file("out.csv");
    for (int j = 0; j < iterations; j++) {
        eco.Update();
        eco.Output(file);
    }
    cout << "Sum squared error: " << errors[0] << "\n";
}

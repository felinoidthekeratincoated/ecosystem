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
    for (unsigned int i = 0; i < min_pops.size(); i++) {
        double size = eco.Get((Species)i).size;
        double over = size / max_pops[i].size;
        double under = min_pops[i].size / size;
        error += max(over * over, under * under);
    }
    return error;
}

/* Constructor. */
Solver::Solver() {
    // Seed the random number generators
    srand(time(nullptr));
    engine.seed(rand());

    iterations = 4000;

    string suffix = ".json";

    ifstream files("files.json");
    json f = json::parse(files);
    vector<string> filenames = f["files"].get<vector<string>>();
    string pop_folder = f["path"];

    for (unsigned int i = 0; i < filenames.size(); i++) {
        /* open file */
        ifstream infile(pop_folder + filenames[i] + "-min" + suffix);
        /* Put data in json. */
        json j = json::parse(infile);
        int index = j["species"];
        min_pops.resize(max((int)min_pops.size(), index + 1));
        min_pops[index] = j.get<Population>();
    }
    for (unsigned int i = 0; i < filenames.size(); i++) {
        /* open file */
        ifstream infile(pop_folder + filenames[i] + "-max" + suffix);
        /* Put data in json. */
        json j = json::parse(infile);
        int index = j["species"];
        max_pops.resize(max((int)max_pops.size(), index + 1));
        max_pops[index] = j.get<Population>();
    }
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
    vector<Population> test = generate();
    Ecosystem eco(test);
    ofstream file("out.csv");
    double error = 0;
    for (int i = 0; i < iterations; i++) {
        eco.Update();
        eco.Output(file);
        error += getError(eco);
    }
    cout << "Sum squared error: " << error << "\n";
}

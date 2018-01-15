#include "population.hh"
#include "resource.hh"
#include "prey.hh"
#include "json.hpp"
#include <cassert>
#include <fstream>

/* For convenience. */
using json = nlohmann::json;

using namespace std;

/* Set each value to the value *this or pop has, whichever is smaller. */
void Population::setMin(const Population &pop) {
    assert(species == pop.species);
    size = min(size, pop.size);
    change_rate = min(change_rate, pop.change_rate);
    for (unsigned int i = 0; i < resources.size(); i++) {
        assert(resources[i].type == pop.resources[i].type);
        resources[i].consumption = min(resources[i].consumption, 
                    pop.resources[i].consumption);
        resources[i].affinity = min(resources[i].affinity, 
                    pop.resources[i].affinity);
    }
    for (unsigned int i = 0; i < prey.size(); i++) {
        assert(prey[i].species == pop.prey[i].species);
        prey[i].consumption = min(prey[i].consumption, pop.prey[i].consumption);
        prey[i].catch_rate = min(prey[i].catch_rate, pop.prey[i].catch_rate);
    }
}

/* Set each value to the value *this or pop has, whichever is larger. */
void Population::setMax(const Population &pop) {
    assert(species == pop.species);
    size = max(size, pop.size);
    change_rate = max(change_rate, pop.change_rate);
    for (unsigned int i = 0; i < resources.size(); i++) {
        assert(resources[i].type == pop.resources[i].type);
        resources[i].consumption = max(resources[i].consumption, 
                    pop.resources[i].consumption);
        resources[i].affinity = max(resources[i].affinity, 
                    pop.resources[i].affinity);
    }
    for (unsigned int i = 0; i < prey.size(); i++) {
        assert(prey[i].species == pop.prey[i].species);
        prey[i].consumption = max(prey[i].consumption, pop.prey[i].consumption);
        prey[i].catch_rate = max(prey[i].catch_rate, pop.prey[i].catch_rate);
    }
}

/* Load a vector of populations from their json files. */
vector<Population> Population::loadPops(string prefix, 
        vector<string> filenames, string suffix) {
    vector<Population> answer;
    for (unsigned int i = 0; i < filenames.size(); i++) {
        /* open file */
        ifstream infile(prefix + filenames[i] + suffix);
        /* Put data in json. */
        json j = json::parse(infile);
        int index = j["species"];
        answer.resize(max((int)answer.size(), index + 1));
        answer[index] = j.get<Population>();
    }
    return answer;
}

/* Load from a json file. */
void from_json(const json &j, Population &p) {
    int species = j["species"];
    p.species = (Species)species;
    p.size = j["size"];
    p.resources = j["resources"].get<vector<Resource>>();
    p.prey = j["prey"].get<vector<Prey>>();
    p.change_rate = j["change_rate"];
}

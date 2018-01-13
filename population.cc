#include "population.hh"
#include "resource.hh"
#include "prey.hh"
#include "json.hpp"

/* For convenience. */
using json = nlohmann::json;

using namespace std;

/* Load from a json file. */
void from_json(const json &j, Population &p) {
    int species = j["species"];
    p.species = (Species)species;
    p.size = j["size"];
    p.resources = j["resources"].get<vector<Resource>>();
    p.prey = j["prey"].get<vector<Prey>>();
    p.reproduction_rate = j["reproduction_rate"];
}

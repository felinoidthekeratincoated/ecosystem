#include "prey.hh"
#include "json.hpp"

/* For convenience. */
using json = nlohmann::json;

using namespace std;

/* Load from a json file. */
void from_json(const json &j, Prey &prey) {
    int species = j["species"];
    prey.species = (Species)species;
    prey.consumption = j["consumption"];
    prey.catch_rate = j["catch_rate"];
}

#ifndef PREY_HH
#define PREY_HH
#include "json.hpp"

/* For convenience. */
using json = nlohmann::json;

enum class Species;

struct Prey {
public:
    Species species;
    double consumption;
    double catch_rate;
};

void from_json(const json &j, Prey &prey);
void to_json(json &j, const Prey &prey);

#endif

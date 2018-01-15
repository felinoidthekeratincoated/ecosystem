#ifndef POPULATION_HH
#define POPULATION_HH

#include <vector>
#include "resource.hh"
#include "prey.hh"
#include "json.hpp"

/* For convenience. */
using json = nlohmann::json;

enum class Species {
    GRASS,
    BUTTERCUP,
    CLOVER,
    HONEYBEE,
    BISON,
    WOLF,
    GRASSHOPPER
};

struct Population {
public:
    Species species;
    double size;
    std::vector<Resource> resources;
    std::vector<Prey> prey;
    double change_rate;
    void setMin(const Population &pop);
    void setMax(const Population &pop);
    static std::vector<Population> loadPops(std::string prefix,
             std::vector<std::string> filenames, std::string suffix);
};

void from_json(const json &j, Population &p);

#endif

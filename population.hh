#ifndef POPULATION_HH
#define POPULATION_HH

#include <vector>
#include "resource.hh"
#include "prey.hh"
#include "json.hpp"

/* For convenience. */
using json = nlohmann::json;

enum class Species {
    GRASS, // 0
    BUTTERCUP,
    RED_CLOVER,
    HONEYBEE,
    BISON,
    WOLF,
    GRASSHOPPER,
    SPIDER,
    BIG_BLUESTEM,
    INDIAN_GRASS,
    SWITCH_GRASS, // 10
    LITTLE_BLUESTEM,
    PRAIRIE_CORDGRASS, 
    GAMA_GRASS, // Has declined in range because of habitat destruction
    PRAIRIE_DROPSEED, // Possibly leave out
    WILD_RYE, // Possibly leave out
    COMPASS_PLANT,
    PRAIRIE_DOCK,
    LEADPLANT,
    ROSINWEED,
    RATTLESNAKE_MASTER,
    WILD_QUININE,
    MISSOURI_IRONWEED,
    GOLDENROD,
    NEW_ENGLAND_ASTER,
    WILD_WHITE_INDIGO,
    HANDSOME_GRASSHOPPER,
    SLANT_FACED_GRASSHOPPER,
    SHORT_WINGED_TOOTHPICK_GRASSHOPPER
    
    
};

struct Population {
public:
    Species species;
    double size;
    std::vector<Resource> resources;
    std::vector<Prey> prey;
    double change_rate;
    Population();
    void setMin(const Population &pop);
    void setMax(const Population &pop);
    static std::vector<Population> loadPops(std::string prefix,
             std::vector<std::string> filenames, std::string suffix);
};

void from_json(const json &j, Population &p);

void to_json(json &j, const Population &p);

#endif

#include "ecosystem.hh"
#include "json.hpp"
#include <string>
#include <fstream>
#include <algorithm>

/* For convenience. */
using json = nlohmann::json;

using namespace std;

Ecosystem::Ecosystem(string biome) {
    string pop_folder, bio_folder, suffix;
    vector<string> filenames = GetFilenames(pop_folder, bio_folder, suffix);

    ifstream type(bio_folder + biome + suffix);
    json t = json::parse(type);
    resources = t["resources"].get<vector<double>>();

    for (unsigned int i = 0; i < filenames.size(); i++) {
        /* open file */
        ifstream infile(pop_folder + filenames[i] + suffix);
        /* Put data in json. */
        json j = json::parse(infile);
        int index = j["species"];
        pops.resize(max((int)pops.size(), index + 1));
        pops[index] = j.get<Population>();
    }
}

Ecosystem::Ecosystem(vector<Population> &populations, vector<double> &r) {
    pops = populations;
    resources = r;
}

/* Update the populations. */
void Ecosystem::Update() {
    // Temporary fix: hardcode the amount of resources
    useResources();
    catchPrey();
    reproduce();
}

/* Output to a file, csv format. */
void Ecosystem::Output(ofstream &file) const {
    for (unsigned int i = 0; i < pops.size(); i++) {
        file << pops[i].size << ",";
    }
    file << "\n";
}

/* Write a comma-separated list of species names to the file. */
void Ecosystem::WriteNames(ofstream &file) const {
    string s, t, u;
    vector<string> filenames = GetFilenames(s, t, u);
    for (unsigned int i = 0; i < filenames.size(); i++) {
        file << filenames[i] << ",";
    }
    file << "\n";
}

/* Get the status of a givem population. */
Population Ecosystem::Get(Species species) const {
    return pops[(int)species];
}

/* Gets a list of all filenames of populations, in order, and changes the 
string prefix to be the path to the files and suffix to be the extension. */
vector<string> Ecosystem::GetFilenames(string &pop_prefix, string &bio_prefix,
        string &suffix) {
    suffix = ".json";

    ifstream files("files.json");
    json f = json::parse(files);
    vector<string> filenames = f["files"].get<vector<string>>();
    pop_prefix = f["pop_path"];
    bio_prefix = f["biome_path"];
    return filenames;
}

/* Saves all populations to json files. */
void Ecosystem::Save() {
    string prefix, biome, suffix;
    vector<string> filenames = Ecosystem::GetFilenames(prefix, biome, suffix);
    for (unsigned int i = 0; i < filenames.size(); i++) {
        ofstream file(prefix + filenames[i] + suffix);
        json j = pops[i];
        file << j;
    }
}

/* Use each resource. */
void Ecosystem::useResources() {
    for (unsigned int i = 0; i < resources.size(); i++) {
        useResource((ResourceType)i, resources[i]);
    }
}

/* Decrease populations that don't have enough resources for all members to
survie. */
double Ecosystem::useResource(ResourceType type, double amount) {
    vector<unsigned int> users;
    vector<unsigned int> res;
    double affinity = 0;
    double total_resource_used = 0;
    for (unsigned int i = 0; i < pops.size(); i++) {
        for (unsigned int j = 0; j < pops[i].resources.size(); j++) {
            if (pops[i].resources[j].type == type) {
                // If that pop consumes this resource
                if (pops[i].resources[j].consumption > 0) {
                    users.push_back(i);
                    res.push_back(j);
                }
                else {
                    // This population produces this resource
                    double gain = -1 * pops[i].resources[j].consumption;
                    amount += gain * pops[i].size;
                }
            }
        }
    }
            
    // Loop through consumers of this resource
    assert(users.size() == res.size());
    for (unsigned int i = 0; i < users.size(); i++) {
        Population pop = pops[users[i]];
        Resource r = pop.resources[res[i]];
        // For calculating the results of interspecies competition
        affinity += pop.size * r.affinity  * r.consumption;
    }
    for (unsigned int i = 0; i < users.size(); i++) {
        Population pop = pops[users[i]];
        if (pop.size == 0) {
            continue;
        }
        Resource r = pop.resources[res[i]];
        double resource_used = amount * pop.size * r.affinity
                * r.consumption / affinity;
        total_resource_used += resource_used;
        // + 1 in case of floating point rounding errors
        assert((int)total_resource_used <=  (int)amount + 1);
        // Kill off population members that didn't get enough resource
        double pop_survived = resource_used / r.consumption;
        pops[users[i]].size = min(pop.size, pop_survived);
    }

    // Return the amount left over
    return amount - total_resource_used;
}

/* Decrease populations of prey and predators based on how many predators 
cought their prey. */
void Ecosystem::catchPrey() {
    // First get initial values of all species, so that which order the 
    // predators catch things in has no effect
    vector<double> initial;
    for (unsigned int i = 0; i < pops.size(); i++) {
        initial.push_back(pops[i].size);
    }

    // Now for actually catching stuff
    for (unsigned int i = 0; i < pops.size(); i++) {
        // Ignore non-predators
        if (pops[i].prey.size() == 0
                || pops[i].size == 0) {
            continue;
        }

        double num_predator = initial[i];
        // Eat the prey
        for (unsigned int j = 0; j < pops[i].prey.size(); j++) {
            Prey prey = pops[i].prey[j];
            if (pops[(int)prey.species].size == 0) {
                continue;
            }
            double num_prey = initial[(int)prey.species];
            double prey_caught = num_predator * num_prey * prey.catch_rate;
            prey_caught = min(prey_caught, num_prey);
            int n = (int)prey.species;
            pops[n].size = max(pops[n].size - prey_caught, 0.0);
            pops[i].size += prey_caught / prey.consumption;
        }
    }
}

/* Grow all populations by change_rate. */
void Ecosystem::reproduce() {
    for (unsigned int i = 0; i < pops.size(); i++) {
        pops[i].size *= pops[i].change_rate;
    }
}

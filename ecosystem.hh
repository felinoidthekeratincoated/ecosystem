#ifndef ECOSYSTEM_HH
#define ECOSYSTEM_HH

#include "population.hh"
#include <vector>
#include <fstream>

using namespace std;

class Ecosystem {
private:
    vector<Population> pops;
    vector<double> resources;
    double useResource(ResourceType resource, double amount);
    void useResources();
    void catchPrey();
    void reproduce();
public:
    Ecosystem(string biome);
    Ecosystem(vector<Population> &populations, vector<double> &resources);
    void Update();
    void Output(ofstream &file) const;
    void WriteNames(ofstream &file) const;
    Population Get(Species species) const;
    static vector<string> GetFilenames(string &pop_prefix, string &bio_prefix,
        string &suffix);
    void Save();
};



#endif

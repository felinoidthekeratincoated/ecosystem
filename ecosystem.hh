#ifndef ECOSYSTEM_HH
#define ECOSYSTEM_HH

#include "population.hh"
#include <vector>
#include <fstream>

using namespace std;

class Ecosystem {
private:
    vector<Population> pops;
    double useResource(ResourceType resource, double amount);
    void useResources();
    void catchPrey();
    void reproduce();
public:
    Ecosystem();
    Ecosystem(vector<Population> &populations);
    void Update();
    void Output(ofstream &file) const;
    Population Get(Species species) const;
    static vector<string> GetFilenames(string &prefix, string &suffix);
    void Save();
};



#endif

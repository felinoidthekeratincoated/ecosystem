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
    void Update();
    void Output(ofstream &file);
};



#endif

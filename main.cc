#include "ecosystem.hh"
#include <fstream>

int main(int argc, char ** argv) {
    Ecosystem ecosystem;
    ofstream file;
    file.open("out.csv");
    for (int i = 0; i < 1600; i++) {
        ecosystem.Update();
        ecosystem.Output(file);
    }
    return 0;
}

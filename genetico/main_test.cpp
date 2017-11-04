#include <iostream>
#include "matingpool.h"

using namespace std;

int main() {
    Board board(6, 7);
    MatingPool mp(6, 7, 4, 21, 10, 10, 1, 0.6f, 0.05f, 0.3f, 1.0f);
    mp.newGeneration();
    Genome g1 = mp.getPopulation().at(0);
    Genome g2 = mp.getPopulation().at(1);
    Genome g3 = mp.crossover(g1, g2);
    displayVector(g3.geneWeights);
    return 0;
}

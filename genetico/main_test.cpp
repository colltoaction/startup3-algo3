#include <iostream>
#include "matingpool.h"

using namespace std;

int main() {
    Board board(6, 7);
    // (int rows, int cols, int c, int pieces, int amountOfSurvivors, unsigned int populationSize, unsigned int games, float pc, float pm, float t, float mr);
    MatingPool mp(6, 7, 4, 21, 10, 50, 100, 0.6f, 0.05f, 0.3f, 1.0f);
    mp.evolvePopulation(30);
    // Genome g1 = mp.getPopulation().at(0);
    // Genome g2 = mp.getPopulation().at(1);
    // Genome g3 = mp.crossover(g1, g2);
    return 0;
}

#include <iostream>
#include "matingpool.h"

using namespace std;

int main() {
    Board board(6, 7);
    MatingPool mp(10, 10, 4, 0.5, 0.05, 0.75, 2.0, FitnessFunction::AVERAGE_OF_WINS);
    Genome g1 = mp.getPopulation().at(0);
    Genome g2 = mp.getPopulation().at(1);
    Genome g3 = mp.crossover(g1, g2);
    displayVector(g3.geneWeights);
	return 0;
}

#include <iostream>
#include "matingpool.h"

using namespace std;

int main() {
    // Board board(4, 4);
    // (int rows, int cols, int c, int pieces, int amountOfSurvivors, unsigned int populationSize, unsigned int games, float pc, float pm, float t, float mr);
   int rows = 6;
    int cols = 7;
    int c = 4;
    int pieces = 21;
    int amountOfSurvivors = 5;
    unsigned int populationSize = 30;
    unsigned int games = 50;
    float pc = 0.6f;
    float pm = 0.05f;
    float t = 0.3f;
    float mr = 1.0f;
    float pRandomMating = 0.005f;


    #ifdef FITNESS
        cerr << "Generacion" << ";" << "Fitness" << endl;
	#endif

    

    MatingPool mp(rows, cols, c, pieces, amountOfSurvivors, populationSize, games, pc, pm, t, mr, pRandomMating);
    mp.evolvePopulation	(30, 5);
    // Genome g1 = mp.getPopulation().at(0);
    // Genome g2 = mp.getPopulation().at(1);
    // Genome g3 = mp.crossover(g1, g2);
    return 0;
}

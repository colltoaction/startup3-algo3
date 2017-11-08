#include <iostream>
#include "matingpool.h"

using namespace std;

int main() {
    // Board board(4, 4);
    // (int rows, int cols, int c, int pieces, int amountOfSurvivors, unsigned int populationSize, unsigned int games,
    // float pc, float pm, float t, float mr, float pRandomMating, int fitnessFunction, float alpha, int extinctionRate);
    int rows = 6;
    int cols = 7;
    int c = 4;
    int pieces = 21;
    int amountOfSurvivors = 20;
    unsigned int populationSize = 100;
    unsigned int games = 300;
    float pc = 0.7f;
    float pm = 0.05f;
    float t = 0.3f;
    float mr = 1.0f;
    float pRandomMating = 0.005f;
    int fitnessFunction = 1;
    float alpha = 0.002f;
    float pNewcomer = 0.0f;
    int extinctionRate = 100;

    #ifdef FITNESS
        cerr << "Generacion" << ";" << "Fitness" << endl;
	#endif



    MatingPool mp(rows, cols, c, pieces, amountOfSurvivors, populationSize, games, pc, pm, t, mr, pRandomMating, fitnessFunction, alpha, pNewcomer, extinctionRate);
    mp.evolvePopulation	(400, 5);
    // Genome g1 = mp.getPopulation().at(0);
    // Genome g2 = mp.getPopulation().at(1);
    // Genome g3 = mp.crossover(g1, g2);
    return 0;
}

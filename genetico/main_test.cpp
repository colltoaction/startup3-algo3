#include <iostream>
#include "matingpool.h"

using namespace std;

int main() {
    // Board board(4, 4);
    // (int rows, int cols, int c, int pieces, int amountOfSurvivors, unsigned int populationSize, unsigned int games,
    // float pc, float pm, float t, float mr, float pRandomMating, int fitnessFunction, float alpha);
    int rows = 6;
    int cols = 7;
    int c = 4;
    int pieces = 21;
    int amountOfSurvivors = 5;
    unsigned int populationSize = 50;
    unsigned int games = 50;
    float pc = 0.7f;
    float pm = 0.05f;
    float t = 0.3f;
    float mr = 1.0f;
    float pRandomMating = 0.005f;


    #ifdef FITNESSONE
    int fitnessFunction = 1;
    #endif
    #ifdef FITNESSTWO
    int fitnessFunction = 2;
    #endif

    float alpha = 0.1;
    float pNewcomer = 0.01f;

    #ifdef FITNESS
        cerr << "Generacion" << ";" << "Fitness" << endl;
	#endif



    MatingPool mp(rows, cols, c, pieces, amountOfSurvivors, populationSize, games, pc, pm, t, mr, pRandomMating, fitnessFunction, alpha, pNewcomer);
    mp.evolvePopulation	(5, 5);
    // Genome g1 = mp.getPopulation().at(0);
    // Genome g2 = mp.getPopulation().at(1);
    // Genome g3 = mp.crossover(g1, g2);
    return 0;
}

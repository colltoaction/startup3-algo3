#include "genome.h"

enum class FitnessFunction {
    AVERAGE_OF_WINS,
    SCORE,
};

class MatingPool {
public:
    MatingPool(unsigned int p, int c, float pc, float pm, FitnessFunction f);
private:
    unsigned int populationSize;
    int c;
    unsigned int amountOfGamesToPlay;
    float pCrossover;
    float pMutate;
    vector<Genome> population;
    vector<float> fitnesses;
    FitnessFunction f;

    float fitness(Genome g);
    void evolvePopulation(unsigned int generations);
    void newGeneration();
    vector<Genome> fittestK(unsigned int k);
};

MatingPool::MatingPool(unsigned int p, int c, float pc, float pm, FitnessFunction f) :
    populationSize(p),
    pCrossover(pc),
    pMutate(pm),
    population(populationSize, Genome(c)),
    fitnesses(populationSize),
    f(f) {}

void MatingPool::newGeneration() {
    float totalFitness = 0;
    for (unsigned int i = 0; i < populationSize; ++i) {
        fitnesses[i] = fitness( population.at(i) );
        totalFitness += fitnesses.at(i);
    }
}

void MatingPool::evolvePopulation(unsigned int generations) {
    for (unsigned int i = 0; i < generations; ++i) {
        newGeneration();
    }
}

float MatingPool::fitness(Genome g) {
    return 0;
}

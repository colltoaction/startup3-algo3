#include <vector>

using namespace std;

class MatingPool
{
public:
private:
    unsigned int populationSize;
    unsigned int genomeSize;
    float pCrossover;
    float pMutate;
    vector<vector<float>> population;

    float fitness(vector<float> v);
    void evolvePopulation(unsigned int generations);
    void newGeneration();
    void fittestK(unsigned int k);
};

MatingPool::MatingPool(unsigned int p, unsigned int g, float pc, float pm) :
    populationSize(p),
    genomeSize(g),
    pCrossover(pc),
    pMutate(p) {
    for (unsigned int i = 0; i < populationSize; ++i) {
        vector<float> individual;
        for (unsigned int j = 0; j < genomeSize; ++j) {
            individual.push_back(0);
        }
    }
}


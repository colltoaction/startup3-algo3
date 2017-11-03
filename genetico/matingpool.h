#include "genome.h"

enum class FitnessFunction {
    AVERAGE_OF_WINS,
    SCORE,
};

class MatingPool {
public:
    MatingPool(unsigned int p, unsigned int games, int c, float pc, float pm,
         float t, float mr, FitnessFunction f);
    vector<Genome> getPopulation();
    Genome crossover(Genome& g1, Genome& g2);
private:
    unsigned int populationSize;
    unsigned int amountOfGamesToPlay;
    int c;
    float pCrossover;
    float pMutate;
    float crossoverThreshold; // probabilidad de que haya un "corte" durante el crossover
    float mutationRadius;
    vector<Genome> population;
    vector<float> fitnesses;
    FitnessFunction f;

    float fitness(Genome g);
    void evolvePopulation(unsigned int generations);
    void newGeneration();
    vector<Genome> fittestK(unsigned int k);
};

MatingPool::MatingPool(unsigned int p, unsigned int games, int c, float pc, float pm,
    float t, float mr, FitnessFunction f) :
    populationSize(p),
    amountOfGamesToPlay(games),
    c(c),
    pCrossover(pc),
    pMutate(pm),
    crossoverThreshold(t),
    mutationRadius(mr),
    fitnesses(populationSize),
    f(f) {
        assert(crossoverThreshold >= 0 && crossoverThreshold <= 1 &&
            pMutate >= 0 && pMutate <= 1 && mutationRadius >= 0);
        for (unsigned int i = 0; i < populationSize; ++i) {
            population.push_back(Genome(c));
        }
    }

vector<Genome> MatingPool::getPopulation() {
    return population;
}

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

Genome MatingPool::crossover(Genome& g1, Genome& g2) {
    vector<float> newWeights;
    Genome& activeGenome = g1;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine cutGenerator(seed);
    default_random_engine mutationDecisionGenerator(seed);
    default_random_engine mutationGenerator(seed);
    uniform_real_distribution<float> cutDistribution(0.0, 1.0);
    uniform_real_distribution<float> mutationDecisionDistribution(0.0, 1.0);
    uniform_real_distribution<float> mutationDistribution(-mutationRadius, mutationRadius);

    float startP = cutDistribution(cutGenerator);
    if (startP >= 0.5) {
        cerr << "Switching starting genome to g2." << endl;
        activeGenome = g2;
    }

    for (int i = 0; i < g1.geneWeights.size(); ++i) {
        float pCut = cutDistribution(cutGenerator);
        if (pCut > crossoverThreshold) {
            // Si la variable aleatoria supera el umbral, se hace un corte en los cromosomas.
            // Cuando se hace un corte, se empiezan a copiar alelos del otro cromosoma.
            cerr << "Making cut at locus " << i << "." << endl;
            if (&activeGenome == &g1) {
                cerr << "Switching active genome to g2." << endl;
                activeGenome = g2;
            } else if (&activeGenome == &g2) {
                cerr << "Switching active genome to g1." << endl;
                activeGenome = g1;
            }
        }
        float w = activeGenome.geneWeights.at(i);
        float p = mutationDecisionDistribution(mutationDecisionGenerator);
        if (p <= pMutate) {
            cerr << "The gene on locus " << i << " has mutated." << endl;
            float mutation = mutationDistribution(mutationGenerator);
            w += mutation;
        }
        newWeights.push_back(w);
    }

    Genome result(c, newWeights);
    return result;
}

#include "genome.h"
#include <random>

class PlayerRandom : public Player {
private:
std::random_device rd;
    std::mt19937 generator = std::mt19937(rd());

public:
    int nextMove(Game& game) {
        auto moves = game.board().possibleMoves();
        std::uniform_int_distribution<int> do_move(0, moves.size() - 1);
        int m = do_move(generator);
        return moves.at(m);

    }
};


class PlayerGenetic : public Player {
private:
    Genome g;
public:
    PlayerGenetic(Genome g) : g(g) {}
    int nextMove(Game& game) {
        int max = 0;
        int bestCol = 0;

        // TODO only available columns
        for (auto i : game.board().possibleMoves()) {
            int possibleValue = g.activate(game.board(), i);
            if (max < possibleValue) {
                bestCol = i;
                max = possibleValue;
            }
        }

        return bestCol;
    }
};


class MatingPool {
public:
    MatingPool(int rows, int cols, int c, int pieces, int amountOfSurvivors, unsigned int populationSize, unsigned int games, float pc, float pm, float t, float mr);
    vector<Genome> getPopulation();
    Genome crossover(Genome& g1, Genome& g2);
    Genome mitosis(Genome& g1);
    void newGeneration();
private:
    int rows;
    int cols;
    int c;
    int pieces;
    int amountOfSurvivors;
    unsigned int populationSize;
    unsigned int amountOfGamesToPlay;
    float pCrossover;
    float pMutate;
    float crossoverThreshold; // probabilidad de que haya un "corte" durante el crossover
    float mutationRadius;
    vector<Genome> population;
    vector< pair<float, bool> > fitnesses;

    float calculateFitness(Genome g);
    void evolvePopulation(unsigned int generations);
    vector<unsigned int> survivorIndices();
};

MatingPool::MatingPool(int rows, int cols, int c, int pieces, int amountOfSurvivors, unsigned int populationSize, unsigned int games, float pc, float pm, float t, float mr) :
    rows(rows),
    cols(cols),
    c(c),
    pieces(pieces),
    amountOfSurvivors(amountOfSurvivors),
    populationSize(populationSize),
    amountOfGamesToPlay(games),
    pCrossover(pc),
    pMutate(pm),
    crossoverThreshold(t),
    mutationRadius(mr) {
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
    vector<unsigned int> fittest = survivorIndices();

    vector<Genome> newPopulation;

    // Copiamos los k mejores
    for (unsigned int j = 0; j < amountOfSurvivors; ++j) {
        newPopulation.push_back(population.at(fittest.at(j)));
    }


    for (unsigned int j = 0; j < populationSize - amountOfSurvivors; ++j) {
        Genome& firstGenome = population.at(fittest.at(j));
        // Module for not going out of range
        Genome& secondGenome = population.at(fittest.at((j + 1) % (populationSize - amountOfSurvivors)));

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        default_random_engine crossOrMitosisGenerator(seed);
        uniform_real_distribution<float> crossOrMitosisDistribution(0.0, 1.0);
        float evolP = crossOrMitosisDistribution(crossOrMitosisGenerator);

        // Lets do crossover
        if (evolP <= pCrossover) {
            // Chequear si estas referencias funcionan bien
            newPopulation.push_back(crossover(firstGenome, secondGenome));
        } else {
            // Lets do mitosis
            newPopulation.push_back(mitosis(firstGenome));
        }
    }

    population = newPopulation;
}

void MatingPool::evolvePopulation(unsigned int generations) {
    for (unsigned int i = 0; i < generations; ++i) {
        newGeneration();
    }
}

float MatingPool::calculateFitness(Genome g) {

    PlayerGenetic player(g);

    PlayerRandom random;

    Game game = Game(rows, cols, c, pieces);

    int wins = 0;

    for (unsigned int i = 0; i < amountOfGamesToPlay; ++i) {

        int result = game.playMatch(player, random);

        //Chequear que result siempre sea 1 o 0 nunca -1!!!!
        wins += result;
    }

    return wins / amountOfGamesToPlay;
}

Genome MatingPool::crossover(Genome& g1, Genome& g2) {
    cerr << "Parent 1: " << endl;
    displayVector(g1.geneWeights);
    cerr << "Parent 2: " << endl;
    displayVector(g2.geneWeights);

    vector<float> newWeights;
    Genome* activeGenome = &g1;

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
        activeGenome = &g2;
    }

    for (int i = 0; i < g1.geneWeights.size(); ++i) {
        float pCut = cutDistribution(cutGenerator);
        if (pCut > crossoverThreshold) {
            // Si la variable aleatoria supera el umbral, se hace un corte en los cromosomas.
            // Cuando se hace un corte, se empiezan a copiar alelos del otro cromosoma.
            if (activeGenome == &g1) {
                cerr << "Switching active genome to g2 at locus " << i << "." << endl;
                activeGenome = &g2;
            } else if (activeGenome == &g2) {
                cerr << "Switching active genome to g1 at locus " << i << "." << endl;
                activeGenome = &g1;
            }
        }
        float w = activeGenome->geneWeights.at(i);
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



Genome MatingPool::mitosis(Genome& g1) {
    return crossover(g1, g1);
}


vector<unsigned int> MatingPool::survivorIndices() {
    assert(fitnesses.empty());

    vector<unsigned int> result;
    float totalFitness = 0;

    for (unsigned int i = 0; i < populationSize; ++i) {
        // pair means fitness and has_been_used
        fitnesses.push_back(make_pair(calculateFitness( population.at(i) ), false));
        totalFitness += fitnesses.at(i).first;
    }

    // For k iterations, lets search the fittest
    for (unsigned int i = 0; i < amountOfSurvivors; ++i) {
        // Reinitialize the max and the bestGenome
        float max = 0;
        unsigned int bestGenome = 0;

        // For every genome
        for (int j = 0; j < populationSize; ++j) {
            // If its better than the max and hasnt been used
            if (max < fitnesses.at(j).first && !fitnesses.at(j).second) {
                // Update the max and the bestGenome Position
                max = fitnesses.at(j).first;
                bestGenome = j;
            }

        }

        // We have the fittest in bestGenome
        result.push_back(bestGenome);
        // Mark it as used
        fitnesses.at(bestGenome).second = true;
    }

    return result;
}


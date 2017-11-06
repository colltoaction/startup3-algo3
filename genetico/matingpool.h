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

        // cerr << "PossibleMoves: " << endl;
        // for (int i = 0; i < moves.size(); ++i)
        // {
        //     cerr << moves.at(i) <<" ";
        // }

        // cerr << endl << "Random elige: " << moves.at(m) << endl;

        return moves.at(m);

    }
};


class PlayerGenetic : public Player {
private:
    Genome g;
public:
    PlayerGenetic(Genome g) : g(g) {}
    int nextMove(Game& game) {

        auto moves = game.board().possibleMoves();
        auto bestCol = max_element(moves.begin(), moves.end(),
            [this, game](const int& m1, const int& m2) {
                return g.activate(game.board(), m1) < g.activate(game.board(), m2);
            }
        );
        assert(bestCol != moves.end()); // encontró alguno

        // cerr << "PossibleMoves: " << endl;
        // for (auto i : game.board().possibleMoves())
        // {
        //     cerr << i <<" ";
        // }

        // cerr << endl << "Genetic elige: " << *bestCol << endl;

        return *bestCol;
    }
};


class MatingPool {
public:
    MatingPool(int rows, int cols, int c, int pieces, int amountOfSurvivors, unsigned int populationSize, unsigned int games, float pc, float pm, float t, float mr, float pRandomMating);
    vector<Genome> getPopulation();
    Genome crossover(Genome& g1, Genome& g2);
    Genome mitosis(Genome& g1);
    void evolvePopulation(unsigned int generations, int spacing);
private:
    int rows;
    int cols;
    int c;
    int pieces;
    int amountOfSurvivors;
    unsigned int populationSize;
    unsigned int numberOfGamesToPlay;
    float pCrossover;
    float pMutate;
    float crossoverThreshold; // probabilidad de que haya un "corte" durante el crossover
    float mutationRadius;
    float pRandomMating;
    vector<Genome> population;
    vector< Player* > lastChampions;
    float averageFitness;

    void newGeneration();
    float calculateFitness(Genome g);
    vector<unsigned int> survivorIndices();
};

MatingPool::MatingPool(int rows, int cols, int c, int pieces, int amountOfSurvivors, unsigned int populationSize, unsigned int games, float pc, float pm, float t, float mr, float pRandomMating) :
    rows(rows),
    cols(cols),
    c(c),
    pieces(pieces),
    amountOfSurvivors(amountOfSurvivors),
    populationSize(populationSize),
    numberOfGamesToPlay(games),
    pCrossover(pc),
    pMutate(pm),
    crossoverThreshold(t),
    mutationRadius(mr),
    pRandomMating(pRandomMating) {
        assert(crossoverThreshold >= 0 && crossoverThreshold <= 1 &&
               pMutate >= 0 && pMutate <= 1 && mutationRadius >= 0);
        for (unsigned int i = 0; i < populationSize; ++i) {
            population.push_back(Genome(c));
        }
        for (int i = 0; i < amountOfSurvivors / 4 + 1; ++i) {
            // Genome genome (4, { -0.211009, 2.2091, 1.19177, 6.09952, 2.55178, -2.45106, 0.561537, 0.307363, 0.7774, 0.506736, 0.750724, 0.914839, -0.234364, 0.0909093, -0.269583, -0.37189, -0.538976, 0.0200729, 0.0, 0.0, 0.0, 0.192907, 1.02838, 0.762831 });
            // Player* p = new PlayerGenetic(genome);
            Player* p = new PlayerRandom();
            lastChampions.push_back(p);
        }
    }

vector<Genome> MatingPool::getPopulation() {
    return population;
}

void MatingPool::newGeneration() {
    vector<unsigned int> fittest = survivorIndices();
    assert(population.size() == populationSize);
    int fitSize = fittest.size();
    assert(fitSize == amountOfSurvivors);

    vector<Genome> newPopulation;

    // Copiamos los k mejores
    for (int j = 0; j < amountOfSurvivors; ++j) {
        newPopulation.push_back(population.at(fittest.at(j)));
    }

    int dead = populationSize - amountOfSurvivors;
    for (int j = 0; j < dead; ++j) {
        Genome& firstGenome = population.at(fittest.at(j % amountOfSurvivors));
        // Module for not going out of range
        Genome& secondGenome = population.at(fittest.at((j + 1) % amountOfSurvivors));

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        default_random_engine crossOrMitosisGenerator(seed);
        uniform_real_distribution<float> crossOrMitosisDistribution(0.0, 1.0);
        default_random_engine randomMatingGenerator(seed);
        uniform_real_distribution<float> randomMatingDistribution(seed);
        float evolP = crossOrMitosisDistribution(crossOrMitosisGenerator);
        float matingP = randomMatingDistribution(randomMatingGenerator);

        if (matingP <= pRandomMating) {
            // Reproducción con un individuo elegido al azar
            unsigned int randomToSelect = rand() % populationSize;
            newPopulation.push_back(crossover(firstGenome, population.at(randomToSelect)));
        } else {
            if (evolP <= pCrossover) {
                // Reproducción entre dos de los individuos más aptos
                newPopulation.push_back(crossover(firstGenome, secondGenome));
            } else {
                // Replicación de uno de los individuos más aptos con posibilidad de mutación
                newPopulation.push_back(mitosis(firstGenome));
            }
        }
    }
    for (int i = 0; i < amountOfSurvivors / 4 + 1; ++i) {
        *(lastChampions.at(i)) = PlayerGenetic(population.at( fittest.at(i)) );
    }

    population = newPopulation;
}

void MatingPool::evolvePopulation(unsigned int generations, int spacing) {
    for (unsigned int i = 0; i < generations; ++i) {
        cerr << "Gen " << i << "------------------------------------" << endl;
        newGeneration();
        if (i%spacing == 0) {
            cerr << "Average fitness: " << averageFitness << endl;
        }
    }
    displayVector(population.at(0).geneWeights); // el individuo de mayor fitness
}

float MatingPool::calculateFitness(Genome g) {
    PlayerGenetic player(g);
    // Genome genome(4, { -0.211009, 2.2091, 1.19177, 6.09952, 2.55178, -2.45106, 0.561537, 0.307363, 0.7774, 0.506736, 0.750724, 0.914839, -0.234364, 0.0909093, -0.269583, -0.37189, -0.538976, 0.0200729, 0.192907, 1.02838, 0.762831 });
    // PlayerGenetic sensei(genome);

    int wins = 0;
    // int globalNumberOfMoves = 0;
    for (unsigned int i = 0; i < numberOfGamesToPlay; ++i) {
        Game game(rows, cols, c, pieces);
        int champion = rand() % lastChampions.size();
        pair<int,int > result = game.playMatch(player, *(lastChampions.at(champion)) );

        //Chequear que result siempre sea 1 o 0 nunca -1!!!!
        wins += result.first;
        // globalNumberOfMoves += result.second * result.first;
    }


    // int totalPossibleMoves = rows*cols*numberOfGamesToPlay;
    return (float) wins / numberOfGamesToPlay;
    // return 0.9f * ((float)wins / numberOfGamesToPlay) + 0.1f * (1 - (float)globalNumberOfMoves/totalPossibleMoves);
}

Genome MatingPool::crossover(Genome& g1, Genome& g2) {
    // cerr << "Parent 1: " << endl;
    // displayVector(g1.geneWeights);
    // cerr << "Parent 2: " << endl;
    // displayVector(g2.geneWeights);

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
        // cerr << "Switching starting genome to g2." << endl;
        activeGenome = &g2;
    }

    for (unsigned int i = 0; i < g1.geneWeights.size(); ++i) {
        float pCut = cutDistribution(cutGenerator);
        if (pCut > crossoverThreshold) {
            // Si la variable aleatoria supera el umbral, se hace un corte en los cromosomas.
            // Cuando se hace un corte, se empiezan a copiar alelos del otro cromosoma.
            if (activeGenome == &g1) {
                // cerr << "Switching active genome to g2 at locus " << i << "." << endl;
                activeGenome = &g2;
            } else if (activeGenome == &g2) {
                // cerr << "Switching active genome to g1 at locus " << i << "." << endl;
                activeGenome = &g1;
            }
        }
        float w = activeGenome->geneWeights.at(i);
        float p = mutationDecisionDistribution(mutationDecisionGenerator);
        if (p <= pMutate) {
            // cerr << "The gene on locus " << i << " has mutated." << endl;
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
    vector< pair<float, bool> > fitnesses(populationSize);
    averageFitness = 0;

    vector<unsigned int> result;

    for (unsigned int i = 0; i < populationSize; ++i) {
        // pair means fitness and has_been_used
        fitnesses.at(i) = make_pair(calculateFitness( population.at(i) ), false);
        averageFitness += fitnesses.at(i).first;
        // cerr << "Fitness: " << fitnesses.at(i).first << endl;
    }

    for (int i = 0; i < amountOfSurvivors; ++i) {
        float max = 0;
        unsigned int bestGenome = 0;

        // For every genome
        for (unsigned int j = 0; j < populationSize; ++j) {
            // If its better than the max and hasnt been used
            if (max < fitnesses.at(j).first && !fitnesses.at(j).second) {
                max = fitnesses.at(j).first;
                bestGenome = j;
            }

        }

        // We have the fittest in bestGenome
        result.push_back(bestGenome);
        // Mark it as used
        fitnesses.at(bestGenome).second = true;
    }

    averageFitness = averageFitness / populationSize;

    return result;
}

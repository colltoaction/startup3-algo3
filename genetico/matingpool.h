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
    MatingPool(int rows, int cols, int c, int pieces, int amountOfSurvivors, unsigned int populationSize, unsigned int games, float pc, float pm, float t, float mr);
    vector<Genome> getPopulation();
    Genome crossover(Genome& g1, Genome& g2);
    Genome mitosis(Genome& g1);
    void evolvePopulation(unsigned int generations);
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
    Player* lastChampion;

    void newGeneration();
    float calculateFitness(Genome g);
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
    lastChampion = new PlayerRandom();
}

vector<Genome> MatingPool::getPopulation() {
    return population;
}

void MatingPool::newGeneration() {
    vector<unsigned int> fittest = survivorIndices();
    assert(population.size() == populationSize);
    int fitSize = fittest.size();
    assert( fitSize == amountOfSurvivors);

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
    *lastChampion = PlayerGenetic(population.at( fittest.at(0)) );

    population = newPopulation;
}

void MatingPool::evolvePopulation(unsigned int generations) {
    for (unsigned int i = 0; i < generations; ++i) {
        cout<<"Gen "<< i <<"------------------------------------"<<endl;
        newGeneration();
    }
    displayVector(population.at(0).geneWeights);
}

float MatingPool::calculateFitness(Genome g) {
    PlayerGenetic player(g);
    Genome genome(4, { -0.314897, 1.09549, 0.0594074, 2.79274, 0.703102, -0.139028, -0.517212, 0.671523, 0.667185, 2.54945, -0.237511, 0.950038, -0.66933, 0.900584, -0.434702, -0.355428, -0.790875, 0.590007, -0.53404, 0.442229, 0.345638 });
    PlayerGenetic sensei(genome);

    int wins = 0;
    int globalNumberOfMoves = 0;
    for (unsigned int i = 0; i < amountOfGamesToPlay; ++i) {
        Game game(rows, cols, c, pieces);
        pair<int,int > result = game.playMatch(player, (*lastChampion) );

        //Chequear que result siempre sea 1 o 0 nunca -1!!!!
        wins += result.first;
        globalNumberOfMoves += result.second;
    }


    int totalMovesPossible = rows*cols*amountOfGamesToPlay;
    return 0.9f * ((float)wins / amountOfGamesToPlay) +
           0.1f * (1 - (float)globalNumberOfMoves/totalMovesPossible);
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

    vector<unsigned int> result;
    // float totalFitness = 0;

    for (unsigned int i = 0; i < populationSize; ++i) {
        // pair means fitness and has_been_used
        fitnesses.at(i) = make_pair(calculateFitness( population.at(i) ), false);
        cerr << "Fitness: " << fitnesses.at(i).first << endl;
    }

    // For k iterations, lets search the fittest
    for (int i = 0; i < amountOfSurvivors; ++i) {
        // Reinitialize the max and the bestGenome
        float max = 0;
        unsigned int bestGenome = 0;

        // For every genome
        for (unsigned int j = 0; j < populationSize; ++j) {
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


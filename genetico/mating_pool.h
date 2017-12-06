#ifndef STARTUP3_ALGO3_MATING_POOL_H
#define STARTUP3_ALGO3_MATING_POOL_H

#include "../common/player.h"
#include "../common/player_minimax_n.h"
#include "genome.h"
#include "player_genetic.h"
#include "../common/player_random.h"
#include <limits>
#include <sstream>
#include <fstream>

class MatingPool {
public:
    MatingPool(int rows, int cols, int c, int pieces, int amountOfSurvivors, unsigned int populationSize, unsigned int games, float pc, float pm, float t, float mr, float pRandomMating, int fitnessFunction, float alpha, unsigned int extinctionRate, ofstream& fitnessOutputFile,ofstream& averageMovesOutputFile);
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
    unsigned int currentGeneration;
    int fitnessFunction;
    float alpha;
    unsigned int extinctionRate;
    ofstream& fitnessOutputFile;
    ofstream& averageMovesOutputFile;

    void newGeneration();
    float calculateFitness(Genome g);
    vector<unsigned int> survivorIndices();
};

MatingPool::MatingPool(int rows, int cols, int c, int pieces, int amountOfSurvivors, unsigned int populationSize, unsigned int games, float pc, float pm, float t, float mr, float pRandomMating, int fitnessFunction, float alpha, unsigned int extinctionRate, ofstream& fitnessOutputFile, ofstream& averageMovesOutputFile) :
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
    pRandomMating(pRandomMating),
    currentGeneration(0),
    fitnessFunction(fitnessFunction),
    alpha(alpha),
    extinctionRate(extinctionRate),
    fitnessOutputFile(fitnessOutputFile),
    averageMovesOutputFile(averageMovesOutputFile) {
        assert(crossoverThreshold >= 0 && crossoverThreshold <= 1 &&
               pMutate >= 0 && pMutate <= 1 && mutationRadius >= 0);
       for (unsigned int i = 0; i < populationSize; ++i) {
           population.push_back(Genome(c));
       }
       for (int i = 0; i < amountOfSurvivors; ++i) {
            // Genome genome (4, { -0.211009, 2.2091, 1.19177, 6.09952, 2.55178, -2.45106, 0.561537, 0.307363, 0.7774, 0.506736, 0.750724, 0.914839, -0.234364, 0.0909093, -0.269583, -0.37189, -0.538976, 0.0200729, 0.0, 0.0, 0.0, 0.192907, 1.02838, 0.762831 });
            // Player* p = new PlayerGenetic(genome);
            Player* p = new PlayerRandom;
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

    vector<Genome> newPopulation;

    for (unsigned int j = 0; j < populationSize; ++j) {
		unsigned int firstGenomeIndex = rand() % amountOfSurvivors;
		unsigned int secondGenomeIndex = rand() % amountOfSurvivors;
		while (firstGenomeIndex == secondGenomeIndex) {
			secondGenomeIndex = rand() % amountOfSurvivors;
		}
        Genome& firstGenome = population.at(fittest.at(firstGenomeIndex));
        Genome& secondGenome = population.at(fittest.at(secondGenomeIndex));

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

        default_random_engine crossOrMitosisGenerator(seed);
        uniform_real_distribution<float> crossOrMitosisDistribution(0.0, 1.0);
        default_random_engine randomMatingGenerator(seed);
        uniform_real_distribution<float> randomMatingDistribution(0.0, 1.0);

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
    if (currentGeneration % extinctionRate == extinctionRate - 1) {
        // queremos entrenar a los primeros campeones contra algo mejor que random
        for (int i = 0; i < amountOfSurvivors; ++i) {
            *(lastChampions.at(i)) = PlayerGenetic(population.at( fittest.at(i)) );
        }
    }


    std::stringstream ss;
    ss << "../experimentacion/sensei_" << string(getenv("RIVAL")) << "_population" << populationSize << ".txt";
    string fileName = ss.str();

    ofstream senseiOutputFile;
    senseiOutputFile.open(fileName);
    displayVector(population.at(fittest.at(0)).geneWeights, senseiOutputFile); // el individuo de mayor fitness
    senseiOutputFile.close();

    population = newPopulation;

    population = newPopulation;
}

void MatingPool::evolvePopulation(unsigned int generations, int spacing) {
    for (; currentGeneration < generations; ++currentGeneration) {
        if(getenv("SHOWPROGRESS") != NULL){
            cout << "Generacion: " << currentGeneration << " --------------------------" << endl;
        }
        if (currentGeneration % extinctionRate == 0 && currentGeneration > 0) {
            // vector<unsigned int> fittest = survivorIndices();
            // cerr << "MASS EXTINCTION" << endl;
            for (unsigned int i = 0; i < populationSize; ++i) {
                population.at(i) = Genome(c);
            }
        } else {
            newGeneration();
        }

    }

    if(getenv("GENECORRELATION") != NULL){
        std::stringstream ssGeneWeights;
        cerr << "asd";
        ssGeneWeights << "../experimentacion/geneWeights_" << string(getenv("RIVAL")) << "_population" << populationSize << ".txt";
        string geneWeightsFileName = ssGeneWeights.str();
        ofstream geneWeightsOutputFile;
        geneWeightsOutputFile.open(geneWeightsFileName);

        for (auto genome : population) {
            for (int i = 0; i < genome.geneWeights.size(); ++i) {
               geneWeightsOutputFile << genome.geneWeights.at(i);
               if(i != genome.geneWeights.size() - 1){
                geneWeightsOutputFile << ";";
               }
            }
            geneWeightsOutputFile << endl;
        }
    }



}

float MatingPool::calculateFitness(Genome g) {
    PlayerGenetic player(g);
    int wins = 0;
    int numberOfMovesToWin = 0;
    int numberOfMovesToLose = 0;
    PlayerRandom randomRival;
    PlayerMinimax_n minimaxRival(2);

    for (unsigned int i = 0; i < numberOfGamesToPlay; ++i) {
        Game game(rows, cols, c, pieces);
        int champion = rand() % lastChampions.size();
        pair<int,int > result;
        if(getenv("RIVAL") != NULL && string(getenv("RIVAL")) == "ANCESTORS"){
            result = game.playMatch(player, *(lastChampions.at(champion)) );
        } else if (getenv("RIVAL") != NULL && string(getenv("RIVAL")) == "RANDOM"){
            result = game.playMatch(player, randomRival);
        } else if(getenv("RIVAL") != NULL && string(getenv("RIVAL")) == "MINIMAX"){
            result = game.playMatch(player, minimaxRival);
        } else if(getenv("RIVAL") != NULL && string(getenv("RIVAL")) == "MIXED"){
            if (i < numberOfGamesToPlay / 3){
                result = game.playMatch(player, *(lastChampions.at(champion)) );
            } else if (i < 2 * numberOfGamesToPlay / 3) {
                result = game.playMatch(player, randomRival);
            } else {
                result = game.playMatch(player, minimaxRival);
            }
        }
        wins += result.first;
        if(result.first){
            numberOfMovesToWin += result.second;
        } else {
            numberOfMovesToLose += result.second;
        }
    }



    int averageMovesToWin = numeric_limits<double>::infinity();
    if (wins > 0) {
        averageMovesToWin = numberOfMovesToWin / wins;
    }
    int averageMovesToLose = numberOfMovesToLose;
    if (numberOfGamesToPlay - wins != 0) {
        averageMovesToLose = numberOfMovesToLose / (numberOfGamesToPlay - wins);
    }

    averageMovesOutputFile << currentGeneration << ";" << averageMovesToWin <<';'<< averageMovesToLose << endl;

    if (fitnessFunction == 1) {
        return (float) wins / numberOfGamesToPlay;
    } else {
        return ((float) wins * 900 / numberOfGamesToPlay) + alpha * min(100, averageMovesToLose / averageMovesToWin) ;
    }
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

    



    for (unsigned int i = 0; i < populationSize; ++i) {
        // pair means fitness and has_been_used
        fitnesses.at(i) = make_pair(calculateFitness( population.at(i) ), false);
        fitnessOutputFile << currentGeneration << ";" << fitnesses.at(i).first << endl;
    }

    // Selection sort sin la parte de sort. En cada iteración del ciclo externo,
    // buscamos el elemento de máximo fitness que aún no haya sido elegido
    // y lo agregamos al resultado. Al final, deberíamos tener la cantidad de
    // sobrevivientes especificada y deberían ser los de mayor fitness,
    // ordenados de más adaptado a menos adaptado.

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

    // cerr << "Survivors ready." << endl;

    return result;
}

#endif //STARTUP3_ALGO3_MATING_POOL_H

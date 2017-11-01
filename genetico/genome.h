#include <random>
#include "board.h"

using namespace Players;

class Gene {
    public:
        // Propiedad que debe cumplir el tablero para que se sume el peso del locus asociado al gen.
        int property(Board b, int row, int col);
};

class ConnectKGene : Gene {
    public:
        ConnectKGene(int k);
    private:
        int k;
};

ConnectKGene::ConnectKGene(int k) : k(k) {}

int ConnectKGene::property(Board b, int row, int col) {
    assert (col >= 0 && col < b.columns());

    // El gen se activa si la ficha introducida en col forma un K en línea.
    return b.positionIsInLine(row, col, k, US);
}

class BlockKGene : Gene {
    public:
        BlockKGene(int k);
    private:
        int k;
};

BlockKGene::BlockKGene(int k) : k(k) {}

int BlockKGene::property(Board b, int row, int col) {
    assert (col >= 0 && col < b.columns());

    // El gen se activa si la ficha introducida en col bloquea un K en línea del oponente.
    return b.positionIsInLine(row, col, k, THEM);
}

class KFreeGene : Gene {
    public:
        KFreeGene(int k);
}

KFreeGene::KFreeGene(int k) : k(k) {}

int KFreeGene::property(Board b, int row, int col) {
    assert (col >= 0 && col < b.columns());

    // El gen se activa si la ficha introducida en col tiene K-1 posiciones libres en línea alrededor.
    return b.positionIsInLine(row, col, k, NONE);
}

class AmountOfLinesOfLengthKGene : Gene {
    public:
        AmountOfLinesOfLengthKGene(int k);
    private:
        int k;
};

AmountOfLinesOfLengthKGene::AmountOfLinesOfLengthKGene(int k) : k(k) {}

AmountOfLinesOfLengthKGene::property(Board b, int row, int col) {
    return b.amountOfLinesOfLengthK(row, col, k);
}

class AmountOfNeighboursGene : Gene {
    public:
        AmountOfNeighboursGene(Players player);
    private:
        Players player;
};

AmountOfNeighboursGene::AmountOfNeighboursGene(Players player) : player(player) {}

AmountOfNeighboursGene::property(Board b, int row, int col) {
    // Player indica de qué tipo son los vecinos que estamos devolviendo.
    return b.amountOfNeighbours(row, col, player);
}

class Genome {
    public:
        Genome(int c);
        Genome(int c, vector<float> geneWeights);
        float activate(Board b);
        vector<float> geneWeights;
    private:
        void initialiseGenes();
        vector<Gene> genes;
        int c;
};

void Genome::initialiseGenes() {
    // Inicializa un vector de genes donde a cada posición le corresponde un gen
    // con una función determinada (similar al genoma de un ser vivo real).
    for (int k = c; k >= 2; --k) {
        genes.push_back(ConnectKGene(k));
    }
    for (int k = c; k >= 2; --k) {
        genes.push_back(BlockKGene(k));
    }
    for (int k = c; k >= 2; --k) {
        genes.push_back(KFreeGene(k));
    }
    for (int k = c; k >= 2; --k) {
        genes.push_back(AmountOfLinesOfLengthKGene(k));
    }

    genes.push_back(AmountOfNeighboursGene(US));
    genes.push_back(AmountOfNeighboursGene(THEM));
    genes.push_back(AmountOfNeighboursGene(NONE));
}

Genome::Genome(int c) : c(c) {
    initialiseGenes();
    for (int i = 0; i < genes.size(); ++i) {
        // Le asigna a cada gen un peso con distribución U[-1, 1].
        geneWeights.push_back(rand(-1, 1));
    }
}

Genome::Genome(int c, vector<float> geneWeights) {
    initialiseGenes();
    geneWeights = geneWeights;
}

float Genome::activate(Board b) {
    // Al gen de genes[i] le corresponde el peso de geneWeights[i];
    // de esta forma, el producto interno entre genes y geneWeights
    // calcula el puntaje del tablero dado por ubicar una ficha
    // en una posición determinada.
}

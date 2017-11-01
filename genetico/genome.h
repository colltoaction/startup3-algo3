#include <vector>
#include <random>
#include "board.h"

using namespace std;

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
    // ¡¡¡¡¡¡¡¡¡IMPORTANTE!!!!!!!!!!!! RIIIIIIZZZZOOOOOO
    // Arreglar positionIsInLine para que sirva con cualquier jugador que se le pase como parámetro.
    // Chequear los casos borde: cuando miramos una línea nuestra hay que sumar K y cuando
    // miramos una del oponente hay que sumar K-1 Y NO INCLUIR LA NUESTRA.
    assert (col >= 0 && col < b.columns());

    // El gen se activa si la ficha introducida en col bloquea un K en línea del oponente.
    return b.positionIsInLine(row, col, k, THEM);
}

class AmountOfLinesOfLengthKGene : Gene {
    public:
        AmountOfLinesOfLengthKGene(int k);
    private:
        int k;
};

AmountOfLinesOfLengthKGene::AmountOfLinesOfLengthKGene(int k) : k(k) {}

AmountOfLinesOfLengthKGene::property(Board b, int row, int col) {
    // HACER!!!! IMPORTANTÍSIMO
    return b.amountOfLinesOfLength(row, col, k);
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
    // IMPORTANTE, HACER ESA FUNCIÓN
    return b.amountOfNeighbours(row, col, player);
}

class Genome {
    public:
        Genome(int c);
        Genome(int c, vector<float> geneWeights);
        void activate(Board b);
        vector<float> geneWeights;
    private:
        void initialiseGenes();
        vector<Gene> genes;
        int c;
};

void Genome::initialiseGenes() {
    for (int k = c; k >= 2; --k) {
        genes.push_back(ConnectKGene(k));
    }
    for (int k = c; k >= 2; --k) {
        genes.push_back(BlockKGene(k));
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
        geneWeights.push_back(rand(-1, 1));
    }
}

Genome::Genome(int c, vector<float> geneWeights) {
    initialiseGenes();
    geneWeights = geneWeights;
}

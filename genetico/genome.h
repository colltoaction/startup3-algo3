#include <random>
#include <chrono>
#include "board.h"

// FUNCIÓN DE DEBUG

void displayVector(vector<float> v) {
    cerr << "[ ";
    for (auto i : v) {
        cerr << i << " ";
    }
    cerr << "]" << endl;
}

class Gene {
    public:
        // Propiedad que debe cumplir el tablero para que se sume el peso del locus asociado al gen.
        virtual int boardProperty(Board b, int row, int col) = 0;
        int pepe = 0;
};

class ConnectKGene : public Gene {
    public:
        ConnectKGene(int k);
        int boardProperty(Board b, int row, int col);
    private:
        int k;
};

ConnectKGene::ConnectKGene(int k) : k(k) {}

int ConnectKGene::boardProperty(Board b, int row, int col) {
    assert (row < b.getRows() && col >= 0 && col < b.getColumns());

    // El gen se activa si la ficha introducida en col forma un K en línea.
    cerr << "ConnectKGene with k = " << k << "." << endl;
    return b.positionIsInLine(row, col, k, Players::US);
}

class BlockKGene : public Gene {
    public:
        BlockKGene(int k);
        int boardProperty(Board b, int row, int col);
    private:
        int k;
};

BlockKGene::BlockKGene(int k) : k(k) {}

int BlockKGene::boardProperty(Board b, int row, int col) {
    assert (row < b.getRows() && col >= 0 && col < b.getColumns());

    // El gen se activa si la ficha introducida en col bloquea un K en línea del oponente.
    cerr << "BlockKGene with k = " << k << "." << endl;
    return b.positionIsInLine(row, col, k, Players::THEM);
}

class KFreeGene : public Gene {
    public:
        KFreeGene(int k);
        int boardProperty(Board b, int row, int col);
    private:
        int k;
};

KFreeGene::KFreeGene(int k) : k(k) {}

int KFreeGene::boardProperty(Board b, int row, int col) {
    assert (row < b.getRows() && col >= 0 && col < b.getColumns());

    // El gen se activa si la ficha introducida en col tiene K-1 posiciones libres en línea alrededor.
    cerr << "KFreeGene with k = " << k << "." << endl;
    return b.positionIsInLine(row, col, k, Players::NONE);
}

class AmountOfLinesOfLengthKGene : public Gene {
    public:
        AmountOfLinesOfLengthKGene(int k);
        int boardProperty(Board b, int row, int col);
    private:
        int k;
};

AmountOfLinesOfLengthKGene::AmountOfLinesOfLengthKGene(int k) : k(k) {}

int AmountOfLinesOfLengthKGene::boardProperty(Board b, int row, int col) {
    assert (row < b.getRows() && col >= 0 && col < b.getColumns());

    // Cuenta la cantidad de líneas de largo k de las que forma parte la posición
    // al introducir la ficha.
    cerr << "AmountOfLinesOfLengthKGene with k = " << k << "." << endl;
    return b.amountOfLinesOfLengthK(row, col, k, Players::US);
}

class AmountOfBlockedLinesOfLengthKGene : public Gene {
    public:
        AmountOfBlockedLinesOfLengthKGene(int k);
        int boardProperty(Board b, int row, int col);
    private:
        int k;
};

AmountOfBlockedLinesOfLengthKGene::AmountOfBlockedLinesOfLengthKGene(int k) : k(k) {}

int AmountOfBlockedLinesOfLengthKGene::boardProperty(Board b, int row, int col) {
    assert (row < b.getRows() && col >= 0 && col < b.getColumns());
    cerr << "AmountOfBlockedLinesOfLengthKGene with k = " << k << "." << endl;
    return b.amountOfLinesOfLengthK(row, col, k, Players::THEM);
}

class AmountOfFreeLinesOfLengthKGene : public Gene {
    public:
        AmountOfFreeLinesOfLengthKGene(int k);
        int boardProperty(Board b, int row, int col);
    private:
        int k;
};

AmountOfFreeLinesOfLengthKGene::AmountOfFreeLinesOfLengthKGene(int k) : k(k) {}

int AmountOfFreeLinesOfLengthKGene::boardProperty(Board b, int row, int col) {
    assert (row < b.getRows() && col >= 0 && col < b.getColumns());
    cerr << "AmountOfFreeLinesOfLengthKGene with k = " << k << "." << endl;
    return b.amountOfLinesOfLengthK(row, col, k, Players::NONE);
}

class AmountOfNeighboursGene : public Gene {
    public:
        AmountOfNeighboursGene(Players player);
        int boardProperty(Board b, int row, int col);
    private:
        Players player;
};

AmountOfNeighboursGene::AmountOfNeighboursGene(Players player) : player(player) {}

int AmountOfNeighboursGene::boardProperty(Board b, int row, int col) {
    // Player indica de qué tipo son los vecinos que estamos devolviendo.
    assert (row < b.getRows() && col >= 0 && col < b.getColumns());
    cerr << "AmountOfNeighboursGene." << endl;
    return b.amountOfNeighbours(row, col, player);
}

class Genome {
    public:
        Genome(int c);
        Genome(int c, vector<float> geneWeights);
        float activate(Board b, int col);
        vector<float> geneWeights;
    private:
        int c;
        vector< Gene* > genes;
        void initialiseGenes();
};

void Genome::initialiseGenes() {
    // Inicializa un vector de genes donde a cada posición le corresponde un gen
    // con una función determinada (similar al genoma de un ser vivo real).
    for (int k = c; k >= 2; --k) {
        ConnectKGene* gene = new ConnectKGene(k);
        genes.push_back(gene);
    }
    for (int k = c; k >= 2; --k) {
        BlockKGene* gene = new BlockKGene(k);
        genes.push_back(gene);
    }
    for (int k = c; k >= 2; --k) {
        KFreeGene* gene = new KFreeGene(k);
        genes.push_back(gene);
    }
    for (int k = c; k >= 2; --k) {
        AmountOfLinesOfLengthKGene* gene = new AmountOfLinesOfLengthKGene(k);
        genes.push_back(gene);
    }
    for (int k = c; k >= 2; --k) {
        AmountOfBlockedLinesOfLengthKGene* gene = new AmountOfBlockedLinesOfLengthKGene(k);
        genes.push_back(gene);
    }
    for (int k = c; k >= 2; --k) {
        AmountOfFreeLinesOfLengthKGene* gene = new AmountOfFreeLinesOfLengthKGene(k);
        genes.push_back(gene);
    }

    genes.push_back(new AmountOfNeighboursGene(Players::US));
    genes.push_back(new AmountOfNeighboursGene(Players::THEM));
    genes.push_back(new AmountOfNeighboursGene(Players::NONE));
}

Genome::Genome(int c) : c(c) {
    initialiseGenes();

    // Genera una distribución U[-1, 1].
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<float> distribution(-1.0, 1.0);

    for (int i = 0; i < genes.size(); ++i) {
        // Le asigna a cada gen un peso con distribución U[-1, 1].
        geneWeights.push_back(distribution(generator));
    }

    cerr << "Gene weights: ";
    displayVector(geneWeights);
}

Genome::Genome(int c, vector<float> geneWeights) : c(c), geneWeights(geneWeights) {
    initialiseGenes();
}

float Genome::activate(Board b, int col) {
    // Al gen de genes[i] le corresponde el peso de geneWeights[i];
    // de esta forma, el producto interno entre genes y geneWeights
    // calcula el puntaje del tablero dado por ubicar una ficha
    // en una posición determinada.
    // Es necesario pasar col como parámetro para saber cuál es
    // el puntaje asociado a poner la ficha en una columna determinada.
    float result = 0;
    for (int i = 0; i < genes.size(); ++i) {
        int row = b.getLowestFreeCell(col);
<<<<<<< HEAD
        result += genes.at(i)->boardProperty(b, row, col) * geneWeights.at(i);
=======
        result += genes.at(i)->boardProperty(b, row, col) * geneWeights.at(i)
        // cerr << genes.at(i)->boardProperty(b, row, col) << endl;
>>>>>>> f3c9d7c7291c9579e93e4cedd77ef36c0c49cb5e
    }
    return result;
}

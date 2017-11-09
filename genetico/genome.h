#include <random>
#include <chrono>
#include <iostream>
#include <fstream>
#include "../common/player_random.h"

const int DEACTIVATE = 0;

// FUNCIÓN DE DEBUG

void displayVector(vector<float> v, ofstream& outputFile) {
    outputFile << "{ ";
    auto last = v.back();
    v.pop_back();
    for (auto i : v) {
        outputFile << i << ", ";
    }
    v.push_back(last);
    outputFile << last << " }" << endl;
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
    assert (row < b.rows() && col >= 0 && col < b.columns());

    // El gen se activa si la ficha introducida en col forma un K en línea.
    // cerr << "ConnectKGene with k = " << k << "." << endl;
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
    assert (row < b.rows() && col >= 0 && col < b.columns());

    // El gen se activa si la ficha introducida en col bloquea un K en línea del oponente.
    // cerr << "BlockKGene with k = " << k << "." << endl;
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
    assert (row < b.rows() && col >= 0 && col < b.columns());

    // El gen se activa si la ficha introducida en col tiene K-1 posiciones libres en línea alrededor.
    // cerr << "KFreeGene with k = " << k << "." << endl;
    return b.positionIsInLine(row, col, k, Players::NONE);
}

class NumberOfLinesOfLengthKGene : public Gene {
public:
    NumberOfLinesOfLengthKGene(int k);
    int boardProperty(Board b, int row, int col);
private:
    int k;
};

NumberOfLinesOfLengthKGene::NumberOfLinesOfLengthKGene(int k) : k(k) {}

int NumberOfLinesOfLengthKGene::boardProperty(Board b, int row, int col) {
    assert (row < b.rows() && col >= 0 && col < b.columns());

    // Cuenta la cantidad de líneas de largo k de las que forma parte la posición
    // al introducir la ficha.
    // cerr << "NumberOfLinesOfLengthKGene with k = " << k << "." << endl;
    return b.numberOfLinesOfLengthK(row, col, k, Players::US);
}

class NumberOfBlockedLinesOfLengthKGene : public Gene {
public:
    NumberOfBlockedLinesOfLengthKGene(int k);
    int boardProperty(Board b, int row, int col);
private:
    int k;
};

NumberOfBlockedLinesOfLengthKGene::NumberOfBlockedLinesOfLengthKGene(int k) : k(k) {}

int NumberOfBlockedLinesOfLengthKGene::boardProperty(Board b, int row, int col) {
    assert (row < b.rows() && col >= 0 && col < b.columns());
    // cerr << "NumberOfBlockedLinesOfLengthKGene with k = " << k << "." << endl;
    return b.numberOfLinesOfLengthK(row, col, k, Players::THEM);
}

class NumberOfFreeLinesOfLengthKGene : public Gene {
public:
    NumberOfFreeLinesOfLengthKGene(int k);
    int boardProperty(Board b, int row, int col);
private:
    int k;
};

NumberOfFreeLinesOfLengthKGene::NumberOfFreeLinesOfLengthKGene(int k) : k(k) {}

int NumberOfFreeLinesOfLengthKGene::boardProperty(Board b, int row, int col) {
    assert (row < b.rows() && col >= 0 && col < b.columns());
    // cerr << "NumberOfFreeLinesOfLengthKGene with k = " << k << "." << endl;
    return b.numberOfLinesOfLengthK(row, col, k, Players::NONE);
}

class SuicideMoveGene : public Gene {
public:
    SuicideMoveGene(int k);
    int boardProperty(Board b, int row, int col);
private:
    int k;
};

SuicideMoveGene::SuicideMoveGene(int k) : k(k) {}

int SuicideMoveGene::boardProperty(Board b, int row, int col) {
    return b.suicideMove(row, col, k);
}

class AntiSuicideMoveGene : public Gene {
public:
    AntiSuicideMoveGene(int k);
    int boardProperty(Board b, int row, int col);
private:
    int k;
};

AntiSuicideMoveGene::AntiSuicideMoveGene(int k) : k(k) {}

int AntiSuicideMoveGene::boardProperty(Board b, int row, int col) {
    return b.antiSuicideMove(row, col, k);
}

class NumberOfNeighboursGene : public Gene {
public:
    NumberOfNeighboursGene(Players player);
    int boardProperty(Board b, int row, int col);
private:
    Players player;
};

NumberOfNeighboursGene::NumberOfNeighboursGene(Players player) : player(player) {}

int NumberOfNeighboursGene::boardProperty(Board b, int row, int col) {
    // Player indica de qué tipo son los vecinos que estamos devolviendo.
    assert (row < b.rows() && col >= 0 && col < b.columns());
    // cerr << "NumberOfNeighboursGene." << endl;
    return b.numberOfNeighbours(row, col, player);
}

class PiecesInRowGene : public Gene {
public:
    PiecesInRowGene(Players player);
    int boardProperty(Board b, int row, int col);
private:
    Players player;
};

PiecesInRowGene::PiecesInRowGene(Players player) : player(player) {}

int PiecesInRowGene::boardProperty(Board b, int row, int col) {
    assert (row < b.rows() && col >= 0 && col < b.columns());
    return b.piecesInRow(row, player) * DEACTIVATE;
}

class PiecesInColumnGene : public Gene {
public:
    PiecesInColumnGene(Players player);
    int boardProperty(Board b, int row, int col);
private:
    Players player;
};

PiecesInColumnGene::PiecesInColumnGene(Players player) : player(player) {}

int PiecesInColumnGene::boardProperty(Board b, int row, int col) {
    assert (row < b.rows() && col >= 0 && col < b.columns());
    return b.piecesInColumn(col, player) * DEACTIVATE;
}

class PiecesInUpperLeftDiagonalGene : public Gene {
public:
    PiecesInUpperLeftDiagonalGene(Players player);
    int boardProperty(Board b, int row, int col);
private:
    Players player;
};

PiecesInUpperLeftDiagonalGene::PiecesInUpperLeftDiagonalGene(Players player) : player(player) {}

int PiecesInUpperLeftDiagonalGene::boardProperty(Board b, int row, int col) {
    assert (row < b.rows() && col >= 0 && col < b.columns());
    return b.piecesInUpperLeftDiagonal(row, col, player) * DEACTIVATE;
}

class PiecesInLowerLeftDiagonalGene : public Gene {
public:
    PiecesInLowerLeftDiagonalGene(Players player);
    int boardProperty(Board b, int row, int col);
private:
    Players player;
};

PiecesInLowerLeftDiagonalGene::PiecesInLowerLeftDiagonalGene(Players player) : player(player) {}

int PiecesInLowerLeftDiagonalGene::boardProperty(Board b, int row, int col) {
    assert (row < b.rows() && col >= 0 && col < b.columns());
    return b.piecesInLowerLeftDiagonal(row, col, player) * DEACTIVATE;
}

class ColumnHeightGene : public Gene {
public:
    ColumnHeightGene();
    int boardProperty(Board b, int row, int col);
};

ColumnHeightGene::ColumnHeightGene() {}

int ColumnHeightGene::boardProperty(Board b, int row, int col) {
    return b.columnHeight(col) * DEACTIVATE;
}

class DistanceToPieceGene : public Gene {
public:
    DistanceToPieceGene(Players player);
    int boardProperty(Board b, int row, int col);
private:
    Players player;
};

DistanceToPieceGene::DistanceToPieceGene(Players player) : player(player) {}

int DistanceToPieceGene::boardProperty(Board b, int row, int col) {
    assert(row >= 0 && row < b.rows() && col >= 0 && col < b.columns());
    return b.distanceToPiece(row, col, player) * DEACTIVATE;
}

class Genome {
private:
    int c;
    vector< Gene* > genes;
    vector< Gene* > initialiseGenes();
public:
    Genome(int c);
    Genome(int c, vector<float> geneWeights);
    float activate(Board b, int col);
    vector<float> geneWeights;

};

bool theHeadersHaveBeenSet = false;

vector< Gene* > Genome::initialiseGenes() {
    // Inicializa un vector de genes donde a cada posición le corresponde un gen
    // con una función determinada (similar al genoma de un ser vivo real).
    vector< Gene* > genes;
    for (int k = c; k >= 2; --k) {
        ConnectKGene* gene = new ConnectKGene(k);
        genes.push_back(gene);
        #ifdef GENECORRELATION
        if(!theHeadersHaveBeenSet){
           cerr << "Connect " << k << " Gene;";
        }
        #endif
    }
    for (int k = c; k >= 2; --k) {
        BlockKGene* gene = new BlockKGene(k);
        genes.push_back(gene);
        #ifdef GENECORRELATION
        if(!theHeadersHaveBeenSet){
            cerr << "Block " << k << " Gene;";
        }
        #endif
    }
    for (int k = c; k >= 2; --k) {
        KFreeGene* gene = new KFreeGene(k);
        genes.push_back(gene);
        #ifdef GENECORRELATION
        if(!theHeadersHaveBeenSet){
            cerr << k << " Free Gene;";
        }
        #endif
    }
    for (int k = c; k >= 2; --k) {
        NumberOfLinesOfLengthKGene* gene = new NumberOfLinesOfLengthKGene(k);
        genes.push_back(gene);
        #ifdef GENECORRELATION
        if(!theHeadersHaveBeenSet){
            cerr <<"Number Of Lines Of Length " << k << " Gene;";
        }
        #endif
    }
    for (int k = c; k >= 2; --k) {
        NumberOfBlockedLinesOfLengthKGene* gene = new NumberOfBlockedLinesOfLengthKGene(k);
        genes.push_back(gene);
        #ifdef GENECORRELATION
        if(!theHeadersHaveBeenSet){
            cerr <<"Number Of Blocked Lines Of Length " << k << " Gene;";
        }
        #endif
    }
    for (int k = c; k >= 2; --k) {
        NumberOfFreeLinesOfLengthKGene* gene = new NumberOfFreeLinesOfLengthKGene(k);
        genes.push_back(gene);
        #ifdef GENECORRELATION
        if(!theHeadersHaveBeenSet){
            cerr <<"Number Of Free Lines Of Length " << k << " Gene;";
        }
        #endif
    }
    for (int k = c; k >= 2; --k) {
        SuicideMoveGene* gene = new SuicideMoveGene(k);
        genes.push_back(gene);
        #ifdef GENECORRELATION
        if(!theHeadersHaveBeenSet){
            cerr <<"Suicide Move Gene " << k << ";";
        }
        #endif
    }
    for (int k = c; k >= 2; --k) {
        AntiSuicideMoveGene* gene = new AntiSuicideMoveGene(k);
        genes.push_back(gene);
    }

    vector<Players> ps = {Players::US, Players::THEM, Players::NONE};

    for (auto p : ps) {
        genes.push_back(new NumberOfNeighboursGene(p));
        genes.push_back(new PiecesInRowGene(p));
        genes.push_back(new PiecesInColumnGene(p));
        genes.push_back(new PiecesInUpperLeftDiagonalGene(p));
        genes.push_back(new PiecesInLowerLeftDiagonalGene(p));
        genes.push_back(new DistanceToPieceGene(p));
    }

    genes.push_back(new ColumnHeightGene);

        #ifdef GENECORRELATION
        if(!theHeadersHaveBeenSet){
            cerr <<"Number Of numberOfNeighbours Gene: US " << ";"
            <<"Pieces in Row Gene: US " << ";"
            <<"Pieces in Column Gene: US " << ";"
            <<"Number Of numberOfNeighbours Gene: THEM " << ";"
            <<"Pieces in Row Gene: THEM " << ";"
            <<"Pieces in Column Gene: THEM " << ";"
            <<"Number Of numberOfNeighbours Gene: NONE " << ";"
            <<"Pieces in Row Gene: NONE " << ";"
            <<"Pieces in Column Gene: NONE ";
        }
        #endif


       #ifdef GENECORRELATION
        if(!theHeadersHaveBeenSet){
            cerr <<endl;
        }
        #endif

    theHeadersHaveBeenSet = true;

    // genes.push_back(new NumberOfNeighboursGene(Players::US));
    // genes.push_back(new NumberOfNeighboursGene(Players::THEM));
    // genes.push_back(new NumberOfNeighboursGene(Players::NONE));
    return genes;
}

Genome::Genome(int c)
        : c(c)
        , genes(initialiseGenes()) {
    // Genera una distribución U[-1, 1].
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<float> distribution(-1.0, 1.0);

    for (unsigned int i = 0; i < genes.size(); ++i) {
        // Le asigna a cada gen un peso con distribución U[-1, 1].
        geneWeights.push_back(distribution(generator));
    }
}

Genome::Genome(int c, vector<float> geneWeights)
        : c(c)
        , genes(initialiseGenes())
        , geneWeights(geneWeights) {
}

float Genome::activate(Board b, int col) {
    // Al gen de genes[i] le corresponde el peso de geneWeights[i];
    // de esta forma, el producto interno entre genes y geneWeights
    // calcula el puntaje del tablero dado por ubicar una ficha
    // en una posición determinada.
    // Es necesario pasar col como parámetro para saber cuál es
    // el puntaje asociado a poner la ficha en una columna determinada.
    float result = 0;
    for (unsigned int i = 0; i < genes.size(); ++i) {
        int row = b.lowestFreeCell(col);
        result += genes.at(i)->boardProperty(b, row, col) * geneWeights.at(i);
    }
    return result;
}

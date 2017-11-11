#ifndef STARTUP3_ALGO3_GENOME_H
#define STARTUP3_ALGO3_GENOME_H

#include <chrono>
#include <iostream>
#include <fstream>
#include <cassert>
#include <random>
#include "../common/board.h"
#include "../common/possible_move.h"
#include "gene.h"


const int DEACTIVATE = 1;

#ifdef GENEKNOCKOUT
DEACTIVATE = 0;
#endif

// FUNCIÓN DE DEBUG

void displayVector(vector<float> v, ostream& outputFile) {
    outputFile << "{ ";
    auto last = v.back();
    v.pop_back();
    for (auto i : v) {
        outputFile << i << ", ";
    }
    v.push_back(last);
    outputFile << last << " }" << endl;
}

class ConnectKGene : public Gene {
public:
    ConnectKGene(int k);
    int boardProperty(Board b, const PossibleMove &move);
private:
    int k;
};

ConnectKGene::ConnectKGene(int k) : k(k) {}

int ConnectKGene::boardProperty(Board b, const PossibleMove &move) {
    assert (move.row() < b.rows() && move.move() >= 0 && move.move() < b.columns());

    // El gen se activa si la ficha introducida en col forma un K en línea.
    // cerr << "ConnectKGene with k = " << k << "." << endl;
    return b.positionIsInLine(move.row(), move.move(), k, move.us());
}

class BlockKGene : public Gene {
public:
    BlockKGene(int k);
    int boardProperty(Board b, const PossibleMove &move);
private:
    int k;
};

BlockKGene::BlockKGene(int k) : k(k) {}

int BlockKGene::boardProperty(Board b, const PossibleMove &move) {
    assert (move.row() < b.rows() && move.move() >= 0 && move.move() < b.columns());

    // El gen se activa si la ficha introducida en col bloquea un K en línea del oponente.
    // cerr << "BlockKGene with k = " << k << "." << endl;
    return b.positionIsInLine(move.row(), move.move(), k, move.em());
}

class KFreeGene : public Gene {
public:
    KFreeGene(int k);
    int boardProperty(Board b, const PossibleMove &move);
private:
    int k;
};

KFreeGene::KFreeGene(int k) : k(k) {}

int KFreeGene::boardProperty(Board b, const PossibleMove &move) {
    assert (move.row() < b.rows() && move.move() >= 0 && move.move() < b.columns());

    // El gen se activa si la ficha introducida en col tiene K-1 posiciones libres en línea alrededor.
    // cerr << "KFreeGene with k = " << k << "." << endl;
    return b.positionIsInLine(move.row(), move.move(), k, Players::NONE);
}

class NumberOfLinesOfLengthKGene : public Gene {
public:
    NumberOfLinesOfLengthKGene(int k);
    int boardProperty(Board b, const PossibleMove &move);
private:
    int k;
};

NumberOfLinesOfLengthKGene::NumberOfLinesOfLengthKGene(int k) : k(k) {}

int NumberOfLinesOfLengthKGene::boardProperty(Board b, const PossibleMove &move) {
    assert (move.row() < b.rows() && move.move() >= 0 && move.move() < b.columns());

    // Cuenta la cantidad de líneas de largo k de las que forma parte la posición
    // al introducir la ficha.
    // cerr << "NumberOfLinesOfLengthKGene with k = " << k << "." << endl;
    return b.numberOfLinesOfLengthK(move.row(), move.move(), k, move.us());
}

class NumberOfBlockedLinesOfLengthKGene : public Gene {
public:
    NumberOfBlockedLinesOfLengthKGene(int k);
    int boardProperty(Board b, const PossibleMove &move);
private:
    int k;
};

NumberOfBlockedLinesOfLengthKGene::NumberOfBlockedLinesOfLengthKGene(int k) : k(k) {}

int NumberOfBlockedLinesOfLengthKGene::boardProperty(Board b, const PossibleMove &move) {
    assert (move.row() < b.rows() && move.move() >= 0 && move.move() < b.columns());
    // cerr << "NumberOfBlockedLinesOfLengthKGene with k = " << k << "." << endl;
    return b.numberOfLinesOfLengthK(move.row(), move.move(), k, move.em());
}

class NumberOfFreeLinesOfLengthKGene : public Gene {
public:
    NumberOfFreeLinesOfLengthKGene(int k);
    int boardProperty(Board b, const PossibleMove &move);
private:
    int k;
};

NumberOfFreeLinesOfLengthKGene::NumberOfFreeLinesOfLengthKGene(int k) : k(k) {}

int NumberOfFreeLinesOfLengthKGene::boardProperty(Board b, const PossibleMove &move) {
    assert (move.row() < b.rows() && move.move() >= 0 && move.move() < b.columns());
    // cerr << "NumberOfFreeLinesOfLengthKGene with k = " << k << "." << endl;
    return b.numberOfLinesOfLengthK(move.row(), move.move(), k, Players::NONE);
}

class SuicideMoveGene : public Gene {
public:
    SuicideMoveGene(int k);
    int boardProperty(Board b, const PossibleMove &move);
private:
    int k;
};

SuicideMoveGene::SuicideMoveGene(int k) : k(k) {}

int SuicideMoveGene::boardProperty(Board b, const PossibleMove &move) {
    return b.nextMoveConnectsK(move.row(), move.move(), k, move.em());
}

class AntiSuicideMoveGene : public Gene {
public:
    AntiSuicideMoveGene(int k);
    int boardProperty(Board b, const PossibleMove &move);
private:
    int k;
};

AntiSuicideMoveGene::AntiSuicideMoveGene(int k) : k(k) {}

int AntiSuicideMoveGene::boardProperty(Board b, const PossibleMove &move) {
    return b.nextMoveConnectsK(move.row(), move.move(), k, move.us());
}

class NumberOfNeighboursGene : public Gene {
public:
    NumberOfNeighboursGene(Players player);
    int boardProperty(Board b, const PossibleMove &move);
private:
    Players player;
};

NumberOfNeighboursGene::NumberOfNeighboursGene(Players player) : player(player) {}

int NumberOfNeighboursGene::boardProperty(Board b, const PossibleMove &move) {
    // Player indica de qué tipo son los vecinos que estamos devolviendo.
    assert (move.row() < b.rows() && move.move() >= 0 && move.move() < b.columns());
    // cerr << "NumberOfNeighboursGene." << endl;
    return b.numberOfNeighbours(move.row(), move.move(), player);
}

class PiecesInRowGene : public Gene {
public:
    PiecesInRowGene(Players player);
    int boardProperty(Board b, const PossibleMove &move);
private:
    Players player;
};

PiecesInRowGene::PiecesInRowGene(Players player) : player(player) {}

int PiecesInRowGene::boardProperty(Board b, const PossibleMove &move) {
    assert (move.row() < b.rows() && move.move() >= 0 && move.move() < b.columns());
    return b.piecesInRow(move.row(), player) * DEACTIVATE;
}

class PiecesInColumnGene : public Gene {
public:
    PiecesInColumnGene(Players player);
    int boardProperty(Board b, const PossibleMove &move);
private:
    Players player;
};

PiecesInColumnGene::PiecesInColumnGene(Players player) : player(player) {}

int PiecesInColumnGene::boardProperty(Board b, const PossibleMove &move) {
    assert (move.row() < b.rows() && move.move() >= 0 && move.move() < b.columns());
    return b.piecesInColumn(move.move(), player) * DEACTIVATE;
}

class PiecesInUpperLeftDiagonalGene : public Gene {
public:
    PiecesInUpperLeftDiagonalGene(Players player);
    int boardProperty(Board b, const PossibleMove &move);
private:
    Players player;
};

PiecesInUpperLeftDiagonalGene::PiecesInUpperLeftDiagonalGene(Players player) : player(player) {}

int PiecesInUpperLeftDiagonalGene::boardProperty(Board b, const PossibleMove &move) {
    assert (move.row() < b.rows() && move.move() >= 0 && move.move() < b.columns());
    return b.piecesInUpperLeftDiagonal(move.row(), move.move(), player) * DEACTIVATE;
}

class PiecesInLowerLeftDiagonalGene : public Gene {
public:
    PiecesInLowerLeftDiagonalGene(Players player);
    int boardProperty(Board b, const PossibleMove &move);
private:
    Players player;
};

PiecesInLowerLeftDiagonalGene::PiecesInLowerLeftDiagonalGene(Players player) : player(player) {}

int PiecesInLowerLeftDiagonalGene::boardProperty(Board b, const PossibleMove &move) {
    assert (move.row() < b.rows() && move.move() >= 0 && move.move() < b.columns());
    return b.piecesInLowerLeftDiagonal(move.row(), move.move(), player) * DEACTIVATE;
}

class ColumnHeightGene : public Gene {
public:
    ColumnHeightGene();
    int boardProperty(Board b, const PossibleMove &move);
};

ColumnHeightGene::ColumnHeightGene() {}

int ColumnHeightGene::boardProperty(Board b, const PossibleMove &move) {
    return b.columnHeight(move.move()) * DEACTIVATE;
}

class DistanceToPieceGene : public Gene {
public:
    DistanceToPieceGene(Players player);
    int boardProperty(Board b, const PossibleMove &move);
private:
    Players player;
};

DistanceToPieceGene::DistanceToPieceGene(Players player) : player(player) {}

int DistanceToPieceGene::boardProperty(Board b, const PossibleMove &move) {
    assert(move.row() >= 0 && move.row() < b.rows() && move.move() >= 0 && move.move() < b.columns());
    return b.distanceToPiece(move.row(), move.move(), player) * DEACTIVATE;
}

class Genome {
private:
    int c;
    vector< Gene* > genes;
    vector< Gene* > initialiseGenes();
public:
    Genome(int c);
    Genome(int c, vector<float> geneWeights);
    float activate(Board b, const PossibleMove &move);
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
        if (getenv("GENECORRELATION") != NULL) {
            if (!theHeadersHaveBeenSet) {
                cerr << "Connect " << k << " Gene;";
            }
        }
    }
    for (int k = c; k >= 2; --k) {
        BlockKGene* gene = new BlockKGene(k);
        genes.push_back(gene);
        if (getenv("GENECORRELATION") != NULL) {
            if (!theHeadersHaveBeenSet) {
                cerr << "Block " << k << " Gene;";
            }
        }
    }
    for (int k = c; k >= 2; --k) {
        KFreeGene* gene = new KFreeGene(k);
        genes.push_back(gene);
        if (getenv("GENECORRELATION") != NULL) {
            if (!theHeadersHaveBeenSet) {
                cerr << k << " Free Gene;";
            }
        }
    }
    for (int k = c; k >= 2; --k) {
        NumberOfLinesOfLengthKGene* gene = new NumberOfLinesOfLengthKGene(k);
        genes.push_back(gene);
        if (getenv("GENECORRELATION") != NULL) {
            if (!theHeadersHaveBeenSet) {
                cerr << "Number Of Lines Of Length " << k << " Gene;";
            }
        }
    }
    for (int k = c; k >= 2; --k) {
        NumberOfBlockedLinesOfLengthKGene* gene = new NumberOfBlockedLinesOfLengthKGene(k);
        genes.push_back(gene);
        if (getenv("GENECORRELATION") != NULL) {
            if (!theHeadersHaveBeenSet) {
                cerr << "Number Of Blocked Lines Of Length " << k << " Gene;";
            }
        }
    }
    for (int k = c; k >= 2; --k) {
        NumberOfFreeLinesOfLengthKGene* gene = new NumberOfFreeLinesOfLengthKGene(k);
        genes.push_back(gene);
        if (getenv("GENECORRELATION") != NULL) {
            if (!theHeadersHaveBeenSet) {
                cerr << "Number Of Free Lines Of Length " << k << " Gene;";
            }
        }
    }
    for (int k = c; k >= 2; --k) {
        SuicideMoveGene* gene = new SuicideMoveGene(k);
        genes.push_back(gene);
        if (getenv("GENECORRELATION") != NULL) {
            if (!theHeadersHaveBeenSet) {
                cerr << "Suicide Move Gene " << k << ";";
            }
        }
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

    if (getenv("GENECORRELATION") != NULL) {
        if (!theHeadersHaveBeenSet) {
            cerr << "Number Of numberOfNeighbours Gene: US " << ";"
                 << "Pieces in Row Gene: US " << ";"
                 << "Pieces in Column Gene: US " << ";"
                 << "Number Of numberOfNeighbours Gene: THEM " << ";"
                 << "Pieces in Row Gene: THEM " << ";"
                 << "Pieces in Column Gene: THEM " << ";"
                 << "Number Of numberOfNeighbours Gene: NONE " << ";"
                 << "Pieces in Row Gene: NONE " << ";"
                 << "Pieces in Column Gene: NONE ";
        }
    }


    if (getenv("GENECORRELATION") != NULL) {
        if (!theHeadersHaveBeenSet) {
            cerr << endl;
        }
    }

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

    unsigned int bound = genes.size();

    if (getenv("NONLINEAR") != NULL) {
        bound = bound * 2;
    }

    for (unsigned int i = 0; i < bound; ++i) {
        // Le asigna a cada gen un peso con distribución U[-1, 1].
        geneWeights.push_back(distribution(generator));
    }
}

Genome::Genome(int c, vector<float> geneWeights)
    : c(c)
    , genes(initialiseGenes())
    , geneWeights(geneWeights) {
}

float Genome::activate(Board b, const PossibleMove &move) {
    // Al gen de genes[i] le corresponde el peso de geneWeights[i];
    // de esta forma, el producto interno entre genes y geneWeights
    // calcula el puntaje del tablero dado por ubicar una ficha
    // en una posición determinada.
    // Es necesario pasar col como parámetro para saber cuál es
    // el puntaje asociado a poner la ficha en una columna determinada.
    float result = 0;

    unsigned int bound = genes.size();

    if (getenv("NONLINEAR") != NULL) {
        bound = bound * 2;
    }

    for (unsigned int i = 0; i < bound; ++i) {
        int aux = i > genes.size() ? 2 : 1;
        result += ((genes.at(i % (genes.size()))->boardProperty(b, move)) ^ aux) * geneWeights.at(i);
    }
    return result;
}

#endif //STARTUP3_ALGO3_GENOME_H

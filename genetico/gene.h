#ifndef STARTUP3_ALGO3_GENE_H
#define STARTUP3_ALGO3_GENE_H

#include "../common/board.h"
#include "../common/possible_move.h"

class Gene {
public:
    // Propiedad que debe cumplir el tablero para que se sume el peso del locus asociado al gen.
    virtual int boardProperty(Board b, const PossibleMove &move) = 0;
};

#endif //STARTUP3_ALGO3_GENE_H

#include <iomanip>
#include "../genetico/matingpool.h"

PlayerRandom sensei;

typedef vector<float> Parameters;

int evaluate(Parameters& gridLine) {
    PlayerGenetic grasshopper(Genome(4, gridLine));
    int wins = 0;
    for (int i = 0; i < 100; ++i) {
        Game game(6, 7, 4, 21);
        pair<int,int > result = game.playMatch(grasshopper, sensei);
        if (result.first == 1) {
            ++wins;
        }
    }

    cerr << wins;
    for (auto&& param : gridLine) {
        cerr << "," << param;
    }
    cerr << endl;
    return wins;
}

void parameterSubSweep(Parameters& gridLine, Parameters& subSweepMaxGridLine, int& subSweepMaxWins, const float& step, const int& paramsLeft, const int& subSweepLeft) {
    if (paramsLeft == 0 || subSweepLeft == 0) {
        auto wins = evaluate(gridLine);
        if (wins > subSweepMaxWins) {
            subSweepMaxWins = wins;
            subSweepMaxGridLine = gridLine;
        }
    }
    else {
        for (float p = -1.0f; p <= 1.0f; p += step) {
            gridLine.at(gridLine.size() - paramsLeft) = p;
            parameterSubSweep(gridLine, subSweepMaxGridLine, subSweepMaxWins, step, paramsLeft - 1, subSweepLeft - 1);
        }
    }
}


void parameterSweep(const float& step, const int& numberOfParams, const int& subSweepStep) {
    // en cada sweep me quedo con el mayor y dejo fijos esos valores
    // para el sweep siguiente
    Parameters gridLine(numberOfParams, 0.0f);
    for (int i = 0; i < numberOfParams; i += subSweepStep) {
        // En cada subsweep busco el que tenga más victorias, sin importar
        // si antes hubo otro con más victorias.
        // Esto evita overfitting.
        Parameters subSweepMaxGridLine(gridLine);
        int maxWins = -1;
        parameterSubSweep(gridLine, subSweepMaxGridLine, maxWins, step, numberOfParams - i, subSweepStep);
        gridLine = subSweepMaxGridLine;
    }

    auto last = gridLine.back();
    gridLine.pop_back();
    cout << "GridSensei: { ";
    for (auto&& param : gridLine) {
        cout << param << ", ";
    }
    cout << last << " }" << endl;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        cerr <<
        "Uso: ./main NPARAMS STEP SUBSTEP" << endl <<
        "    NPARAMS     Cantidad de parámetros totales a entrenar." << endl <<
        "    STEPS       Paso entre cada parámetro a probar. E.g.: 0.25." << endl <<
        "    SUBSTEP     Cantidad de parámetros en cada subgrilla a entrenar." << endl;
        return 1;
    }

    int numberOfParams = stoi(argv[1]);
    float step = stof(argv[2]);
    int subSweepStep = stoi(argv[3]);
    parameterSweep(step, numberOfParams, subSweepStep);
}

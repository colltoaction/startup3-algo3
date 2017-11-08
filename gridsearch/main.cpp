#include <iomanip>
#include "../genetico/matingpool.h"

PlayerRandom sensei;

int evaluate(vector<float>& gridLine) {
    PlayerGenetic grasshopper(Genome(4, gridLine));
    int wins = 0;
    for (int i = 0; i < 100; ++i) {
        Game game(6, 7, 4, 21);
        pair<int,int > result = game.playMatch(grasshopper, sensei);
        if (result.first == 1) {
            ++wins;
        }
    }

    cout << std::setw(4) << wins;
    for (auto&& param : gridLine) {
        cout << "," << std::setw(4) << param;
    }
    cout << endl;
    return wins;
}

int parameterSubSweep(vector<float>& gridLine, const float& step, const int& paramsLeft, const int& subSweepLeft) {
    if (paramsLeft == 0) {
        return -1;
    }

    if (subSweepLeft == 0) {
        for (int i = 0; i < paramsLeft; ++i) {
            gridLine.push_back(0);
        }
        int v = evaluate(gridLine);
        for (int i = 0; i < paramsLeft; ++i) {
            gridLine.pop_back();
        }
        return v;
    }
    else {
        int maxWins = -1;
        vector<float> maxGridLine;
        for (float p = -1.0f; p <= 1.0f; p += step) {
            gridLine.push_back(p);
            int v = parameterSubSweep(gridLine, step, paramsLeft - 1, subSweepLeft - 1);
            if (v > maxWins) {
                cout << "v > maxWins" << endl;
                maxWins = v;
                maxGridLine = gridLine;
            }
            gridLine.pop_back();
        }
        // reemplazamos gridLine con el mayor que hayamos encontrado
        gridLine = maxGridLine;
        return maxWins;
    }
}


// EL PROBLEMA ES QUE ESTOY USANDO GRIDLINE PARA ALMACENAR TANTO EL ACTUAL COMO EL MAYOR,
// Y ESO NO JUEGA PORQUE NO PUEDO ITERAR TRANQUILO


void parameterSweep(vector<float>& gridLine, const float& step, const int& paramsLeft, const int& subSweepStep) {
    for (int i = 0; i < paramsLeft; i += subSweepStep) {
        parameterSubSweep(gridLine, step, paramsLeft - i, subSweepStep);
        for (int i = 0; i < paramsLeft; ++i) {
            gridLine.push_back(0);
        }
        cout << "evaluate after: " << evaluate(gridLine) << endl;
    }
}

int main() {
    int numberOfParams = 43;
    vector<float> gridLine;
    float step = 0.5f;
    int subSweepStep = 2;
    parameterSweep(gridLine, step, numberOfParams, subSweepStep);
}

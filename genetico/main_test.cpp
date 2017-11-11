#include <iomanip>
#include "mating_pool.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 17) {
        cerr <<
        "Uso: ./main ROWS COLS C PIECES NSURVIVORS POPSIZE GAMES PC PM T MR PRMATING FFUNCTION ALPHA EXTRATE GENS" << endl <<
        "    ROWS           Filas." << endl <<
        "    COLS           Columnas." << endl <<
        "    C              Cantidad de fichas en línea necesarias para ganar." << endl <<
        "    PIECES         Fichas de cada jugador." << endl <<
        "    NSURVIVORS     Cantidad de sobrevivientes." << endl <<
        "    POPSIZE        Tamaño de la población." << endl <<
        "    GAMES          Cantidad de partidos para calcular el fitness." << endl <<
        "    PC             Probabilidad de que haya crossover en vez de mitosis." << endl <<
        "    PM             Probabilidad de que un alelo mute." << endl <<
        "    T              1 - probabilidad de que se produzca un corte en un punto dado durante el crossover." << endl <<
        "    MR             Radio de la mutación." << endl <<
        "    PRMATING       Probabilidad de que un individuo sobreviviente se reproduzca con uno aleatorio." << endl <<
        "    FFUNCTION      Función de fitness." << endl <<
        "    ALPHA          Importancia que se le da a la rapidez en la función de fitness 2." << endl <<
        "    EXTRATE        Cantidad de generaciones tras las que ocurre una extinción masiva." << endl <<
        "    GENS           Cantidad de generaciones de la evolución." << endl;

        return 1;
    }

        int rows = stoi(argv[1]);
        // cerr << rows << endl;

        int cols = stoi(argv[2]);
        // cerr << cols << endl;

        int c = stoi(argv[3]);
        // cerr << c << endl;

        int pieces = stoi(argv[4]);
        // cerr << pieces << endl;

        int amountOfSurvivors = stoi(argv[5]);
        // cerr << amountOfSurvivors << endl;

        int populationSize = stoi(argv[6]);
        // cerr << populationSize << endl;

        int games = stoi(argv[7]);
        // cerr << games << endl;

        float pc = stof(argv[8]);
        // cerr << pc << endl;

        float pm = stof(argv[9]);
        // cerr << pm << endl;

        float t = stof(argv[10]);
        // cerr << t << endl;

        float mr = stof(argv[11]);
        // cerr << mr << endl;

        float pRandomMating = stof(argv[12]);
        // cerr << pRandomMating << endl;

        int fitnessFunction = stoi(argv[13]);
        // cerr << fitnessFunction << endl;

        float alpha = stof(argv[14]);
        // cerr << alpha << endl;

        int extinctionRate = stoi(argv[15]);
        // cerr << extinctionRate << endl;

        int generations = stoi(argv[16]);
    // cerr << generations << endl;


    std::stringstream ss;
    ss << "../experimentacion/fitness_" << string(getenv("RIVAL")) << "_population" << populationSize << ".txt";
    string fitnessFileName = ss.str();

    ofstream fitnessOutputFile;
    fitnessOutputFile.open(fitnessFileName);

    fitnessOutputFile << "Generation;Fitness" << endl;

    MatingPool mp(rows, cols, c, pieces, amountOfSurvivors, populationSize, games, pc, pm, t, mr, pRandomMating, fitnessFunction, alpha, extinctionRate, fitnessOutputFile);

    mp.evolvePopulation(generations, 5);
    
    fitnessOutputFile.close();


}

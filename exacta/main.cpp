#include "player.h"

using namespace std;

void game_main() {
    int rows, columns, p, c;
    cin >> columns >> rows >> p >> c;

    string input;
    cin >> input;

    Game game = Game(rows,columns,p,c);
    Player player = Player(game);

    int placedPieces = 0;

    if (input == "vos") {
        column jugada = player.calcularJugada(ALFA_BETA, placedPieces);
        player.addPiece(jugada, 1);
        cout << jugada << endl;
        placedPieces++;
    }

    while (true) {
        cin >> input;
        if (input == "ganaste" || input == "perdiste" || input == "empataron") {
            cerr << "se terminó: " << input << endl << flush;
            // terminó este juego
            return;
        }

        cerr << "input: " << input << endl << flush;

        // leo jugada del otro
        column jugada_otro = stoi(input);
        player.addPiece(jugada_otro, -1);
        player.printBoard();

        // calculo la jugada
        column jugada = player.calcularJugada(ALFA_BETA, placedPieces);
        player.addPiece(jugada, 1);
        placedPieces++;
        player.printBoard();
        cout << jugada << endl;
    }
}

int main() {
    while (true) {
        string color;
        cin >> color;
    
        if (color == "salir") {
            cout << "listo" << endl;
            return 0;
        }

        string color_otro;
        cin >> color_otro;

        // por ahora ignoramos los colores
        game_main();
    }
}
#include "player.h"

using namespace std;

void game_main() {
    int rows, columns, p, c;
    cin >> columns >> rows >> c >> p;

    string input;
    cin >> input;

    Game game = Game(rows,columns,p,c);
    Player player = Player(game);

    int placedPieces = 0;

    if (input == "vos") {
        column move = player.calculateMove(SolutionType::MINIMAX, placedPieces);
        player.addPiece(move, Winner::PLAYER1);
        cout << move << endl;
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
        column opponentMove = stoi(input);
        player.addPiece(opponentMove, Winner::PLAYER2);
        player.printBoard();

        // calculo la jugada
        column move = player.calculateMove(SolutionType::MINIMAX, placedPieces);
        player.addPiece(move, Winner::PLAYER1);
        placedPieces++;
        player.printBoard();
        cout << move << endl;
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

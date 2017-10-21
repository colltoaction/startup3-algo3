
#include "player.h"

using namespace std;


void game_main() {
    int rows, columns, p, c;
    cin >> rows >> columns >> p >> c;

    string input;
    cin >> input;

    Game game = Game(rows,columns,p,c);
    Player player = Player(game);

    int placedPieces = 0;
    bool termino = false;

    while(!termino){
	    column jugada;

        if (input =="vos"){
        	//calculo la jugada
            // jugada = player.minimax(placedPieces,1).second;
            jugada = player.minimaxAB(placedPieces).second;
            cout << jugada;
            //agrego la ficha en la columna que elegi
            // game.addPiece(jugada,1);
            player.addPiece(jugada,1);
        }
        else {
        	if(input == "el"){
        		//si empieza el otro jugador, tengo que leer su jugada
	        	cin >> jugada;
        	}
        	else{
        		//paso el input a int
        		jugada = stoi(input);
        	}
        	//agrego la ficha que puso mi rival al tablero
            //game.addPiece(jugada,-1);
        	player.addPiece(jugada,-1);
	        player.printBoard();
	        // game.printBoard();	        //calculo la jugada
	        jugada = player.minimaxAB(placedPieces).second;
            // jugada = player.minimax(placedPieces,1).second;
            cout << jugada;
            //agrego la ficha en la columna elegida
            // game.addPiece(jugada,1);
            player.addPiece(jugada,1);
        }

        //incremento las fichas usadas
        placedPieces++;
		player.printBoard();
        // game.printBoard();

       	//espero a que juegue mi rival y leo su jugada
        cin >> input;
        
        if(input == "perdiste" || input == "ganaste" || input == "empataron") termino=true;
    }
    return;
}

int main() {
    string line;
    while (getline(cin, line) && line != "salir") {
        // line tiene los colores, por ahora lo ignoramos

        // además hay dos valores que no sabemos para qué son

        //descomento esto para poder correr el test
        // int dummy;
        // cin >> dummy >> dummy;
        game_main();
        line = "salir";
        break;
    }

    return 0;
}
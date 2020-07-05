/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/main.cpp,v 1.33 2003/04/29 08:19:31 fb Exp $
 *
 * Rivisions:
 * 	$Log: main.cpp,v $
 * 	Revision 1.33  2003/04/29 08:19:31  fb
 * 	Eliminated some warnings from g++ on linux
 * 	
 * 	Revision 1.32  2003/04/29 08:01:57  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 *
 * 	Revision 1.29  2003/04/22 15:54:51  fb
 * 	made all the functions in utils.h macros and put in macros.h
 *
 * 	Revision 1.27  2003/04/17 11:23:44  fb
 * 	improved the evaluation function and functions in the utils.cpp file
 *
 * 	Revision 1.22  2003/04/07 14:22:20  fb
 * 	Improved the alphaBeta function and trying to improve the code
 *
 * 	Revision 1.21  2003/03/26 16:50:43  fb
 * 	no message
 *
 * 	Revision 1.20  2003/03/26 09:52:54  fb
 * 	Removed the setPosition method and made constructor of ChessGame
 * 	available with a fen string
 *
 * 	Revision 1.19  2003/03/25 16:59:54  fb
 * 	Trying to locate the getId() 's and eliminate them.
 *
 * 	Revision 1.18  2003/03/25 13:11:09  fb
 * 	Compilable under linux too
 *
 * 	Revision 1.16  2003/03/21 19:43:42  fb
 * 	Trying to accelerate the machine!
 *
 * 	Revision 1.15  2003/03/21 09:19:34  fb
 * 	Added the alpha beta prunning algorithm
 *
 * 	Revision 1.11  2003/03/17 16:48:09  fb
 * 	Fixed a bug at promotion and undo of the promoting.
 *
 * 	Revision 1.10  2003/03/17 08:47:50  fb
 * 	Trying to find out the bug which tries to do an illegal move.
 * 	"This piece can not move to that square."
 *
 * 	Revision 1.8  2003/03/12 17:38:21  fb
 * 	Added a random function to choose between equal moves so that the program
 * 	does not play the same game every time.
 *
 * 	Revision 1.7  2003/03/12 08:43:49  fb
 * 	Second design with undo_move capability that works.
 *
 * 	Revision 1.5  2003/03/05 17:28:08  fb
 * 	Introduced the minmax function
 *
 * 	Revision 1.4  2003/03/05 16:36:11  fb
 * 	Moved to the new repository.
 *
 * 	Revision 1.2  2003/03/04 09:23:58  fb
 * 	Changed the name of the function calculateValues to evaluatePosition
 *
 * 	Revision 1.3  2003/02/26 18:11:22  fb
 * 	Copied getCanMoveToSquares from board to al of the pieces.
 *
 * 	Revision 1.2  2003/02/26 16:51:31  fb
 * 	Almost completed the rules. yet to optimize!
 *
 * 	Revision 1.1  2003/02/26 12:54:45  fb
 * 	Added to the cvs
 *
 *
 */

#include "chessgame.h"
#include "minmax.h"

void help() {
	cout << " This program accepts the following commands: \n";
	cout << "\t init  : To start a new game.\n";
	cout << "\t go    : To let computer play against itself.\n";
	cout << "\t fen   : To set up a position in fen notation.\n";
	cout << "\t undo  : to undo a move.\n";
	cout << "\t e2e4  : or any legal move to make a move.\n";
	cout << "\t list  : to show a list of legal moves.\n";
	cout << "\t depth : to set search depth.\n";
	cout << "\t exit  : to exit this program.\n";
	cout << "\t help  : to show this help screen.\n";

	cout << "\nPress enter to continue.\n";
	
	cin.ignore();

	cout << " Ok.\n";
}

int main(int32_t argc, char ** argv) {

	int depth1 = 5;

	srand((uint32_t) time(0));

	cout << "Hi Moon!\n";

	if (argc >= 2)
		depth1 = (int32_t) atoi(argv[1]);

	cout << "Using depth : " << (int) depth1 << endl;

	help();

	string fen;
	

	//fen = "/5R/7k/3Q/1P5P//4PP/R3K"; /* Mate in 1 ply */
	//fen = "r6k/6pp/7N//2Q/1K"; /* Mate in 3 ply */
	//fen = "r5k/5Npp///2Q/1K";  /* Mate in 5 ply */

	//fen = "/k1P1q//K"; //A pown promoting to a knight is better than a queen

	//fen = "r3k2r/ppp2ppp/2n1p/bB1qNb/3PpB/2P1P/PP1Q1PPP/R3K2R"; //complicated winning opening

	//fen = "r1k3nQ/p6p/p3b/q4Np//P1p/1P3PPP/R3K2R"; //complicated post opening position

	//fen = "3rk2r/4n1p/p2n/1pp1qp/4P2p/2N1Q/PPP3PP/2KR1B1R"; //attackedByLessThan bug

	//fen = "4B/1p/1p1kp//2N1K/7r"; //black to move (repetition case)

	//fen = "rnb2rk/ppp2ppp/1b1qpn/3p/3P/1BN1PN/PPP2PPP/R1BQ1RK"; //normal opening situation


	fen = "r6k/6pp//6N/2Q/1K";  /* Mate in 7 ply */

	//fen = "1R/k2r2b/q4p/2Pp/P6p/1QP3p/5PPP/1R4K"; //pown promoting

	//fen = "/pk1r/3p2p/3R1p1p/5P1P/1K4P/2P"; //repetition

	//fen = "rn1qkb1r/pbp2ppp/1p1p1n/4p/4P/2NP1NB/PPP2PPP/R2QKB1R"; //opponentHasMoreAttackers bug

	//fen = "r2q1rk/1ppb1ppp/2nbp/p/P2PpB/5NP/1PP1PPBP/R2Q1RK"; //Pown attacks a knight

	//fen = "r2qbrk/1pp2ppp/p1nbpn/3pN/1P1P1B/P1N3P/2P1PPBP/R2Q1RK"; //Attacked by less than but also by more than one piece

	//fen = "2Q///5pk/6np//Q5PP/7K"; //En passant test

	//fen = "k2B/5p/4p/4P//6K";	//En Passant checker 30/April/2003 Ali Nourali

	//fen = "////k//1P/6K"; //En Passant problem

	//fen = "r1bqk2r/ppp2ppp/2n2n/3pp/1b/1BN1PN/PPPP1PPP/R1BQK2R";

	//fen = "kbb2rr/1p/p3p1q/PN1pB/1n/R3Q/5PB/6K"; // mate in 9 ply

	ChessGame * game = new ChessGame(fen);
	game->depth = depth1;

	cout << *game;

	cout << " \n\t\t\t Mate in 7 ply !!\n";

	string command;
	Move hamfMove;

	int32_t no = 1;
	bool go = false;
	int32_t v = -1;

	bool computerMove = false;

	while (command != "exit") {

		if (!go && computerMove == false) {
			cin >> command;
		} else {
			command = "look";
			computerMove = false;
		}

		if (command == "exit")
			break;

		else if (command == "init") {
			no = 1;
			delete game;
			game = new ChessGame();
			cout << *game;
			continue;
		}

		else if (command == "go") {
			go = true;
			continue;
		}

		else if (command == "list") {

			hamfMove = game->listMoves();

			continue;

		}

		else if (command == "look") {

			game->nodes = 0;

			v = game->findBestMove();

			hamfMove = game->bestMove;

			cout << "\n\t\t **** " << (unsigned int) game->nodes << " nodes evaluated. ***\n";

			if (game->getSideToMove() == BLACK)
				cout << no << ". ... ";
			else
				cout << no << ". ";

			cout << hamfMove << " " ;

			if (abs(hamfMove.value) >= CHECKMATE) {

				cout << " CHECKMATE in " << (int) (game->depth - (abs(hamfMove.value) - CHECKMATE)) << endl;

			} else {
				cout << (int) hamfMove.value << ":" << v << endl;
			}

			if (!game->nodes) {
				cout << *game;
				go = false;

				cout << "  <<<<< GAME OVER ! >>>>>>  \n";
				continue;
			}

		}

		else if (command == "undo") {
			game->undoMove();
			cout << *game;
			continue;

		}

		else if (command == "fen") {
			string fen;
			cout << "Enter the fen notation : ";
			cin >> fen;
			delete game;
			game = new ChessGame(fen);
			cout << *game;
			continue;
		}
		
		else if (command == "depth") {
			cout << "Enter the desired depth : ";
			cin >> depth1;
			if (depth1 < 0) depth1 = 2;
			if (depth1 > 50) depth1 = 50;

			cout << " Setting depth to " << depth1 << endl;
			game->depth = depth1;
			continue;
		}
		else if (command == "help" || command == "?") {
			help();
			continue;
		}

		else {
			v = -1;
			hamfMove = command;

			if (game->getSideToMove() == BLACK) {
				hamfMove.promotePiece = 'q';
			} else {
				hamfMove.promotePiece = 'Q';
			}

			command = "look";
			computerMove = true;
		}

		if (game->checkMove(hamfMove)) {

			if (game->getSideToMove() == BLACK) {
				no++;
			}

			int32_t v1 = game->evaluateMove(hamfMove);

			cout << " First I thought the value was = " << (int) v1 << endl;

			game->makeMove(hamfMove);
			cout << *game;

		} else {

			computerMove = false;
			
			if (v == 0) {
				cout << "\n Remise \n";
			}

			go = false;
		}
	}

	cout << "End.\n";
	cin.ignore();

	delete game;

	return 0;
}





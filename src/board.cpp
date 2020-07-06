/*******************************************************************************************************************************
 *
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/board.cpp,v 1.26 2003/04/29 08:01:53 fb Exp $
 *
 * Rivision :
 * 	$Log: board.cpp,v $
 * 	Revision 1.26  2003/04/29 08:01:53  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 *
 * 	Revision 1.23  2003/04/22 15:54:53  fb
 * 	made all the functions in utils.h macros and put in macros.h
 *
 * 	Revision 1.20  2003/04/17 11:23:46  fb
 * 	improved the evaluation function and functions in the utils.cpp file
 *
 * 	Revision 1.15  2003/04/07 16:36:58  fb
 * 	Changed the list container to a set container
 *
 * 	Revision 1.14  2003/04/07 14:22:18  fb
 * 	Improved the alphaBeta function and trying to improve the code
 *
 * 	Revision 1.13  2003/03/25 16:59:52  fb
 * 	Trying to locate the getId() 's and eliminate them.
 *
 * 	Revision 1.12  2003/03/25 13:11:11  fb
 * 	Compilable under linux too
 *
 * 	Revision 1.10  2003/03/21 19:43:40  fb
 * 	Trying to accelerate the machine!
 *
 * 	Revision 1.9  2003/03/21 09:19:35  fb
 * 	Added the alpha beta prunning algorithm
 *
 * 	Revision 1.5  2003/03/17 16:48:11  fb
 * 	Fixed a bug at promotion and undo of the promoting.
 *
 * 	Revision 1.4  2003/03/17 08:47:51  fb
 * 	Trying to find out the bug which tries to do an illegal move.
 * 	"This piece can not move to that square."
 *
 * 	Revision 1.2  2003/03/12 17:39:55  fb
 * 	Found a mistake and a bug. still have to delete the promoting piece after an undo
 *
 * 	Revision 1.1  2003/03/12 08:43:46  fb
 * 	Second design with undo_move capability that works.
 *
 * 	Revision 1.7  2003/03/05 17:28:08  fb
 * 	Introduced the minmax function
 *
 * 	Revision 1.6  2003/03/05 16:36:12  fb
 * 	Moved to the new repository.
 *
 * 	Revision 1.4  2003/03/04 09:23:58  fb
 * 	Changed the name of the function calculateValues to evaluatePosition
 *
 * 	Revision 1.3  2003/03/04 09:10:42  fb
 * 	- Let the pieces wake up (not more handicap) after a check is dealed with
 * 	- Improved the evaluation function slightly
 * 	- Ring all the squares by a rookade. is it ok for the queen side ??
 * 	- Corrected the case where a Knight check's. It is different than when another piece check's.
 * 	- Corrected a bug in isDefended
 * 	- moved the code to move a rook by a rokade to ChessPiece::move
 *
 * 	Revision 1.2  2003/03/03 16:50:46  fb
 * 	Adding more comments
 *
 *
 ******************************************************************************************************************************/

#include "board.h"
#include "king.h"
#include "queen.h"
#include "bishop.h"
#include "knight.h"
#include "rook.h"
#include "pown.h"
#include "moveinfo.h"

/*******************************************************************************************************************************
 *
 * Purpose	:
 * parameters	:
 * return value	:
 ******************************************************************************************************************************/

/*******************************************************************************************************************************
 * The class Board represents a game board with heigth and width which has BOARD_SIZE * BOARD_SIZE squares
 * The squares can contain a piece and they administrator a list of pieces which are interested in it.
 * The pieces can book interest by calling addInterestFor(Piece) and they are notified with the method ring()
 ******************************************************************************************************************************/

ostream & operator << (ostream & os, Board & cg) {
	os << "\t\t    A  B  C  D  E  F  G  H  \n";
	os << "\t\t  --------------------------\n";
	for (int32_t jj = BOARD_SIZE - 1; jj >= 0; jj--) {
		os << "\t\t";
		os << (int) (jj + 1) << " |";
		for (int32_t ii = 0; ii < BOARD_SIZE; ii++) {
			if (!cg.squares[ii][jj]->getPiece())
				os << " . ";
			else if (cg.squares[ii][jj]->getPiece()->getColor() == BLACK) {
				os << "(" << (cg.squares[ii][jj]->getPiece()->getName()) << ")";
			} else {
				os << " " << (cg.squares[ii][jj]->getPiece()->getName()) << " ";
			}
		}
		os << "| "  << (int) (jj + 1) << endl;
		if (jj > 0)
			os << endl;
	}
	os << "\t\t  --------------------------\n";
	os << "\t\t    A  B  C  D  E  F  G  H  \n";
	os << "\n";
	return os;
}


/*******************************************************************************************************************************
 * The copy constructor of an object of type Board. it makes a clone of an existing Board object
 * Purpose	: Make a clone of an existing Board object for the purpose of evaluations
 * parameters	: A reference to another Board object
 * return value	: None
 ******************************************************************************************************************************/


Board::Board(string & fen, Color sidesMove, Square * enPas, bool wccqs, bool wccks,
			 bool bccqs, bool bccks) : moveInfoStack() {

	for (int z = 0; z < BOARD_SIZE; z++) {
		pownOnX[0][z] = 0;
		pownOnX[1][z] = 0;
		pownOnY[0][z] = 0;
		pownOnY[1][z] = 0;
	}

	//pownOnX[2][BOARD_SIZE] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
	//pownY[2][BOARD_SIZE] = {{7,7,7,7,7,7,7,7},{0,0,0,0,0,0,0,0}};

	repeated = false;

	promotionPiece[WHITE] = 'Q';
	promotionPiece[BLACK] = 'q';

	nMoves[WHITE] = 0;
	nMoves[BLACK] = 0;

	sideToMove = sidesMove;

	enPassantSquare[opponent(sideToMove)] = enPas;
	enPassantSquare[sideToMove] = 0;

	KINGS[WHITE] = 0;
	KINGS[BLACK] = 0;

	int32_t ii;

	for (ii = 0; ii < MAX_PIECES_PER_SIDE; ii++) {
 		pieceTable[0][ii] = 0;
		pieceTable[1][ii] = 0;
	}

	for (ii = 0; ii < BOARD_SIZE; ii++)
		for (int32_t j = 0; j < BOARD_SIZE; j++)
			squares[ii][j] = new Square(ii, j, this);


	size_t s = fen.size();
	size_t i = 0;
	int32_t x = 0;
	int32_t y = 7;
	char c;
	while (i < s) {
		c = fen.c_str()[i];
		if (isalpha(c)) {
			Piece * p = createPiece(c);
			if (c == 'k') {
				pieceTable[p->getColor()][p->getArrayIndex()] = p;
				KINGS[BLACK] = (King *) p;
				KINGS[BLACK]->canCastleBits = (bccks ? 1 : 0);
				KINGS[BLACK]->canCastleBits += (bccqs ? 2 : 0);
				squares[x][y]->setPiece(p);
				KINGS[BLACK]->setSquare(squares[x][y]);
			} else if (c == 'K') {
				pieceTable[p->getColor()][p->getArrayIndex()] = p;
				KINGS[WHITE] = (King *) p;
				KINGS[WHITE]->canCastleBits = (wccks ? 1 : 0);
				KINGS[WHITE]->canCastleBits += (wccqs ? 2 : 0);
				squares[x][y]->setPiece(p);
				KINGS[WHITE]->setSquare(squares[x][y]);
			} else {
				addPiece(p, squares[x][y]);
				if (p->canPromote()) {
					if (p->getColor() == WHITE) {
						p->setHasMoved(y != 1);
					} else {
						p->setHasMoved(y != 6);
					}
				}
			}
			x++;
		} else if (c == '/' || c == '\\') {
			x = 0;
			y--;
		} else {
			/* Nummeric */
			int j;
			stringstream str;
			str << c;
			str >> j;
			x += j;
		}
		i++;
	}

	updateAllPieces(NO_COLOR);
}

/*******************************************************************************************************************************
 * A constructor of an object of type Board. It creates a new object
 * Purpose	: Create an object
 * parameters	: None
 * return value	: None
 ******************************************************************************************************************************/

Board::Board() : moveInfoStack() {

	moveInfoStack.reserve(100);

	for (int z = 0; z < BOARD_SIZE; z++) {
		pownOnX[0][z] = 0;
		pownOnX[1][z] = 0;
		pownOnY[0][z] = 0;
		pownOnY[1][z] = 0;
	}

	repeated = false;

	promotionPiece[WHITE] = 'Q';
	promotionPiece[BLACK] = 'q';

	nMoves[WHITE] = 0;
	nMoves[BLACK] = 0;

	sideToMove = WHITE;
	enPassantSquare[WHITE] = 0;
	enPassantSquare[BLACK] = 0;

	KINGS[WHITE] = 0;
	KINGS[BLACK] = 0;

	for (int32_t i = 0; i < BOARD_SIZE; i++)
		for (int32_t j = 0; j < BOARD_SIZE; j++)
			squares[i][j] = new Square(i, j, this);

	initPieces();

	updateAllPieces(NO_COLOR);
}

/*******************************************************************************************************************************
 * The destructor of the class Board.
 * Purpose	: Free memory that was allocated by the constructor and have not been freed during the life cycle of the object.
 * parameters	: None
 * return value	: None
 ******************************************************************************************************************************/

Board::~Board() {

	cleanPieces();

	for (int32_t ii = 0; ii < BOARD_SIZE ; ii++)
		for (int32_t jj = 0; jj < BOARD_SIZE ; jj++)
			delete squares[ii][jj];

}

/*******************************************************************************************************************************
 * addPiece
 * Purpose	: Add a piece to this Board by saving a pointer to it in the pieces list
 * parameters	: Pointers to a Piece and a Square where the piece should be placed
 * return value	: None
 ******************************************************************************************************************************/

void Board::addPiece(Piece * p, Square * s) {

	Color c = p->getColor();

	p->setSquare(s);
	s->setPiece(p);

	/*It is very important to add interest to this square ?
	Because when a killed piece is set back after it was killed (undo_move)
	It has to update itself. This will happen when the moving piece goes back
	And rings the square of the killed piece!! */

	//Not more the case if we use the update method of the moving pieces

	//See the line: Update the killed piece that was put back
	//In SituationChangedBack

	//s->addInterestFor(p);
	//p->addInterestFor(s);

	nMoves[c] += p->getNumberOfMoves();

	pieceTable[c][p->getArrayIndex()] = p;
}

/*******************************************************************************************************************************
 * removePiece
 * Purpose	: Remove a piece from this Boards pieces list and from the Square of the Board
 * parameters	: A pointer to a Piece
 * return value	: None
 ******************************************************************************************************************************/

void Board::removePiece(Piece * p) {


	assert(!p->castles()); //Never remove a king

	p->removeAllAttacks();
	p->removeAllInterests();

	Color c = p->getColor();

#if defined _DEBUG

	Square * s = p->getSquare();

	assert(s);

	s->setPiece(0);
#else
	p->getSquare()->setPiece(0);
#endif

	p->setSquare(0);

	/* If a piece is removed then decrease the no. of moves */
	nMoves[c] -= p->getNumberOfMoves();
	pieceTable[c][p->getArrayIndex()] = 0;
}

/*******************************************************************************************************************************
 * getSquare
 * Purpose	: Return a pointer to a Square if the name of the square is known
 * parameters	: The name of the square as defined in squareNames array
 * return value	: A pointer to the Square. if the name is not recognized as a name of a square then 0 will be returned.
 ******************************************************************************************************************************/

Square * Board::getSquare(string & name) {
	for (int32_t ii = 0; ii < BOARD_SIZE; ii++) {
		for (int32_t jj = 0; jj < BOARD_SIZE; jj++) {
			if (squares[ii][jj]->getName() == name)
				return squares[ii][jj];
		}
	}
	return 0;
}


/*******************************************************************************************************************************
 * createPiece
 * Purpose	: Create a Piece
 * parameters	:
 * return value	:
 ******************************************************************************************************************************/

Piece * Board::createPiece(char c, int32_t arrayIdx) {
	Piece * p = 0;
	switch (c) {
		case ('K') : p = new King(WHITE, this); break;
		case ('Q') : p = new Queen(WHITE, this); break;
		case ('B') : p = new Bishop(WHITE, this); break;
		case ('N') : p = new Knight(WHITE, this); break;
		case ('R') : p = new Rook(WHITE, this); break;
		case ('P') : p = new Pown(WHITE, this); break;

		case ('k') : p = new King(BLACK, this); break;
		case ('q') : p = new Queen(BLACK, this); break;
		case ('b') : p = new Bishop(BLACK, this); break;
		case ('n') : p = new Knight(BLACK, this); break;
		case ('r') : p = new Rook(BLACK, this); break;
		case ('p') : p = new Pown(BLACK, this); break;
	}
	if (!p)
		return 0;

	if (arrayIdx != NO_UID)
		p->setArrayIndex(arrayIdx);

	return p;
}

void Board::initPieces() {
	Piece * p;
	p = createPiece('K');
	KINGS[WHITE] = (King *) p;
	pieceTable[p->getColor()][p->getArrayIndex()] = p;
	p->setSquare(squares[4][0]);
	squares[4][0]->setPiece(p);


	p = createPiece('Q'); addPiece(p, squares[3][0]);
	p = createPiece('B'); addPiece(p, squares[2][0]);
	p = createPiece('N'); addPiece(p, squares[1][0]);
	p = createPiece('R'); addPiece(p, squares[0][0]);
	p = createPiece('B'); addPiece(p, squares[5][0]);
	p = createPiece('N'); addPiece(p, squares[6][0]);
	p = createPiece('R'); addPiece(p, squares[7][0]);
	p = createPiece('P'); addPiece(p, squares[0][1]);
	p = createPiece('P'); addPiece(p, squares[1][1]);
	p = createPiece('P'); addPiece(p, squares[2][1]);
	p = createPiece('P'); addPiece(p, squares[3][1]);
	p = createPiece('P'); addPiece(p, squares[4][1]);
	p = createPiece('P'); addPiece(p, squares[5][1]);
	p = createPiece('P'); addPiece(p, squares[6][1]);
	p = createPiece('P'); addPiece(p, squares[7][1]);

	p = createPiece('k');
	KINGS[BLACK] = (King *) p;
	pieceTable[p->getColor()][p->getArrayIndex()] = p;
	p->setSquare(squares[4][7]);
	squares[4][7]->setPiece(p);


	p = createPiece('q'); addPiece(p, squares[3][7]);
	p = createPiece('b'); addPiece(p, squares[2][7]);
	p = createPiece('n'); addPiece(p, squares[1][7]);
	p = createPiece('r'); addPiece(p, squares[0][7]);
	p = createPiece('b'); addPiece(p, squares[5][7]);
	p = createPiece('n'); addPiece(p, squares[6][7]);
	p = createPiece('r'); addPiece(p, squares[7][7]);
	p = createPiece('p'); addPiece(p, squares[0][6]);
	p = createPiece('p'); addPiece(p, squares[1][6]);
	p = createPiece('p'); addPiece(p, squares[2][6]);
	p = createPiece('p'); addPiece(p, squares[3][6]);
	p = createPiece('p'); addPiece(p, squares[4][6]);
	p = createPiece('p'); addPiece(p, squares[5][6]);
	p = createPiece('p'); addPiece(p, squares[6][6]);
	p = createPiece('p'); addPiece(p, squares[7][6]);
}

void Board::updateAllPieces(Color c) {

	if (c == NO_COLOR) {
		updateAllPieces(WHITE);
		updateAllPieces(BLACK);
	} else {

		for (int32_t idx = 0; idx < MAX_PIECES_PER_SIDE; idx++) {
			if (pieceTable[c][idx])
				pieceTable[c][idx]->update();
		}

		updateKings();
	}
}

void Board::cleanPieces() {
	for (int32_t idx = 0; idx < MAX_PIECES_PER_SIDE; idx++) {
		if (pieceTable[0][idx])
			delete pieceTable[0][idx];
		if (pieceTable[1][idx])
			delete pieceTable[1][idx];
	}
}

void Board::getSquares(Square * s1, Square * s2, SomeSquares & to) {

	int32_t kx, ky, ax, ay, dx, dy;

	s1->getXY(kx, ky);
	s2->getXY(ax, ay);

	dx = SIGN(ax - kx);
	dy = SIGN(ay - ky);

	while (true) {
		kx += dx; ky += dy;

		if (dx > 0) {
			if (kx > ax) break;
		} else if (dx < 0) {
			if (kx < ax) break;
		} else {
			if(!dy)
				return;
		}

		if (dy > 0) {
			if (ky > ay) break;
		} else if (dy < 0) {
			if (ky < ay) break;
		} else {
			if(!dx)
				return;
		}
		setBitOn64_2(to, kx, ky);
	}
}


#define CASTLED_BONUS 500
#define CANT_CASTLE_MALUS 900
#define CAN_CASTLE_BONUS 250

#define ROOK_7TH_RANK_BONUS 200
#define FIRST_RANK_MALUS 200
#define SQUARE_FORWARD_POWN 500
#define DOUBLE_POWN_MALUS 3000
#define NAKED_KING_MALUS 3500
#define PIECE_AT_EDGE_MALUS 500

#define SALIK_POWN_BONUS 3500

int32_t Board::evaluatePosition() {

	score[0] = 0;
	score[1] = 0;
	pieceKilled[0] = 0;
	pieceKilled[1] = 0;

	int8_t pownY[2][BOARD_SIZE] = {{7,7,7,7,7,7,7,7},{0,0,0,0,0,0,0,0}};

	for (int32_t cc = 0; cc < 2; cc++) {

		Color opponentColor = opponent(cc);

		for (int32_t pieceIdx = 0; pieceIdx < MAX_PIECES_PER_SIDE; pieceIdx++) {

			Piece * piece = pieceTable[cc][pieceIdx];

			if (!piece) continue;

			if (piece->castles()) {

				score[cc] += (piece)->getPower();

				continue;

			}

			sEval = (piece)->getSquare();

			sEval->getXY(xEval, yEval);

			int32_t value = (piece)->getValue();


			score[cc] += (piece)->getPower() + value;

			if (piece->canPromote()) {

				if (cc == WHITE) {
					if (pownY[cc][xEval] > yEval) {
						pownY[cc][xEval] = yEval;
					}
				} else {
					if (pownY[cc][xEval] < yEval) {
						pownY[cc][xEval] = yEval;
					}
				}

				bool salik = false;

				if (xEval == 0) {
					if (pownOnX[opponentColor][xEval] == 0 && pownOnX[opponentColor][xEval + 1] == 0) {
						//Salik
						salik = true;
						score[cc] += SALIK_POWN_BONUS;
						score[cc] += ((cc == WHITE) ? SQUARE_FORWARD_POWN * yEval : SQUARE_FORWARD_POWN * (7 - yEval));
					}
				} else if (xEval == BOARD_SIZE - 1) {
					if (pownOnX[opponentColor][xEval] == 0 && pownOnX[opponentColor][xEval - 1] == 0) {
						//Salik
						salik = true;
						score[cc] += SALIK_POWN_BONUS;
						score[cc] += ((cc == WHITE) ? SQUARE_FORWARD_POWN * yEval : SQUARE_FORWARD_POWN * (7 - yEval));
					}
				} else {
					if (pownOnX[opponentColor][xEval] == 0 && pownOnX[opponentColor][xEval - 1] == 0
						&& pownOnX[opponentColor][xEval + 1] == 0) {
						//Salik
						salik = true;
						score[cc] += SALIK_POWN_BONUS;
						score[cc] += ((cc == WHITE) ? SQUARE_FORWARD_POWN * yEval : SQUARE_FORWARD_POWN * (7 - yEval));
					}
				}

				if (!salik && pownOnX[cc][xEval] > 1) {
					//Double or more powns on same x
					score[cc] -= (DOUBLE_POWN_MALUS * (pownOnX[cc][xEval] - 1));
				}

				continue;
			}

			//The following can be much simpler I think

			if (cc != sideToMove) { //This piece is not for the side that has to move

				int32_t diff = sEval->opponentHasMoreAttackers((Color) cc);

				if (diff > pieceKilled[cc]) {
					score[cc] += pieceKilled[cc];
					score[cc] -= diff;
					pieceKilled[cc] = diff;
				}

			}

			/* Look if it is bad for this piece to be at first rank */

			if (value < PIECE_VALUE_ROOK /* So that a rook, queen and king not included*/ ) {
				if (cc * 7 == yEval) {
					/* At first rank */
					score[cc] -= FIRST_RANK_MALUS;

				/*} else if((piece)->canPromote()) {
					// Increase the power of pown as forwarding
					score[cc] += ((cc == WHITE) ? SQUARE_FORWARD_POWN * yEval : SQUARE_FORWARD_POWN * (7 - yEval));
				*/
				} else if (xEval == 0 || xEval == 7) {
					score[cc] -= PIECE_AT_EDGE_MALUS;
				}
			} else {
				if (piece->isEngagedInCastling() && yEval == (6 - (5 * cc))) {
					/* Rook or queen at 7th rank */
					score[cc] += ROOK_7TH_RANK_BONUS;
				}
			}

		}

		//For each side count the king safety

		if (!KINGS[cc]->getCastled()) {
			//Not yet castled
			if (!KINGS[cc]->canCastleBits) {
				//Can not castle
				score[cc] -= CANT_CASTLE_MALUS;
			} else {
				//Stil can castle
				score[cc] += CAN_CASTLE_BONUS;
			}
		} else {
			//castled

			score[cc] += CASTLED_BONUS;

			sEval = KINGS[cc]->getSquare();

			sEval->getXY(xEval, yEval);

			//If a pown beside the king is missing or is too far away in the yEval direction then malus
			if (xEval > 0) {
				if (pownOnX[cc][xEval - 1] == 0 || abs(pownY[cc][xEval - 1] - yEval) > 2) {
					score[cc] -= NAKED_KING_MALUS;
				}
			}

			if (pownOnX[cc][xEval] == 0 || abs(pownY[cc][xEval] - yEval) > 2) {
				score[cc] -= NAKED_KING_MALUS;
			}

			if (xEval < BOARD_SIZE - 1) {
				if (pownOnX[cc][xEval + 1] == 0 || abs(pownY[cc][xEval + 1] - yEval) > 2) {
					score[cc] -= NAKED_KING_MALUS;
				}
			}
		}

		//For each side count the double powns
		for (int xTest = 0; xTest < BOARD_SIZE; xTest++) {

			if (pownOnX[cc][xTest] > 0) {

				if (xTest == 0) {
					if (pownOnX[opponentColor][xTest] == 0 && pownOnX[opponentColor][xTest + 1] == 0) {
						//Salik
						score[cc] += SALIK_POWN_BONUS;
						//score[cc] += ((cc == WHITE) ? SQUARE_FORWARD_POWN * yEval : SQUARE_FORWARD_POWN * (7 - yEval));
					}
				} else if (xTest == BOARD_SIZE - 1) {
					if (pownOnX[opponentColor][xTest] == 0 && pownOnX[opponentColor][xTest - 1] == 0) {
						//Salik
						score[cc] += SALIK_POWN_BONUS;
					}
				} else {
					if (pownOnX[opponentColor][xTest] == 0 && pownOnX[opponentColor][xTest - 1] == 0
						&& pownOnX[opponentColor][xTest + 1] == 0) {
						//Salik
						score[cc] += SALIK_POWN_BONUS;
					}
				}

				if (pownOnX[cc][xTest] > 1) {
					//Double or more powns on same x
					score[cc] -= (DOUBLE_POWN_MALUS * (pownOnX[cc][xTest] - 1));
				}
			}
		}
	}

	return score[sideToMove] - score[opponent(sideToMove)];

}

void Board::updateKings() {
	/* After each Move */

	//cout << " ======= updating Kings." << endl;
	Color opponentColor = opponent(sideToMove);

	KINGS[opponentColor]->lookForPinnedAndCheckers();
	KINGS[sideToMove]->lookForPinnedAndCheckers();

	KINGS[sideToMove]->update();
	KINGS[opponentColor]->update();
	KINGS[sideToMove]->update();

}

#define ringIfNotZero(s) {if (s) s->ringPieces();}

void Board::updateEffectedPieces(MoveInfo & newSitu) {
	assert (newSitu.s1);
	assert (newSitu.s2);


	if (newSitu.s3) {

		//Castling

		assert (newSitu.s4);

		Piece * castlingRook = newSitu.s4->getPiece();

		assert (castlingRook->isEngagedInCastling());

		newSitu.s3->ringPieces();

		castlingRook->update();

		newSitu.s4->ringPieces();

	}

	//Square piece moved from
	newSitu.s1->ringPieces();
	//uint32_t toRing = newSitu.s1->getInterested();

	//Udate the moving piece
	newSitu.s2->getPiece()->update();

	//Square piece moved to
	newSitu.s2->ringPieces();
	//toRing |= newSitu.s2->getInterested();

#if defined _DEBUG
	if (newSitu.s2->getPiece() != newSitu.mover) {
		/* Look if the mover was a pown if the piece has changed by the move
		Only a pown can change in a promotion */
		assert (newSitu.mover->canPromote());
	}
#endif

	//If the enPassant square was deleted then ring it
	ringIfNotZero(newSitu.passantSquareDel);
	//if (newSitu.passantSquareDel) toRing |=  newSitu.passantSquareDel->getInterested();

	//If there was an en passant square created then ring it
	ringIfNotZero(newSitu.passantSquareCre);
	//if (newSitu.passantSquareCre) toRing |= newSitu.passantSquareCre->getInterested();

	/* If it was a enpassant killing then ring the square where the pown is restored */
	if (newSitu.killedAt != newSitu.s2) {
		ringIfNotZero(newSitu.killedAt);
		//toRing |= newSitu.killedAt->getInterested();
	}


	updateKings();

	toggleSideToMove();
}

//************************************************************************************

void Board::updateEffectedPiecesByUndo(MoveInfo & newSitu) {

	assert (newSitu.s1);
	assert (newSitu.s2);

	if (newSitu.s3) {

		//Castling

		assert (newSitu.s4);

		Piece * castlingRook = newSitu.s3->getPiece();
		assert (castlingRook->isEngagedInCastling());

		newSitu.s4->ringPieces();

		castlingRook->update();

		newSitu.s3->ringPieces();

	}


	//Ring the square the piece is was moved to (but it comes back)
	newSitu.s2->ringPieces();
	//uint32_t toRing = newSitu.s2->getInterested();

	newSitu.s1->getPiece()->update();

	//Ring the square where the piece is going back to
	newSitu.s1->ringPieces();
	//toRing |= newSitu.s1->getInterested();


	if (newSitu.s2->getPiece()) {
		//Update the killed piece that was put back
		newSitu.s2->getPiece()->update();
	}


	assert(newSitu.s1->getPiece() == newSitu.mover);

	//If there was an enPassant square deleted by this undo move then ring it
	ringIfNotZero(newSitu.passantSquareDel);
	//if (newSitu.passantSquareDel) toRing |= newSitu.passantSquareDel->getInterested();

	//If there was an enPassant square restord by this undo move then ring it
	ringIfNotZero(newSitu.passantSquareCre);
	//if (newSitu.passantSquareCre) toRing |= newSitu.passantSquareCre->getInterested();

	/* If it was a enpassant killing */
	if (newSitu.killedAt && newSitu.killedAt != newSitu.s2) {
		//ringIfNotZero(newSitu.killedAt);
		newSitu.killedAt->ringPieces();
		//And update the restored pown
		newSitu.killed->update();
		//toRing |= newSitu.killedAt->getInterested();
	}


	updateKings();

	toggleSideToMove();
}



//====================================================================

void Board::saveSituation(MoveInfo & situ) {

/*struct Situation {
	class Board * b;						Filled by the caller
	Color col;			Side moving			Filled here
	Color opponentColor;			The opponent			Filled here
	class Square * s1;		The original place		Filled by the caller
	class Square * s2;		Piece moved to		Fileld by the caller
	class Square * s3;		Another piece moved from	Filled by the king
	class Square * s4;		Another piece moved to	Fileld by the king
	class Piece * mover;		The piece that moved		Filled by the caller
	class Piece * killed;		The piece that been killed	Filled here
	class Square * killedAt;	The killed piece square	Filled here
	class Square * passantSquareDel;Passant square expired	Filled here
	class Square * passantSquareCre;New passant square created	Filled by Pown
	class Piece * wchecker1;	The white checker1		Filled here
	class Piece * wchecker2;	The white checker2		Filled here
	class Piece * bchecker1;	The black checker1		Filled here
	class Piece * bchecker2;	The black checker2		Filled here
	bool pieceWasMoved;						Filled here
	bool King_CouldCastleKingSide[2];				Filled here
	bool King_CouldCastleQueenSide[2];				Filled here
} situation; */

	assert (situ.b == this);
	/*Get the piece that is moving */
	Piece * p1 = situ.s1->getPiece();
	assert(p1);
	situ.col = p1->getColor();
	situ.opponentColor = opponent(situ.col);
	situ.mover = p1;

	situ.pieceWasMoved = p1->getHasMoved();

	situ.king_castleBits[WHITE] = KINGS[WHITE]->canCastleBits;
	situ.king_castleBits[BLACK] = KINGS[BLACK]->canCastleBits;

	/* This move will delete the enpassant square of the opponent */
	situ.passantSquareDel = enPassantSquare[situ.opponentColor];
	enPassantSquare[situ.opponentColor] = 0;

	int32_t x, y, yy;
	situ.s1->getXY(x, y);
	yy = situ.col == WHITE ? 6 : 1;

	if (p1->canPromote() && y == yy) {
		/* Look if the pown is promoting by this move
		this pown is promoting
		Replace it with the promoted piece */
		removePiece(p1);
		Piece * promo = createPiece(situ.promotePiece, p1->getArrayIndex());
		addPiece(promo, situ.s1);

		//promo->addInterestFor(situ.s2);	//Changed from s1 to s2 because of the removeInterest in piece::do_move
		//situ.s2->addInterestFor(promo);	//Not necessary if we update every moving piece
	}

	/* Remove the killed piece */
	Piece * p2 = situ.s2->getPiece();

	if (p2) {

#if defined _DEBUG

		if (p2->castles()) {
			cout << p1->getName() << " Killed the king (In saveSituation).\n";
			cout << situ;

			cout << " +++++++++++++++ The last moves : ++++++++++++++ \n";

			MoveInfoStack::iterator sit = moveInfoStack.begin();

			while (sit != moveInfoStack.end()) {
				cout << *sit;
				++sit;
			}

			cout << *this;
			assert(p2 != KINGS[situ.opponentColor]);
		}
		if (p2->getColor() == p1->getColor()) {
			cout << p1->getName() << " Killed own piece (In saveSituation).\n";
			cout << situ;

			cout << " +++++++++++++++ The last moves : ++++++++++++++ \n";

			MoveInfoStack::iterator sit = moveInfoStack.begin();

			while (sit != moveInfoStack.end()) {
				cout << *sit;
				++sit;
			}

			cout << *this;
			assert (p2->getColor() != p1->getColor());
		}

#endif

		//This piece is killed
		removePiece(p2);

		situ.killed = p2;
		situ.killedAt = situ.s2;
	}
}

Piece * Board::restoreSituation(MoveInfo & situ) {

	Piece * promoted = 0;

	assert (situ.b == this);

	/* Restore the killed piece */

	if (situ.killed) {
		addPiece(situ.killed, situ.killedAt);
	}

	Piece * p1 = situ.s1->getPiece();
	assert (p1);
	if (situ.mover != p1) {
		/* This piece was promoted, Remove it and restore the promoter */
		removePiece(p1);
		/* Don't delete the promoted one because he is calling this routine!! */
		addPiece(situ.mover, situ.s1);
		promoted = p1;
		p1 = situ.mover;
	}

	/* This undo move will restore the enpassant square of the opponent */
	if (situ.passantSquareDel) {
		//cout << " Restoring deleted passant square. " << situ.passantSquareDel->getName() << "\n";
		enPassantSquare[situ.opponentColor] = situ.passantSquareDel;
	}

	//The last move created an enpassant square
	//Because this move will be undone
	//So we have to delete the created passant square
	if (situ.passantSquareCre) {
		//cout << " Deleting created passant square. " << situ.passantSquareCre->getName() << "\n";
		enPassantSquare[situ.col] = 0;
		//situ.passantSquareCre = 0; //This is wriong. it has to be rung !
	}


	KINGS[WHITE]->canCastleBits = situ.king_castleBits[WHITE];
	KINGS[BLACK]->canCastleBits = situ.king_castleBits[BLACK];

	p1->setHasMoved(situ.pieceWasMoved);

	return promoted;
}




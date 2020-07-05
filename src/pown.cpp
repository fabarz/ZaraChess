/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/pown.cpp,v 1.21 2003/04/29 08:01:58 fb Exp $
 *
 * Rivisions:
 * 	$Log: pown.cpp,v $
 * 	Revision 1.21  2003/04/29 08:01:58  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 *
 * 	Revision 1.19  2003/04/22 15:54:49  fb
 * 	made all the functions in utils.h macros and put in macros.h
 *
 * 	Revision 1.16  2003/04/17 11:23:43  fb
 * 	improved the evaluation function and functions in the utils.cpp file
 *
 * 	Revision 1.11  2003/04/07 16:36:57  fb
 * 	Changed the list container to a set container
 *
 * 	Revision 1.10  2003/03/25 13:11:08  fb
 * 	Compilable under linux too
 *
 * 	Revision 1.8  2003/03/21 19:43:43  fb
 * 	Trying to accelerate the machine!
 *
 * 	Revision 1.7  2003/03/21 09:19:33  fb
 * 	Added the alpha beta prunning algorithm
 *
 *
 *
 *
 */

#include "pown.h"
#include "board.h"
#include "situation.h"

/*******************************************************************************
 *
 ******************************************************************************/

Pown::Pown(Color c, Board * b) :
	Piece(c, PIECE_VALUE_POWN, b, ((c == WHITE ? 'P' : 'p'))) {
}

Pown::~Pown() {
}

void Pown::squareChanged(Square * s) {

	/*There are three possibilities:
	1. One square forward x1 == x2 && abs(y2 - y1) == 1 (In this case have to look at 2 squares ahead again)
	2. Two squares forward x1 == x2 && abs(y2 - y1) == 2
	3. At the sides : abs(x1 - x2) == 1 :
		a. If there is a piece of opponent
		b. If there is enpassant
	*/

	board->changeNMoves(color, -1 * getNumberOfMoves());

	int32_t x1, y1, x2, y2;

	square->getXY(x1, y1);
	s->getXY(x2, y2);

	Piece * p = s->getPiece();


	if (x1 == x2) {

		//A square a head is rung (1 or 2 squares a head)

		if (p) {
			//If there is a piece in it then this pown can not move to it
			setBitOff64_2(canMoveToSquares, x2, y2);
		} else {
			//If there is no piece in it then this pown can move to it
			setBitOn64_2(canMoveToSquares, x2, y2);
		}

		if (abs(y2 - y1) == 1 && !hasMoved) {

			//If this pown has not moved yet and the square rung was one square ahead
			//Then we have to handle two squares ahead also

			assert (y2 == 2 || y2 == 5);

			if (y2 == 2) {
				assert (color == WHITE);
				y2 += 1;

			} else {
				assert (color == BLACK);
				y2 -= 1;
			}

			s = board->getSquare(x2, y2);

			if (p) {

				//There was a piece in the first square
				//This pown can neither move to the second square

				setBitOff64_2(canMoveToSquares, x2, y2);

				setBitOff64_2(interestedSquares, x2, y2);
				s->removeInterestOf(this);

			} else {

				//There was no piece in the first square

				//Add interest for the second square

				setBitOn64_2(interestedSquares, x2, y2);
				s->addInterestFor(this);

				//See if there is a piece in the second square

				if (s->getPiece()) {
					setBitOff64_2(canMoveToSquares, x2, y2);
				} else {
					setBitOn64_2(canMoveToSquares, x2, y2);
				}
			}
		}

	} else {
		//A square to the sides has been rung

		assert(abs(x2 - x1) == 1);
		assert(abs(y2 - y1) == 1);

		if (s->getColorOfPiece() == opponentColor) {

			assert (s != board->getPassantSquare(opponentColor));

			//Piece of opponent there

			setBitOn64_2(canMoveToSquares, x2, y2);


		} else if (s == board->getPassantSquare(opponentColor)) {

			//En passant square

#if defined _DEBUG

			if (p != 0) {
				cout << *board;
				cout << " Here the enpassant square " << s->getName() << " is not empty. \n";
				cout << " It contains a " << p->getName() << endl;

				cout << " Piece " << name << " at " << square->getName() << " was rung for it \n";
				assert (p == 0);
			}
#endif

			setBitOn64_2(canMoveToSquares, x2, y2);

		} else {

			//Own piece or empty

			setBitOff64_2(canMoveToSquares, x2, y2);
		}
	}

	board->changeNMoves(color, getNumberOfMoves());

}

void Pown::updateToSquares(int32_t x, int32_t y) {

	int32_t divY = (color == WHITE ? 1 : -1); /*1 - (2 * color);*/

	Square * s;
	//Piece * p;

	int32_t xy;
	int32_t dy;


	if (!hasMoved) //THis is the first time a pown moves. it can do 2 squares at once also
		dy = 2;
	else
		dy = 1;

	int32_t xx, yy;

	/* Move 1 and 2 squares forward */

	for (xy = 1; xy <= dy; xy++) {

		yy = y + (divY * xy);

		s = board->getSquare(x, yy);

		assert(s);

		s->addInterestFor(this);
		setBitOn64_2(interestedSquares, x, yy);

		if (s->getPiece()) {
			/* There is a piece in front of this pown */
			break;
		} else {
			/* Empty square */
			setBitOn64_2(canMoveToSquares, x, yy);
		}
	}

	/* Kill a piece of opponent */
	for (xy = -1; xy <= 1; xy += 2) {

		xx = x + xy;
		yy = y + divY;

		s = board->getSquare(xx, yy); /* These are the two squares to kill! */
		if (!s)
			continue;

		s->addInterestFor(this);
		setBitOn64_2(interestedSquares, xx, yy);

		s->addAttacker(this);
		setBitOn64_2(attackedSquares, xx, yy);

		Color pieceColor = s->getColorOfPiece();

		if (pieceColor == color) {
			//Our piece in square
			assert(board->getPassantSquare(opponentColor) != s);

		} else if (pieceColor == opponentColor) {
			/* This square has a piece of the opponent */

			assert (board->getPassantSquare(opponentColor) != s);

			setBitOn64_2(canMoveToSquares, xx, yy);

		} else {

			/* Square is empty */
			if (board->getPassantSquare(opponentColor) == s) {

				/* If (it is empty but enPassant) */
				setBitOn64_2(canMoveToSquares, xx, yy);

				/* Add this as an attacker to the enpassant pown */
				int32_t xPas, yPas;

				s->getXY(xPas, yPas);

				if (opponentColor == WHITE) {
					yPas++;
				} else {
					yPas--;
				}

				board->getSquare(xPas, yPas)->addAttacker(this);
				setBitOn64_2(attackedSquares, xPas, yPas);
			}


		}
	}
}

void Pown::do_move(Situation & situ) {

	situ.b->saveSituation(situ);

	assert (situ.col == opponent(situ.opponentColor));

#if defined _DEBUG

	if (situ.mover != this) {

		cout << "Unconsistency ==> Situ.mover = " << situ.mover->getName();
		cout << " mover is at " << situ.mover->getSquare()->getName() << endl;
		cout << " this = " << name << endl;
		cout << " this is at " << square->getName() << endl;
		cout << situ;
		cin.ignore();
	}
#endif

	if (situ.s1 != square) {
		//If this pown has promoted then it's square has set to zero by
		//Board::saveSituation
		assert (situ.s1->getPiece() != situ.mover);

		/* Forward this call to the promoted piece.
		   The other piece should also take into acoount what to do */
		situ.s1->getPiece()->do_move(situ);
		return;
	}

	int32_t x1, y1, x2, y2;

	square->getXY(x1, y1);
	situ.s2->getXY(x2, y2);

	Square * pas;

	/*situ.passantSquareDel is the oppoennts en passant square but the real enpassant
	 square was zeroed in
	  situ.saveSituation */

	if (situ.passantSquareDel == situ.s2) {

#if defined _DEBUG
		if(situ.killed) {
			cout << situ << endl;
			cout << "killed != 0\n" << *board;
		}

		if(situ.killedAt) {
			cout << situ << endl;
			cout << "killedAt != 0\n" << *board;
		}
#endif
		//An enpassant square must be empty !

		assert (!situ.killed);
		assert (!situ.killedAt);
		/* This is killing an en passant
		   Remove the opponent pown */

		pas = situ.b->getSquare(x2, y1);
		assert(pas);
		Piece * enPassantPown = pas->getPiece();

#if defined _DEBUG
		if (!enPassantPown) {
			cout << "BUG: enPassantPown == 0 move = " << square->getName()
				<< situ.s2->getName() << endl;

			cout << situ.b;

			cout << " Square " << (int) x2 << "," << (int) y1 << " is empty (Pown.cpp)\n";
			cout << situ;
			cin.ignore();
		}

#endif
		//The enpassant pown is killed. So remove it from board
		situ.b->removePiece(enPassantPown);

		situ.killed = enPassantPown;
		situ.killedAt = pas;

	} else if (abs(y1 - y2) == 2) { /* Does it cause en passant ? !hasMoved does not work here
				becasue it can move one square forward */

		Square * opPownS1 = situ.b->getSquare(x2 - 1, y2);
		Square * opPownS2 = situ.b->getSquare(x2 + 1, y2);

		Piece * opPown1 = opPownS1 ? opPownS1->getPiece() : 0;
		Piece * opPown2 = opPownS2 ? opPownS2->getPiece() : 0;

		if((opPown1 && opPown1->canPromote() && opPown1->getColor() != color) ||
			(opPown2 && opPown2->canPromote() && opPown2->getColor() != color)) {

			int32_t yy = (y1 == 1 ? 2 : 5);
			pas = situ.b->getSquare(x1, yy);
			situ.passantSquareCre = pas;

			situ.b->setPassantSquare(color, pas);
		}

	}

	hasMoved = true;

	//situ.s1->removeInterestOf(this);
									/* If we don't do this then we get an assertion failure
									 because the square makes sure every interested piece
									 has stayed on it's place when it rings them.
									 If this piece adds interest and then is killed.
									 It gets a null pointer. ???? */
	situ.s1->setPiece(0);
	square = situ.s2;
	situ.s2->setPiece(this);

	situ.b->updateEffectedPieces(situ);
}

Piece * Pown::undo_move(Situation & situ) {

	/* The first thing is to restore the pown back */
	situ.s2->setPiece(0);
	square = situ.s1;
	situ.s1->setPiece(this);

	assert (situ.col == opponent(situ.opponentColor));
	assert (situ.mover == this);

	Piece * p = situ.b->restoreSituation(situ);
	assert (!p);

	situ.b->updateEffectedPiecesByUndo(situ);

	return p;
}





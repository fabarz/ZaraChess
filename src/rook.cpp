/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/rook.cpp,v 1.20 2003/04/29 08:01:59 fb Exp $
 *
 * Rivisions:
 * 	$Log: rook.cpp,v $
 * 	Revision 1.20  2003/04/29 08:01:59  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 *
 * 	Revision 1.18  2003/04/22 15:54:49  fb
 * 	made all the functions in utils.h macros and put in macros.h
 *
 * 	Revision 1.15  2003/04/17 11:23:42  fb
 * 	improved the evaluation function and functions in the utils.cpp file
 *
 * 	Revision 1.10  2003/04/07 16:36:56  fb
 * 	Changed the list container to a set container
 *
 * 	Revision 1.9  2003/03/25 13:11:08  fb
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

#include "rook.h"
#include "king.h"
#include "moveinfo.h"

/*******************************************************************************
 *
 ******************************************************************************/

Rook::Rook(Color c, Board * b) :
	Piece(c, PIECE_VALUE_ROOK, b, ((c == WHITE ? 'R' : 'r'))) {

	hasMoved = false;
}

Rook::~Rook() {
}

void Rook::updateToSquares(int32_t x, int32_t y) {

	int32_t xx, yy, i, signX, signY;
	Square * s;

	bool oneSquareExtra;

	Piece * king;

	for (signX = -1; signX <= 1; signX += 1) {
		for (signY = -1; signY <= 1; signY += 1) {

			if (signX == signY || signX == -signY) {
				//The rook can not walk diagonaly
				continue;
			}

			oneSquareExtra = false;
			i = 0;

			while (true) {

				i++;

				xx = x + i * signX; yy = y + i * signY;

				s = board->getSquare(xx, yy);

				if (!s) {
					/* Edge of board reached */
					break;
				}

				s->addInterestFor(this);
				setBitOn64_2(interestedSquares, xx, yy);

				if (oneSquareExtra) {
					break;
				}

				s->addAttacker(this);
				setBitOn64_2(attackedSquares, xx, yy);

				if (s->getColorOfPiece() != color) {
					/* This square is empty or have opponent */
					setBitOn64_2(canMoveToSquares, xx, yy);
				}

				king = s->getPiece();

				if (king) {
					if (king == board->getKing(opponentColor)) {
						oneSquareExtra = true;
					} else {
						break;
					}
				}
			}
		}
	}
}

Piece * Rook::undo_move(MoveInfo & situ) {

	assert (situ.col == opponent(situ.opponentColor));
	/* Do the actual move */
	situ.s2->setPiece(0);
	square = situ.s1;
	situ.s1->setPiece(this);

	Piece * p = situ.b->restoreSituation(situ);
	situ.b->updateEffectedPiecesByUndo(situ);

	return p;
}

void Rook::do_move(MoveInfo & situ) {

	/* If a pown was promoted then don't call it again
		It has already called the saveSituation method */

	if (situ.mover == this)
		situ.b->saveSituation(situ);

	assert (situ.col == opponent(situ.opponentColor));

	if (!hasMoved) {
		int32_t x, y;
		hasMoved = true;
		square->getXY(x, y);
		if (!x) {
			//x == 0/ queen side gone
			board->getKing(color)->canCastleBits &= 0x1;
		} else /*if (x == 7)*/ {
			//x == 7 king side gone
			board->getKing(color)->canCastleBits &= 0x2;
		}
	}


	//situ.s1->removeInterestOf(this);
									/* If we don't do this then we get an assertion failure
									 because the square makes sure every interested piece
									 has stayed on it's place when it rings them.
									 If this piece adds interest and then is killed.
									 It gets a null pointer. ???? */

	/* Do the actual move */

	situ.s1->setPiece(0);
	square = situ.s2;
	situ.s2->setPiece(this);

	situ.b->updateEffectedPieces(situ);
}






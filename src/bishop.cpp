/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/bishop.cpp,v 1.21 2003/04/29 08:01:52 fb Exp $
 *
 * Rivisions:
 * 	$Log: bishop.cpp,v $
 * 	Revision 1.21  2003/04/29 08:01:52  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 * 	
 * 	Revision 1.18  2003/04/22 15:54:55  fb
 * 	made all the functions in utils.h macros and put in macros.h
 * 	
 * 	Revision 1.15  2003/04/17 11:23:47  fb
 * 	improved the evaluation function and functions in the utils.cpp file
 *
 * 	Revision 1.10  2003/04/07 16:36:59  fb
 * 	Changed the list container to a set container
 *
 * 	Revision 1.9  2003/03/25 13:11:12  fb
 * 	Compilable under linux too
 *
 * 	Revision 1.8  2003/03/21 19:43:39  fb
 * 	Trying to accelerate the machine!
 *
 * 	Revision 1.7  2003/03/21 09:19:36  fb
 * 	Added the alpha beta prunning algorithm
 *
 *
 *
 *
 */

#include "bishop.h"
#include "board.h"

/*******************************************************************************
 *
 ******************************************************************************/

Bishop::Bishop(Color c, Board * b) :
	Piece(c, PIECE_VALUE_BISHOP, b, (c == WHITE ? 'B' : 'b')) {
}

Bishop::~Bishop() {
}

void Bishop::updateToSquares(int32_t x, int32_t y) {

	int32_t xx, yy, i, signX, signY;
	Square * s;

	Piece * king;
	bool oneSquareExtra;

	for (signX = -1; signX <= 1; signX += 2) {
		for (signY = -1; signY <= 1; signY += 2) {

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
					/* Already counted for one square extra so enough now */
					break;
				}

				s->addAttacker(this);
				setBitOn64_2(attackedSquares, xx, yy);

				if (s->getColorOfPiece() != color) {

					/* This square is empty */
					setBitOn64_2(canMoveToSquares, xx, yy);

				}

				king = s->getPiece();

				if (king) {
					if (king == (Piece *) board->getKing(opponentColor)) {
						oneSquareExtra = true;
					} else {
						/* Other piece than king is reached */
						break;
					}
				}
			}
		}
	}
}




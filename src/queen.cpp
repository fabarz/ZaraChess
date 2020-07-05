/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/queen.cpp,v 1.21 2003/04/29 08:19:32 fb Exp $
 *
 * Rivisions:
 * 	$Log: queen.cpp,v $
 * 	Revision 1.21  2003/04/29 08:19:32  fb
 * 	Eliminated some warnings from g++ on linux
 * 	
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

#include "queen.h"
#include "board.h"

/*******************************************************************************
 *
 ******************************************************************************/

Queen::Queen(Color c, Board * b) :
	Piece(c, PIECE_VALUE_QUEEN, b, ((c == WHITE ? 'Q' : 'q'))) {
}

Queen::~Queen() {
}

void Queen::updateToSquares(int32_t x, int32_t y) {

	int32_t xx, yy, i, signX, signY;
	Square * s;

	Piece * king;
	bool oneSquareExtra;

	for (signX = -1; signX <= 1; signX += 1) {

		for (signY = -1; signY <= 1; signY += 1) {

			if (!signX && !signY) {
				/* If both are zero then continue */
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
					setBitOn64_2(canMoveToSquares, xx, yy);
				}

				king = s->getPiece();

				if (king) {
					if (king == (Piece *) board->getKing(opponentColor)) {
						oneSquareExtra = true;
					} else {
						/* Another piece than king is reached */
						break;
					}
				}
			}
		}
	}
}







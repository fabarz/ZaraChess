/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/knight.cpp,v 1.17 2003/04/29 08:01:56 fb Exp $
 *
 * Rivisions:
 * 	$Log: knight.cpp,v $
 * 	Revision 1.17  2003/04/29 08:01:56  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 * 	
 * 	Revision 1.15  2003/04/22 15:54:51  fb
 * 	made all the functions in utils.h macros and put in macros.h
 * 	
 * 	Revision 1.12  2003/04/17 11:23:44  fb
 * 	improved the evaluation function and functions in the utils.cpp file
 *
 * 	Revision 1.7  2003/04/07 16:36:57  fb
 * 	Changed the list container to a set container
 *
 * 	Revision 1.6  2003/03/21 19:43:42  fb
 * 	Trying to accelerate the machine!
 *
 * 	Revision 1.5  2003/03/21 09:19:34  fb
 * 	Added the alpha beta prunning algorithm
 *
 *
 *
 *
 */

#include "knight.h"
#include "board.h"
#include "square.h"

/*******************************************************************************
 *
 ******************************************************************************/

Knight::Knight(Color c, Board * b) :
	Piece(c, PIECE_VALUE_KNIGHT, b, ((c == WHITE ? 'N' : 'n'))) {
}

Knight::~Knight() {
}

void Knight::squareChanged(Square * s) {

	
	//cout << "squareChanged : " << name << " at " << square->getName() << " for " << s->getName() << endl; 
	
	
	if (s == square) {
		// Own square rang?
		//cout << "Own square changed (updating) : " << name << " at " << square->getName() << endl; 
		
		//update(); //?????? This is not enough

		return;
	}

	int32_t x2, y2;

	s->getXY(x2, y2);

	board->changeNMoves(color, -1 * getNumberOfMoves());
		
	if (s->getColorOfPiece() == color) {

		setBitOff64_2(canMoveToSquares, x2, y2);

	} else {

		setBitOn64_2(canMoveToSquares, x2, y2);

	}

	board->changeNMoves(color, getNumberOfMoves());

}

void Knight::updateToSquares(int32_t x, int32_t y) {

	int32_t xx, yy, dx, dy, signY;
	Square * s;

	for (dx = -2; dx <= 2; dx += 1) {

		if (!dx)
			continue;

		for (signY = -1; signY <= 1; signY += 2) {

			dy = (3 - abs(dx)) * signY;

			xx = x + dx; yy = y + dy;

			s = board->getSquare(xx, yy);

			if (!s) {
				/* Edge of board reached */
				continue;
			}

			s->addInterestFor(this);
			setBitOn64_2(interestedSquares, xx, yy);

			s->addAttacker(this);
			setBitOn64_2(attackedSquares, xx, yy);

			if (s->getColorOfPiece() != color) {
				setBitOn64_2(canMoveToSquares, xx, yy);
			}

		}
	}
}





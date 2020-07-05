/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/king.cpp,v 1.24 2003/04/29 08:01:56 fb Exp $
 *
 * Rivisions:
 * 	$Log: king.cpp,v $
 * 	Revision 1.24  2003/04/29 08:01:56  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 *
 * 	Revision 1.22  2003/04/22 15:54:51  fb
 * 	made all the functions in utils.h macros and put in macros.h
 *
 * 	Revision 1.19  2003/04/17 11:23:44  fb
 * 	improved the evaluation function and functions in the utils.cpp file
 *
 * 	Revision 1.14  2003/04/07 16:36:57  fb
 * 	Changed the list container to a set container
 *
 * 	Revision 1.13  2003/04/07 14:22:20  fb
 * 	Improved the alphaBeta function and trying to improve the code
 *
 * 	Revision 1.12  2003/03/25 16:59:54  fb
 * 	Trying to locate the getId() 's and eliminate them.
 *
 * 	Revision 1.11  2003/03/25 13:11:09  fb
 * 	Compilable under linux too
 *
 * 	Revision 1.9  2003/03/21 19:43:42  fb
 * 	Trying to accelerate the machine!
 *
 * 	Revision 1.8  2003/03/21 09:19:34  fb
 * 	Added the alpha beta prunning algorithm
 *
 *
 *
 *
 */

#include "king.h"
#include "moveinfo.h"

/*******************************************************************************
 *
 ******************************************************************************/

King::King(Color c, Board * b) :

	Piece(c, PIECE_VALUE_KING, b, ((c == WHITE ? 'K' : 'k'))) {

	checker1 = 0;
	checker2 = 0;

	canCastleBits = 3;

	castled = false;
}

King::~King() {
}

void King::squareChanged(Square * s) {
	//Do nothing as the kings are updated after each move
}

void King::updateToSquares(int32_t x, int32_t y) {

	int32_t xx, yy, signX, signY;
	Square * s;

	for (signX = -1; signX <= 1; signX += 1) {
		for (signY = -1; signY <= 1; signY += 1) {
			if (!signX && !signY) {
				/* If both are zero then continue */
				continue;
			}
			xx = x + signX; yy = y + signY;
			s = board->getSquare(xx, yy);
			if (!s) {
				/* Edge of board reached */
				continue;
			}

			s->addInterestFor(this); /* have to add interest because of the opposite king */
			setBitOn64_2(interestedSquares, xx, yy);

			s->addAttacker(this);
			setBitOn64_2(attackedSquares, xx, yy);

			if (s->getColorOfPiece() != color && s->isSafeForKing(color)) {
				setBitOn64_2(canMoveToSquares, xx, yy);
			}
		}
	}

	/* Look if rookade is possible */
	if (hasMoved || checker1) {
		return;
	}

	/* if (x != 4 || y != (color == WHITE ? 0 : 7)) {
		return;	Not necessary to check this because it has not moved yet.
	} */

	/* Pieces rook, night, bishop, queen, king */
	Rook * rp;
	Piece * np;
	Piece * bp;
	Piece * qp;

	Square * r1 /*rook*/, * n1 /* knight*/, * b1 /*bishop*/, * q1 /*queen*/ /* * k1  king */;
	int32_t side;
	for (side = 0 /*queen side*/; side <= 1 /*king side*/; side++) {
		/* 01234567
		   RNBQKBNR */
		r1 = board->getSquare(side * 7 /* 0Q, 7K */, color * 7 /* 0W, 7B*/);

		int32_t n1x = (1 + side * 5); /* 1Q, 6K*/
		int32_t n1y = color * 7;
		n1 = board->getSquare(n1x, n1y);

		int32_t b1x = (2 + side * 3); /* 2Q, 5K*/
		int32_t b1y = color * 7;
		b1 = board->getSquare(b1x, b1y);

		q1 = board->getSquare(3, color * 7);

		rp = (Rook *) (r1->getPiece());
		np = (n1->getPiece());
		bp = (b1->getPiece());
		qp = (q1->getPiece());

		/* Look if the rook is on it's original place */
		if (np || bp || !rp || !(rp->isEngagedInCastling()) || rp->getHasMoved())
			/* (There are pieces  between the king and the rook) or (No rook or is moved) */
			continue;

		if (!side) {

			if (!qp && q1->isSafeForKing(color) && b1->isSafeForKing(color)) {
				/* It is not necessary to book interest for this square
				   As the kings are updated every move seperately
				   And as the king can not kill a piece in this square */
				setBitOn64_2(canMoveToSquares, b1x, b1y);
			}

		} else if(n1->isSafeForKing(color) && b1->isSafeForKing(color)) {
			/* See the above comment */
			setBitOn64_2(canMoveToSquares, n1x, n1y);
		}
	}
}

void King::handleCheck() {

	if (!checker1) {
		return;
	}

	SomeSquares handicapSquares = 0;

	if (checker2) {
		board->allPiecesHandicap(color, handicapSquares); /* Empty handicapSquares for all except king */

	} else if (checker1->canJump()) {

		/* If this is a knight other pieces can only kill it */

		int32_t x, y;

		checker1->getSquare()->getXY(x, y);

		setBitOn64_2(handicapSquares, x, y);

		board->allPiecesHandicap(color, handicapSquares);

	} else if (checker1->canPromote()) {

		/* If this is a pown other pieces can only kill it .
		However , If it has caused an enpassant then it can be killed on another square */

		int32_t x, y;

		checker1->getSquare()->getXY(x, y);

		setBitOn64_2(handicapSquares, x, y);

		Square * pasSqu = board->getPassantSquare(opponentColor);

		if (pasSqu) {

			//Not powns can kill this checker only on it's square

			board->allNotPownsHandicap(color, handicapSquares);

			pasSqu->getXY(x, y);

			setBitOn64_2(handicapSquares, x, y);

			//Only powns can kill this checker on the en passant square and on it's own square

			board->allPownsHandicap(color, handicapSquares);

		} else {

			board->allPiecesHandicap(color, handicapSquares);
		}

	} else {

		/* If it is not a knight and not a pown then other pieces can kill it or block the check */

		board->getSquares(square, checker1->getSquare(), handicapSquares);

		board->allPiecesHandicap(color, handicapSquares);

	}
}

void King::lookForPinnedAndCheckers() {

	clearCheck();

	board->freeHandicaps(color);

	int32_t x, y, xx, yy, i, signX, signY;

	Square * s;
	Piece * p;

	x = square->x;
	y = square->y;

	Piece * pinned;
	Piece * by;

	for (signX = -1; signX <= 1; signX += 1) {
		for (signY = -1; signY <= 1; signY += 1) {

			if (!signX && !signY) {
				//If both are zero then continue
				continue;
			}

			pinned = 0;
			i = 0;

			while (true) {

				i++;
				xx = x + i * signX; yy = y + i * signY;
				s = board->getSquare(xx, yy);
				if (!s) {
					/* Edge of board reached */
					break;
				}

				p = s->getPiece();

				if (!p) { /* Empty square */
					continue;
				}

				if (p->getColor() != color) { /* Opponent piece */
					/* If we haven't yet seen owr own color then break this direction
					   This is a check? */
					if (!pinned) {
						if (p->attacksAKingAt(square)) {
							setChecker(p);
						}

						break;
					}

					/* We have now seen an opponent behind the pinned piece
					   Does it attack our eventually pinned piece?
					   And if it is a pown or a king then it can not pin it */

					if (p->canPromote() || p->castles() || !pinned->getSquare()->isInterestedBy(p)) {
						break;
					}

					by = p;
					pinned->makePinnedBy(by);

					break;

				} else {
					/* Our own color piece */
					if (pinned) {
						/* This is our second piece, break this direction */
						break;
					} else {
						pinned = p;
					}
				}
 			}
		}
	}

	int32_t dx, dy;

	/* Look for a knight checker */

	for (dx = -2; dx <= 2; dx += 1) {

		if (!dx)
			continue;

		for (signY = -1; signY <= 1; signY += 2) {

			dy = (3 - abs(dx)) * signY;

			s = board->getSquare(x + dx, y + dy);

			if (!s) {
				/* Edge of board reached */
				continue;
			}

			p = s->getPiece();

			/* is it a knight of the opponent? */
			if (p && p->canJump() && p->getColor() != color) {
				/* This is an opponent knight */
				setChecker(p);
			}
		}
	}

	handleCheck();

}

void King::do_move(MoveInfo & situ) {

	situ.b->saveSituation(situ);

	checker1 = 0;
	checker2 = 0;

	assert (situ.col == opponent(situ.opponentColor));
	assert (situ.mover == this);

	if (!hasMoved) {
		hasMoved = true;
		canCastleBits = 0;
	}

	/* If castling move the rook */
	int32_t x1, y1, x2, y2, x3, x4;

	situ.s1->getXY(x1, y1);
	situ.s2->getXY(x2, y2);

	if (abs(x1 - x2) == 2) {
		/*It is castling

		x2 == 6 or x2 == 2. y remains y "kings coordinates" the will move later in Piece->move()
		x3 is the old rook x */
		x3 = x2 == 6 ? 7 : 0; /* ( 7 / 4) * x2 - (14 /4); */
		/* x4 is the new rook x */
		x4 = x2 == 6 ? 5 : 3; /* (1 / 2) * x2 + 2; */
		Square * oldS = situ.b->getSquare(x3, y1); /* old rook square */
		assert(oldS);

		Piece * rook = oldS->getPiece();
		assert(rook);
		assert(rook->getHasMoved() == false);

		Square * newS = situ.b->getSquare(x4, y1); /* new rook square */
		assert(newS);

		/*situ.b->removePiece(rook);
		situ.b->addPiece(rook, newS);*/

		oldS->setPiece(0);
		rook->setSquare(newS);
		newS->setPiece(rook);

		rook->setHasMoved(true);

		castled = true;
		situ.s3 = oldS;
		situ.s4 = newS;
	}

	/* Do the actual move */

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

	assert(square);
}

Piece * King::undo_move(MoveInfo & situ) {

	assert (situ.col == opponent(situ.opponentColor));
	assert (situ.mover == this);

	/* Do the actual move back */
	situ.s2->setPiece(0);
	square = situ.s1;
	situ.s1->setPiece(this);

	/* If castling move the rook back */

	if (situ.s3) {
		assert (situ.s4);
		/* It is castling */

		Piece * rook = situ.s4->getPiece();
		assert (rook);

		/*situ.b->removePiece(rook);
		situ.b->addPiece(rook, situ.s3);*/
		situ.s4->setPiece(0);
		situ.s3->setPiece(rook);
		rook->setSquare(situ.s3);

		rook->setHasMoved(false);
		castled = false;
	}

	Piece * p = situ.b->restoreSituation(situ);
	assert (!p);

	situ.b->updateEffectedPiecesByUndo(situ);

	assert(square);

	return p;
}





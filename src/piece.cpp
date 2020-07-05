/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/piece.cpp,v 1.19 2003/04/29 08:01:58 fb Exp $
 *
 * Rivisions:
 * 	$Log: piece.cpp,v $
 * 	Revision 1.19  2003/04/29 08:01:58  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 *
 * 	Revision 1.17  2003/04/22 15:54:50  fb
 * 	made all the functions in utils.h macros and put in macros.h
 *
 * 	Revision 1.14  2003/04/17 11:23:43  fb
 * 	improved the evaluation function and functions in the utils.cpp file
 *
 * 	Revision 1.9  2003/03/25 13:11:08  fb
 * 	Compilable under linux too
 *
 * 	Revision 1.7  2003/03/21 19:43:43  fb
 * 	Trying to accelerate the machine!
 *
 * 	Revision 1.6  2003/03/21 09:19:33  fb
 * 	Added the alpha beta prunning algorithm
 *
 *
 *
 *
 */

#include "piece.h"
#include "square.h"
#include "moveinfo.h"
#include "board.h"
#include "king.h"

int32_t Piece::number[2] = {NO_UID, NO_UID};

/*******************************************************************************
 *
 ******************************************************************************/

Piece::Piece(Color c, int32_t v, Board * b, char n, int32_t arrayIdx) {

	value = v;
	name = n;
	power = 0;
	color = c;
	opponentColor = opponent(c);
	square = 0;
	handicapped = false;
	board = b;
	hasMoved = false;

	if (arrayIdx == NO_UID) {
		/* Uid automatically assigned */
		number[color]++;
		arrayIndex = number[color];
		assert(arrayIdx < MAX_PIECES_PER_SIDE);
	} else {
		/* Uid not automatically assigned */
		assert(arrayIdx < MAX_PIECES_PER_SIDE);
		arrayIndex = arrayIdx;
	}

	canMoveToSquares = 0;
	interestedSquares = 0;
	attackedSquares = 0;
}

Piece::~Piece() {
}

void Piece::squareChanged(Square * s) {

	/* This will handle a : Queen, Rook, Bishop, Knight. The other pieces have to override
		this method. They are : King, Pown */

	/* This method have to do the following :

		1. Start from the squares s towards the edge of the board.

		2. Add this square to interests and attackers

		3. If the color of the piece at this square is the same as this piece then
			a. Remove the attack and the interest of this piece to all of the rest of the squares
			b. return

		4. If there is no piece here then continue.

		5. If there is a piece of opponent in this square then:
			a. If it is a king then count for one extra square and add only interest to that square

		6. 	a. Remove the attack and the interest of this piece to all of the rest of the squares
			b. return



	*/

	//cout << "squareChanged : " << name << " at " << square->getName() << " for " << s->getName() << endl;

	if (s == square) {
		// Own square rang?
		//cout << "Own square changed (updating) : " << name << " at " << square->getName() << endl;

		//update(); //?????? This is not enough

		return;
	}

	int32_t x1, y1, x2, y2, dx, dy;

	square->getXY(x1, y1);

	s->getXY(x2, y2);

	dx = SIGN(x2 - x1);
	dy = SIGN(y2 - y1);

#if defined _DEBUG

	assert(abs(dx) < 2 && abs(dy) < 2);

	if (isEngagedInCastling()) {
		if (dx == dy) {
			cout << *board;
			cout << " The rook at " << square->getName() << " was interested in " << s->getName() << endl;
		}
		assert (dx != dy);
		assert (abs(dx) == 1 || abs(dy) == 1);
		assert (dx == 0 || dy == 0);
	} else if(name == 'B' || name == 'b') {
		assert (abs(dx) == 1 && abs(dy) == 1);
	}

#endif


	//The rang square is interested and attacked. this will not change.
	//Only canMoveTo changes

	board->changeNMoves(color, -1 * getNumberOfMoves());

	//If this is a knight then look at this square and return here.

	bool oneSquareExtra = false;
	bool clearTheRest = false;

	if (s->getColorOfPiece() == color) {

		//cout << " Can not move to it \n";
		setBitOff64_2(canMoveToSquares, x2, y2);

		clearTheRest = true;

	} else {
		//cout << " Can move to it \n";
		setBitOn64_2(canMoveToSquares, x2, y2);

		Piece * p = s->getPiece();
		if (p) {
			if (p == (Piece *) board->getKing(opponentColor)) {
				oneSquareExtra = true;
			} else {
				clearTheRest = true;
			}
		}

	}


	while (true) {

		x2 += dx, y2 += dy;

	//for (; ((unsigned) x2 < BOARD_SIZE) && ((unsigned) y2 < BOARD_SIZE); x2 += dx, y2 += dy) {

		s = board->getSquare(x2, y2);

		//cout << "   looking at " << (s ? s->getName() : " edge ") << endl;

		if (!s) {
			//cout << " s == 0 ";
			//Edge of board reached
			board->changeNMoves(color, getNumberOfMoves());
			return;
		}

		if (clearTheRest) {

			if (!bitIsOn64_2(interestedSquares, x2, y2)) {

				//cout << "   I was not interested in this and further.\n";

				board->changeNMoves(color, getNumberOfMoves());

				return;
			}

			//Clear the rest of the squares until the edge

			s->removeInterestOf(this);
			setBitOff64_2(interestedSquares, x2, y2);

			s->removeAttacker(this);
			setBitOff64_2(attackedSquares, x2, y2);

			setBitOff64_2(canMoveToSquares, x2, y2);

		} else if (oneSquareExtra) {

			//Add interest to this square and clear all the rest
			s->addInterestFor(this);
			setBitOn64_2(interestedSquares, x2, y2);

			s->removeAttacker(this);
			setBitOff64_2(attackedSquares, x2, y2);

			setBitOff64_2(canMoveToSquares, x2, y2);

			clearTheRest = true;

		} else {

			//Normal continuation
			//Add interest and attacker. and if there is a piece of opponent
			//canMoveTo this too
			//If you encounter the king of the opponent then set oneSquareExtra to true
			//If any other piece is seen then set ClearTheRest to true

			s->addInterestFor(this);
			setBitOn64_2(interestedSquares, x2, y2);

			s->addAttacker(this);
			setBitOn64_2(attackedSquares, x2, y2);

			if (s->getColorOfPiece() != color) {

				/* This square is empty */
				setBitOn64_2(canMoveToSquares, x2, y2);

			} else {

				setBitOff64_2(canMoveToSquares, x2, y2);

			}

			Piece * king = s->getPiece();

			if (king) {
				if (king == (Piece *) board->getKing(opponentColor)) {
					oneSquareExtra = true;
				} else {
					/* Other piece than king is reached */
					clearTheRest = true;
				}
			}

		}
	}

	board->changeNMoves(color, getNumberOfMoves());
}

bool Piece::attacksAKingAt(Square * s) {
	if (!s->isInterestedBy(this))
		return false;

	if (!canPromote())
		return true;

	/* A pown */
	return (s->x != square->x);
}

Piece * Piece::undo_move(MoveInfo & situ) {

	assert (situ.col == opponent(situ.opponentColor));

	/* This will be a normal piece who will move to the other square */

	situ.s2->setPiece(0);
	square = situ.s1;
	situ.s1->setPiece(this);

	Piece * p = situ.b->restoreSituation(situ);

	situ.b->updateEffectedPiecesByUndo(situ);

	return p;
}

void Piece::do_move(MoveInfo & situ) {

	/* If a pown was promoted then don't call it again */
	if (situ.mover == this) {
		/* This is a normal piece and not a pown that has changed to a quuen
		   Otherwise the mover is a pown and the piece at s1 is a queen
		   The piece at s1->do_move is then called and the above condition will be false */
		situ.b->saveSituation(situ);
	}

	assert (situ.col == opponent(situ.opponentColor));

	/* This will be a normal piece who will move to the other square */

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

void Piece::makePinnedBy(Piece * pinner) {


	SomeSquares allowedSquares = 0;

	board->getSquares(board->getKing(color)->getSquare(), pinner->getSquare(), allowedSquares);

	/* Don't need to remove my square because it is not one of my moves */

	makeHandicap(allowedSquares);

	return;
}

void Piece::makeHandicap(SomeSquares & squaresBetweenKingAndPinner) {

	if (castles())
		return; /* A king cannot be handicapped. but it is updated after it ???? */

	handicapped = true;

	removeAllAttacks();

	int32_t numberBits;

	bitCount64(canMoveToSquares, numberBits);
	board->changeNMoves(color, -numberBits);

	canMoveToSquares &= squaresBetweenKingAndPinner;

	bitCount64(canMoveToSquares, numberBits);
	board->changeNMoves(color, numberBits);

	/* Add the attackers back to the squares this handicap stil can move to */

	SomeSquares cmt = canMoveToSquares;

	uint32_t bitn;

	while (cmt) {

		firstBitSet64(cmt, bitn); //It does not shift it left now. but sets the bit off

		/* bitn is from 1 to 64 so subtract one */

		int32_t i = (bitn - 1) % BOARD_SIZE;
		int32_t j = (bitn - 1) / BOARD_SIZE;

		Square * s = board->getSquare (i, j);

		s->addAttacker(this);
		setBitOn64_2(attackedSquares, i, j);
	}

}

void Piece::update() {

	//cout << " updating " << name << " at " << square->getName() << endl;

	handicapped = false;

	board->changeNMoves(color, -1 * getNumberOfMoves());

	removeAllAttacks();
	removeAllInterests();
	canMoveToSquares = 0;

	if (power != 0) {
		cout << name << " : Power = " << power << "\t";
	}

	/* Call the real updater */
	updateToSquares(square->x, square->y);

	board->changeNMoves(color, getNumberOfMoves());

}

void Piece::removeAllInterests() {

	uint32_t bitn;

	while (interestedSquares) {

		firstBitSet64(interestedSquares, bitn); //It does not shift it left now. but sets the bit off

		/* bitn is from 1 to 64 so subtract one */

		int32_t i = (bitn - 1) % BOARD_SIZE;
		int32_t j = (bitn - 1) / BOARD_SIZE;

		board->getSquare(i, j)->removeInterestOf(this);
	}
}

void Piece::removeAllAttacks() {
	uint32_t bitn;

	while (attackedSquares) {

		firstBitSet64(attackedSquares, bitn); //It does not shift it left now. but sets the bit off

		/* bitn is from 1 to 64 so subtract one */

		int32_t i = (bitn - 1) % BOARD_SIZE;
		int32_t j = (bitn - 1) / BOARD_SIZE;

		board->getSquare(i, j)->removeAttacker(this);
	}
}

void Piece::addInterestFor(Square * s) {
	setBitOn64_2(interestedSquares, s->x, s->y);
}



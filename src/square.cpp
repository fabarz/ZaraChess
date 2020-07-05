/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/square.cpp,v 1.23 2003/04/29 08:01:59 fb Exp $
 *
 * Rivisions:
 * 	$Log: square.cpp,v $
 * 	Revision 1.23  2003/04/29 08:01:59  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 * 	
 * 	Revision 1.21  2003/04/22 15:54:48  fb
 * 	made all the functions in utils.h macros and put in macros.h
 * 	
 * 	Revision 1.18  2003/04/17 11:23:42  fb
 * 	improved the evaluation function and functions in the utils.cpp file
 *
 * 	Revision 1.13  2003/04/07 16:36:56  fb
 * 	Changed the list container to a set container
 *
 * 	Revision 1.12  2003/03/25 16:59:55  fb
 * 	Trying to locate the getId() 's and eliminate them.
 *
 * 	Revision 1.11  2003/03/25 13:11:07  fb
 * 	Compilable under linux too
 *
 * 	Revision 1.9  2003/03/21 19:43:43  fb
 * 	Trying to accelerate the machine!
 *
 * 	Revision 1.8  2003/03/21 09:19:33  fb
 * 	Added the alpha beta prunning algorithm
 *
 *
 *
 *
 */


#include "chess.h"
#include "square.h"
#include "board.h"
#include "macros.h"

static const int32_t squareValues[8][8] = {
	{1,2,3,3,3,3,2,1},
	{2,3,4,4,4,4,3,2},
	{3,4,5,6,6,5,4,3},
	{3,4,6,7,7,6,4,3},
	{3,4,6,7,7,6,4,3},
	{3,4,5,6,6,5,4,3},
	{2,3,4,4,4,4,3,2},
	{1,2,3,3,3,3,2,1}
};

const char * squareNames[8][8] = {
	{"a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8"},
	{"b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8"},
	{"c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8"},
	{"d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8"},
	{"e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8"},
	{"f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8"},
	{"g1", "g2", "g3", "g4", "g5", "g6", "g7", "g8"},
	{"h1", "h2", "h3", "h4", "h5", "h6", "h7", "h8"}
};



/*******************************************************************************
 *
 ******************************************************************************/

Square::Square(int32_t _x, int32_t _y, Board * b) : name (squareNames[_x][_y]) {

	x = _x;
	y = _y;
	piece = 0;
	board = b;

	interestedPieces[0] = 0;
	interestedPieces[1] = 0;
	attackers[0] = 0;
	attackers[1] = 0;


	value = squareValues[_x][_y];
}


Square::~Square() {
}

void Square::setPiece(Piece * p) {

	if (piece && piece->canPromote()) {
		//A pown has left
		board->pownOnX[piece->getColor()][x]--;
		board->pownOnY[piece->getColor()][y]--;
	}

	if (p && p->canPromote()) {
		//A pown is coming
		board->pownOnX[p->getColor()][x]++;
		board->pownOnY[p->getColor()][y]++;
	}

	piece = p;
}


void Square::ringPieces() {

	for (int c = 0; c < 2; c++) {
	
		int32_t copyOfInteresters = interestedPieces[c];
		
		while (copyOfInteresters) {
			
			int bitnr;
			
			firstBitSet16(copyOfInteresters, bitnr);
			
			Piece * p = board->getPiece(Color(c), bitnr - 1);

			assert (p);

			p->squareChanged(this);
		}
	}
}

bool Square::isInterestedBy(Piece * p) {
	return bitIsOn16(interestedPieces[p->getColor()], p->getArrayIndex());
}

bool Square::isAttackedBy (Piece * p) {
	return bitIsOn16(attackers[p->getColor()], p->getArrayIndex());
}

bool Square::isSafeForKing (Color c) {

	Color opponentC = opponent(c);

	//Any attackers then it is not safe
	if (attackers[opponentC]) {
		return false;
	}

	int32_t bitn;

	uint16_t copyInts = interestedPieces[opponentC];

	while (copyInts) {

		firstBitSet16 (copyInts, bitn); //It does not shif left now. but sets the bit off.

		assert ((bitn - 1 >= 0) && (bitn - 1 < MAX_PIECES_PER_SIDE));

		Piece * p = board->getPiece(opponentC, bitn - 1);

		assert (p);


		if (!p->canPromote()) {
			return false;
		} else {
			if (p->getSquare()->x != x) {
				return false;
			}
		}
	}

	return true;
}

int32_t Square::attackedByLessThan(Color c, int32_t v) {

	int32_t bitn;

	uint16_t copyAttackers = attackers[c];

	int32_t lessAttacker = v;

	while (copyAttackers) {

		firstBitSet16 (copyAttackers, bitn);

		assert ((bitn - 1 >= 0) && (bitn - 1 < MAX_PIECES_PER_SIDE));

		Piece * p = board->getPiece(c, bitn - 1);

		assert (p);

		if (p->getValue() < lessAttacker) {
			lessAttacker = p->getValue();
		}
	} 

	return v - lessAttacker;
}

int32_t Square::opponentHasMoreAttackers(Color c) {

	/* Color is defender and opponent is attacker */

	Color opponentColor = opponent(c);

	int32_t cvTotal = 0;
	uint32_t cnTotal = 0;
	int32_t lessDefender = CHECKMATE;

	int32_t v;

	int32_t ovTotal = 0;
	uint32_t onTotal = 0;
	int32_t lessAttacker = CHECKMATE;

	int32_t bitn;

	uint32_t copyAttackers = attackers[opponentColor];

	while (copyAttackers) {

		firstBitSet16 (copyAttackers, bitn); //It does not shif left now. but sets the bit off.

		Piece * p = board->getPiece(opponentColor, bitn - 1);

		assert(p);
		
		v = p->getValue();
		ovTotal += v;
		onTotal++;
		if (v < lessAttacker) {
			lessAttacker = v;
		}
	}

	if (onTotal == 0) {
		//No attackers
		return 0;
	}

	copyAttackers = attackers[c];	

	while (copyAttackers) {

		firstBitSet16 (copyAttackers, bitn); //It does not shif left now. but sets the bit off.

		Piece * p = board->getPiece(c, bitn - 1);

		assert(p);

		v = p->getValue();

		cvTotal += v;
		cnTotal++;
		if (v < lessDefender) {
			lessDefender = v;
		}
	}

	v = piece->getValue();

	if (cnTotal == 0) {
		//No defenders
		return v;
	}
	
	if (onTotal == cnTotal) {
		//same number of defenders and attackers
		if (onTotal == 1) {
			if (lessAttacker < v) {
				return v - lessAttacker;
			} else {
				return 0;
			}
		} else if (onTotal == 2) {
			if (lessAttacker < v) {
				return v - lessAttacker;
			} else if (ovTotal < v + lessAttacker) {
				return v + lessAttacker - ovTotal;
			} else {
				return 0;
			}
		} else {
			//More than 2 from both sides
			return 0; //Too complicated
		}
	} else if (onTotal < cnTotal) {
		//More defenders
		if (lessAttacker < v) {
			return v - lessAttacker;
		} else {
			return 0;
		}
	} else { //if (onTotal > cnTotal) {
		
		//Opponent has more attackers
		//He will pay one piece and take two pieces
			//His piece		//our two pieces
		if (lessAttacker < (lessDefender + v)) {

			return v;

		} else {
			
			return 0;

		}
	}

}










/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/square.h,v 1.23 2003/04/29 08:02:00 fb Exp $
 *
 * Rivisions:
 * 	$Log: square.h,v $
 * 	Revision 1.23  2003/04/29 08:02:00  fb
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
 * 	Revision 1.12  2003/04/07 14:22:21  fb
 * 	Improved the alphaBeta function and trying to improve the code
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

#if !defined SQUARE_H_INCLUDED
#define SQUARE_H_INCLUDED

#include <algorithm>

#include "piece.h"
#include <string>

class Board;
class ChessGame;
class Move;


/*******************************************************************************
 *
 ******************************************************************************/

ostream & operator << (ostream & os, Board & cg);

class Square {

	public :

	Square(int32_t _x, int32_t _y, Board * b);

	friend class King; /* Can access x and y */
	friend class Piece; /* Can access x and y */

	friend int32_t alphaBeta(ChessGame * cg, int32_t depth, int32_t alpha, int32_t beta, Move & bestmove);

	~Square();

	string & getName() {return name;}

	void getXY(int32_t & _x, int32_t & _y) { _x = x; _y = y; }

	Piece * getPiece() {return piece; }

	void setPiece(Piece * p);

	void addInterestFor(Piece * p) {
		if (bitIsOn16(interestedPieces[p->getColor()], p->getArrayIndex())) {
			
			//This is only necessary because a checker interests in one extra square
			//When the king moves, the checker tries to book interest for the same square again
			//because the extra square lies behind the king. ????

			//cout << *board;
			//cout << p->getName() << " is already interested in " << name << endl;
			//cin.ignore();
		} else {
			p->changePower(value);		
			setBitOn16(interestedPieces[p->getColor()], p->getArrayIndex());
		}
	}

	void removeInterestOf(Piece * p) {		
		if (!bitIsOn16(interestedPieces[p->getColor()], p->getArrayIndex())) {
			
			//See above.

			//cout << *board;
			//cout << p->getName() << " is not interested in " << name << endl;
			//cin.ignore();
		} else {
			p->changePower(-value);
			setBitOff16(interestedPieces[p->getColor()], p->getArrayIndex());
		}
	}

	uint32_t getInterested() {

		//Reset all the interested ones and return them.

		uint32_t result = interestedPieces[0];
		uint16_t * resP = ((uint16_t *) &result) + 1;
		*resP = interestedPieces[1];

		interestedPieces[0] = 0;
		interestedPieces[1] = 0;

		return result;
	}

	void ringPieces();

	Color getColorOfPiece() {
		return (piece ? piece->getColor() : NO_COLOR);
	}

	bool isSafeForKing (Color c);

	bool isInterestedBy (Piece * p);

	void addAttacker(Piece * p) {
		setBitOn16(attackers[p->getColor()], p->getArrayIndex());
	}

	void removeAttacker(Piece * p) {
		setBitOff16(attackers[p->getColor()], p->getArrayIndex());
	}

	bool isAttackedBy (Piece * p);

	bool isAttackedBy (Color c) {
		return attackers[c] != 0;
	}

	int32_t attackedByLessThan(Color c, int32_t v); /* Return 0 if not other wise (big - small) */

	int32_t opponentHasMoreAttackers(Color c); /* Return 0 if not otherwise total difference */

	private:

	int32_t x, y;

	Piece * piece;
	Board * board;

	uint16_t interestedPieces[2]; /* Interested pieces bits */
	uint16_t attackers[2];

	string name;

	int32_t value;

};


#endif





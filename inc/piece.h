/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/piece.h,v 1.19 2003/04/29 08:01:58 fb Exp $
 *
 * Rivisions:
 * 	$Log: piece.h,v $
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

#if !defined PIECE_H_INCLUDED
#define PIECE_H_INCLUDED

#include "chess.h"
#include "macros.h"

class MoveInfo;
class Square;
class ChessGame;
class Board;

/*******************************************************************************
 *
 ******************************************************************************/

class Piece {

	public :

	Piece (Color c, int32_t _v, Board * b, char n, int32_t arrayIdx = NO_UID);
	virtual ~Piece();

	char getName() {return name;}
	int32_t getValue() {return value;}

	Square * getSquare() { return square; }
	void setSquare(Square * s) { square = s; }

	bool getHasMoved() { return hasMoved; }
	void setHasMoved(bool m) { hasMoved = m; }
	bool getHandicapped() { return handicapped; }
	void resetHandicapped() { handicapped = false; }

	void removeAllInterests();
	void removeAllAttacks();

	void addInterestFor(Square * s);

	Color getColor() {
		return color;
	}

	int32_t getPower() { return power; }
	void changePower(int32_t v) {	power += v; }

	SomeSquares & getCanMoveToSquares() { return canMoveToSquares; }
	int32_t getNumberOfMoves() { 
		int32_t res;
		bitCount64(canMoveToSquares, res);
		return res;
	}

	void update();

	//To do: Use this instead of board->call(int32_t Pieces) ?????

	virtual void squareChanged(Square * s);

	void makeHandicap(SomeSquares & squaresBetweenKingAndPinner);

	virtual void do_move(MoveInfo & situ);
	virtual Piece * undo_move(MoveInfo & situ);

	bool attacksAKingAt(Square * s);

	void makePinnedBy(Piece * pinner);

	virtual bool canPromote() {return false;}
	virtual bool castles() {return false;}
	virtual bool isEngagedInCastling() {return false;}
	virtual bool canJump() {return false;}

	private :

	virtual void updateToSquares(int32_t x, int32_t y) = 0;

	public :

	int32_t getArrayIndex() {
		return arrayIndex;
	}

	void setArrayIndex(int32_t arx) {
		arrayIndex = arx;
	}

	static int32_t number[2];

	protected :

	Color color;
	Color opponentColor;

	SomeSquares canMoveToSquares;
	SomeSquares interestedSquares;
	SomeSquares attackedSquares;

	bool hasMoved;
	bool handicapped;
	int32_t power;

	Board * board;
	Square * square;

	char name;
	int32_t value;

	int32_t arrayIndex;

};


#endif




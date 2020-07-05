/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/king.h,v 1.18 2003/04/29 08:01:56 fb Exp $
 *
 * Rivisions:
 * 	$Log: king.h,v $
 * 	Revision 1.18  2003/04/29 08:01:56  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 * 	
 * 	Revision 1.15  2003/04/22 15:54:51  fb
 * 	made all the functions in utils.h macros and put in macros.h
 * 	
 * 	Revision 1.13  2003/04/17 11:23:44  fb
 * 	improved the evaluation function and functions in the utils.cpp file
 *
 * 	Revision 1.8  2003/03/25 13:11:09  fb
 * 	Compilable under linux too
 *
 * 	Revision 1.7  2003/03/21 19:43:42  fb
 * 	Trying to accelerate the machine!
 *
 * 	Revision 1.6  2003/03/21 09:19:34  fb
 * 	Added the alpha beta prunning algorithm
 *
 *
 *
 *
 */

#if !defined KING_H_INCLUDED
#define KING_H_INCLUDED

#include "piece.h"
#include "rook.h"
#include "board.h"

/*******************************************************************************
 *
 ******************************************************************************/

class King : public Piece {

	public :

	Piece * checker1;
	Piece * checker2;

	King(Color c, Board * b);
	virtual ~King();

	void squareChanged(Square * s);

	void lookForPinnedAndCheckers();

	void do_move(MoveInfo & situ);
	Piece * undo_move(MoveInfo & situ);

	bool isCheck() { return checker1 != 0; }

	void setChecker(Piece * p) {
		if (checker1 == p || checker2 == p)
			return;
		if (!checker1) {
			checker1 = p;
			return;
		}
		if (!checker2) {
			checker2 = p;
			return;
		}

		cout << " --------- Too many checkers! ---------\n";
		cout << " 1 : " << checker1->getName();
		cout << " 2 : " << checker2->getName();
		cout << " 3 : " << p->getName() << endl;
		cin.ignore();
	}

	void clearCheck() {checker1 = 0; checker2 = 0;}

	void handleCheck();

	bool getHasMoved() { return hasMoved; }

	bool castles() {return true;}

	int32_t canCastleBits;	

	bool getCastled() {
		return castled;
	}

	void setCastled(bool cas) {
		castled = cas;
	}

	private :

	void updateToSquares(int32_t x, int32_t y);

	bool castled;

};


#endif




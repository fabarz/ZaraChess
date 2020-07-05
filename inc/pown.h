/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/pown.h,v 1.18 2003/04/29 08:01:59 fb Exp $
 *
 * Rivisions:
 * 	$Log: pown.h,v $
 * 	Revision 1.18  2003/04/29 08:01:59  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 * 	
 * 	Revision 1.16  2003/04/22 15:54:49  fb
 * 	made all the functions in utils.h macros and put in macros.h
 * 	
 * 	Revision 1.14  2003/04/17 11:23:42  fb
 * 	improved the evaluation function and functions in the utils.cpp file
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

#if !defined POWN_H_INCLUDED
#define POWN_H_INCLUDED

#include "piece.h"

class situationbeforeMove;
class Board;


/*******************************************************************************
 *
 ******************************************************************************/

class Pown : public Piece {

	public :

	Pown(Color c, Board * b);
	virtual ~Pown();

	void squareChanged(Square * s);

	void do_move(MoveInfo & situ);
	Piece * undo_move(MoveInfo & situ);

	bool canPromote() {return true;}

	private :

	void updateToSquares(int32_t x, int32_t y);

};

#endif




/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/knight.h,v 1.16 2003/04/29 08:01:57 fb Exp $
 *
 * Rivisions:
 * 	$Log: knight.h,v $
 * 	Revision 1.16  2003/04/29 08:01:57  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 * 	
 * 	Revision 1.14  2003/04/22 15:54:51  fb
 * 	made all the functions in utils.h macros and put in macros.h
 * 	
 * 	Revision 1.12  2003/04/17 11:23:44  fb
 * 	improved the evaluation function and functions in the utils.cpp file
 *
 * 	Revision 1.7  2003/03/25 13:11:09  fb
 * 	Compilable under linux too
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

#if !defined KNIGHT_H_INCLUDED
#define KNIGHT_H_INCLUDED

#include "piece.h"

/*******************************************************************************
 *
 ******************************************************************************/

class Knight : public Piece {

	public :

	Knight(Color c, Board * b);
	virtual ~Knight();

	void squareChanged(Square * s);

	bool canJump() {return true;}

	private :

	void updateToSquares(int32_t x, int32_t y);

};

#endif







/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/bishop.h,v 1.17 2003/04/29 08:01:53 fb Exp $
 *
 * Rivisions:
 * 	$Log: bishop.h,v $
 * 	Revision 1.17  2003/04/29 08:01:53  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 * 	
 * 	Revision 1.14  2003/04/22 15:54:55  fb
 * 	made all the functions in utils.h macros and put in macros.h
 * 	
 * 	Revision 1.12  2003/04/17 11:23:47  fb
 * 	improved the evaluation function and functions in the utils.cpp file
 *
 * 	Revision 1.7  2003/03/25 13:11:12  fb
 * 	Compilable under linux too
 *
 * 	Revision 1.6  2003/03/21 19:43:39  fb
 * 	Trying to accelerate the machine!
 *
 * 	Revision 1.5  2003/03/21 09:19:36  fb
 * 	Added the alpha beta prunning algorithm
 *
 *
 *
 *
 */

#if !defined BISHOP_H_INCLUDED
#define BISHOP_H_INCLUDED

#include "piece.h"

/*******************************************************************************
 *
 ******************************************************************************/

class Bishop : public Piece {

	public :

	Bishop(Color c, Board * b);
	virtual ~Bishop();

	private :

	void updateToSquares(int32_t x, int32_t y);

};

#endif




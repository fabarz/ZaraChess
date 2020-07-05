/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/chess.h,v 1.30 2003/04/29 08:01:54 fb Exp $
 *
 * Rivisions:
 * 	$Log: chess.h,v $
 * 	Revision 1.30  2003/04/29 08:01:54  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 * 	
 * 	Revision 1.27  2003/04/22 15:54:53  fb
 * 	made all the functions in utils.h macros and put in macros.h
 * 	
 * 	Revision 1.24  2003/04/17 11:23:45  fb
 * 	improved the evaluation function and functions in the utils.cpp file
 * 	
 * 	Revision 1.19  2003/04/07 16:36:58  fb
 * 	Changed the list container to a set container
 *
 * 	Revision 1.18  2003/03/25 13:11:10  fb
 * 	Compilable under linux too
 *
 * 	Revision 1.15  2003/03/21 19:43:41  fb
 * 	Trying to accelerate the machine!
 *
 * 	Revision 1.14  2003/03/21 09:19:35  fb
 * 	Added the alpha beta prunning algorithm
 *
 *
 *
 *
 */

#if !defined CHESS_H_INCLUDED
#define CHESS_H_INCLUDED

#if defined WIN32
#pragma warning (disable:4786)
#else
#define __int64 long long
#endif



#include <list>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <stdlib.h>
#include <time.h>


using namespace std;

#define PIECE_VALUE_POWN	(int32_t)  10000
#define PIECE_VALUE_ROOK	(int32_t)  50000
#define PIECE_VALUE_KNIGHT	(int32_t)  30900
#define PIECE_VALUE_BISHOP	(int32_t)  31000
#define PIECE_VALUE_QUEEN	(int32_t)  99000
#define PIECE_VALUE_KING	(int32_t) 900000

#define BOARD_SIZE	8

#define CHECKMATE 2000000000
#define STALEMATE 0

enum Color {
	WHITE = 0, BLACK = 1, NO_COLOR = 2, BLOCKED = 4
};

#define opponent(color1) (color1 == WHITE ? BLACK : WHITE)

typedef unsigned __int64 SomeSquares;

#define Moves		list<Move>
#define Situations	list<Situation>

#define MAX_PIECES_PER_SIDE 16

#define NO_UID -1

#if defined WIN32

#define random rand
#define srandom srand

#endif


#endif


















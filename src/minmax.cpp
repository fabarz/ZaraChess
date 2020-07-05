/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/minmax.cpp,v 1.25 2003/04/29 08:19:32 fb Exp $
 *
 * Rivisions:
 * 	$Log: minmax.cpp,v $
 * 	Revision 1.25  2003/04/29 08:19:32  fb
 * 	Eliminated some warnings from g++ on linux
 * 	
 * 	Revision 1.24  2003/04/29 08:01:57  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 *
 * 	Revision 1.22  2003/04/22 15:54:50  fb
 * 	made all the functions in utils.h macros and put in macros.h
 *
 * 	Revision 1.20  2003/04/17 11:23:43  fb
 * 	improved the evaluation function and functions in the utils.cpp file
 *
 * 	Revision 1.15  2003/04/07 16:36:57  fb
 * 	Changed the list container to a set container
 *
 * 	Revision 1.14  2003/04/07 14:22:20  fb
 * 	Improved the alphaBeta function and trying to improve the code
 *
 * 	Revision 1.13  2003/03/25 16:59:55  fb
 * 	Trying to locate the getId() 's and eliminate them.
 *
 * 	Revision 1.12  2003/03/25 13:11:08  fb
 * 	Compilable under linux too
 *
 * 	Revision 1.10  2003/03/21 19:43:42  fb
 * 	Trying to accelerate the machine!
 *
 * 	Revision 1.9  2003/03/21 09:19:33  fb
 * 	Added the alpha beta prunning algorithm
 *
 *
 *
 *
 */


#if defined WIN32
#include <windows.h>
#endif

#include "minmax.h"
#include "chessgame.h"

extern uint32_t nodes;
extern int depth1;

int32_t halfRandom = (RAND_MAX / 2);

Move BESTMOVE;
Move levelUp;
static Move opponentsMove;
static Move my2Move;

int32_t alphaBeta(ChessGame * cg, int32_t depth, int32_t alpha, int32_t beta, Moves ** ppmvs) {

	if (cg->nMoves[cg->sideToMove] == 0) {
		if (cg->KINGS[cg->sideToMove]->isCheck()) return -CHECKMATE - depth;
		else return 0;
	}

	if (depth <= 0)	{
		return cg->evaluatePosition();
	}

	Moves * mvs = *(ppmvs + depth);

	mvs->clear();

	cg->getSortedLegalMoves(mvs);

	Moves::iterator mit = mvs->begin();

	int32_t eval;

	/*int32_t nMoves = mvs->size();

	if (nMoves == 1) {


		nodes += 1;
		//This makes it faster. but there is one problem
		//IT can not find the fastest mate. It is not clear why ????
		if (depth1 == depth)
			BESTMOVE = *mit;

		return -mit->value;

	}*/

	bool foundPvs = false;	

	Moves::iterator mendIt = mvs->end();

	while (mit != mendIt) {

		if(depth == depth1) {
			levelUp = *mit;
		} else {
			if (depth == depth1 - 1) {
				cout << "\r    " << levelUp << " " << *mit << " (Node " << (int) nodes << ") alpha = "
					<< alpha << " & beta = " << beta << "          ";
			}
		}

		nodes++;

		cg->makeMove(*mit);

		if (cg->repeated) {
			cg->undoMove();
			cg->repeated = false;
			if (depth == depth1) {
				levelUp.value = 0;
			}
			return 0;
		}

		if (foundPvs) {
			eval = -alphaBeta(cg, depth - 1, -alpha - 1, -alpha, ppmvs);

			if ((eval > alpha) && (eval < beta)) {/* Check for failure. */
				eval = -alphaBeta(cg, depth - 1, -beta, -alpha, ppmvs);
			}
		} else
			eval = -alphaBeta(cg, depth - 1, -beta, -alpha, ppmvs);

		cg->undoMove();

		if (eval >= beta) return beta;

		if (eval > alpha) {

			foundPvs = true;
			alpha = eval;

			if (depth == depth1) {

				mit->value = alpha;
				BESTMOVE = *mit;
				cout << "\r ==>> " << BESTMOVE << " - " << opponentsMove << " - " << my2Move
					<< " = ";

				if (abs(BESTMOVE.value) >= CHECKMATE) {
					cout << "CHECKMATE in " << depth1 - (abs(BESTMOVE.value) - CHECKMATE);
				} else {
					cout << BESTMOVE.value;
				}

				cout << "                                                       ";

			} else if (depth == depth1 - 1) {

				opponentsMove = *mit;

			} else if (depth == depth1 - 2) {

				my2Move = *mit;

			}

			if (eval == CHECKMATE + depth - 1) {
				//cout << " Direct mate "; 
				return eval;
			}
		}

		++mit;

	}

	return alpha;

}



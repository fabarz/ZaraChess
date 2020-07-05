/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/chessgame.h,v 1.22 2003/04/29 08:01:56 fb Exp $
 *
 * Rivisions:
 * 	$Log: chessgame.h,v $
 * 	Revision 1.22  2003/04/29 08:01:56  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 * 	
 * 	Revision 1.20  2003/04/22 15:54:52  fb
 * 	made all the functions in utils.h macros and put in macros.h
 * 	
 * 	Revision 1.17  2003/04/17 11:23:45  fb
 * 	improved the evaluation function and functions in the utils.cpp file
 *
 * 	Revision 1.12  2003/04/07 14:22:20  fb
 * 	Improved the alphaBeta function and trying to improve the code
 *
 * 	Revision 1.11  2003/03/26 09:52:54  fb
 * 	Removed the setPosition method and made constructor of ChessGame
 * 	available with a fen string
 *
 * 	Revision 1.10  2003/03/25 16:59:54  fb
 * 	Trying to locate the getId() 's and eliminate them.
 *
 * 	Revision 1.9  2003/03/25 13:11:10  fb
 * 	Compilable under linux too
 *
 * 	Revision 1.7  2003/03/21 19:43:41  fb
 * 	Trying to accelerate the machine!
 *
 * 	Revision 1.6  2003/03/21 09:19:34  fb
 * 	Added the alpha beta prunning algorithm
 *
 *
 *
 *
 */

#if !defined CHESS_GAME_H
#define CHESS_GAME_H

#include "chess.h"
#include "move.h"
#include "situation.h"
#include "board.h"
#include "king.h"
#include "minmax.h"

class ChessGame;

ostream & operator << (ostream & os, ChessGame & cg);

/*******************************************************************************
 *
 ******************************************************************************/

class ChessGame : Board {

	public :

	ChessGame();
	ChessGame(string & fen);
	ChessGame(ChessGame & other);

	~ChessGame();

	int32_t alphaBeta(int32_t depth, int32_t alpha, int32_t beta);

	friend ostream & operator<< (ostream & os, ChessGame & cg);

	bool checkMove(Move & m);

	void makeMove(Move & m) {

		Square * s1 = squares[m.x1][m.y1];
		Square * s2 = squares[m.x2][m.y2];

		undoSitu = Situation(s1, s2, s1->getPiece(), this, m.promotePiece);

		undoSitu.mover->do_move(undoSitu);

		//Save this situation
		situs.push_back(undoSitu);

		//Find out weather it is remese by repetition

		if (situs.size() < 6) return;

		Situations::iterator it = situs.end();
		
		--it;
		Situations::iterator sit6 = it;	//B

		--it;
		Situations::iterator sit5 = it; //W

		--it;
		//Situations::iterator sit4 = it; //B

		--it;
		//Situations::iterator sit3 = it; //W

		--it;
		Situations::iterator sit2 = it; //B

		--it;
		Situations::iterator sit1 = it; //W
		
		/*if (sit1->s1 == sit3->s2 && sit1->s2 == sit3->s1 
			&& sit2->s1 == sit4->s2 && sit2->s2 == sit4->s1
			&& sit3->s1 == sit5->s2 && sit3->s2 == sit5->s1
			&& sit4->s1 == sit6->s2 && sit4->s2 == sit6->s1) { */

		if (*sit6 == *sit2 && *sit5 == *sit1) {

			//Is this ok ?????

			repeated = true;
		}
	}

	bool makeMoveStr(string & m);

	void undoMove() {
		if (situs.empty()) {
			cout << "Error!, Can not undo any more.\n";
			return;
		}

		undoSitu = situs.back();
		situs.pop_back();

		undoMove(undoSitu);
	}

	Move listMoves();

	Color getSideToMove() {return sideToMove; }

	int32_t evaluateMove(Move & m);

	Move BESTMOVE;
	uint32_t nodes;
	int depth;

	private :

	Situation undoSitu;	
	MovesPointersArray ppmvs;

	int32_t halfRandom = (RAND_MAX / 2);

	Move levelUp;
	Move opponentsMove;
	Move my2Move;

	void init();
	void getSortedLegalMoves (Moves & moves);
	void getLegalMoves (Moves & moves);	

	bool checkMove(Situation & _situation);

	void undoMove(Situation & _situation);

	bool pieceCanMoveTo(Piece * p, Square *s) {
		int32_t x, y;
		s->getXY(x, y);
		SomeSquares ts = p->getCanMoveToSquares();

		return bitIsOn64_2(ts, x, y);
	}
};


#endif





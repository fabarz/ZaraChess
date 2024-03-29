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
#include "moveinfo.h"
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
	ChessGame(const char * fen);
	ChessGame(ChessGame & other);

	~ChessGame();

	const char * dump()
	{
		std::ostringstream stream;
		stream << *this;
		strMemory = stream.str();
		const char* chr = strMemory.c_str();
		return chr;
	}

	const char * list();
	const char * best();
	int32_t bestValue()
	{
		return bestVal;
	}
	const char * side();
	bool check(const char * move);
	int evaluate(const char * move);
	void setPromotionPiece(char p)
	{
		promoPiece = p;
	}
	void make(const char * move);
	const char * getStack();
	int32_t findBestMove();

	friend ostream & operator<< (ostream & os, ChessGame & cg);

	bool checkMove(Move & m);

	void makeMove(Move & m) {

		Square * s1 = squares[m.x1][m.y1];
		Square * s2 = squares[m.x2][m.y2];

		moveInfo = MoveInfo(s1, s2, s1->getPiece(), this, m.promotePiece);

		moveInfo.mover->do_move(moveInfo);

		//Save this situation
		moveInfoStack.push_back(moveInfo);

		//Find out weather it is remese by repetition

		if (moveInfoStack.size() < 8) return;

		MoveInfoStack::iterator it = moveInfoStack.end();
		
		--it;
		MoveInfoStack::iterator sit8 = it;	//B

		--it;
		MoveInfoStack::iterator sit7 = it;	//W

		--it;
		MoveInfoStack::iterator sit6 = it;	//B

		--it;
		MoveInfoStack::iterator sit5 = it; //W

		--it;
		MoveInfoStack::iterator sit4 = it; //B

		--it;
		MoveInfoStack::iterator sit3 = it; //W

		--it;
		MoveInfoStack::iterator sit2 = it; //B

		--it;
		MoveInfoStack::iterator sit1 = it; //W

		if (*sit1 == *sit5 && *sit4 == *sit8 && *sit2 == *sit6 && *sit3 == *sit7) {

			//Is this ok ?????
			cout << "Remise 2\n";
			cout << getStack() << "\n";
			repeated = true;
		}
	}

	bool makeMoveStr(string & m);

	void undoMove() {
		if (moveInfoStack.empty()) {
			cout << "Error!, Can not undo any more.\n";
			return;
		}

		moveInfo = moveInfoStack.back();
		moveInfoStack.pop_back();

		undoMove(moveInfo);
	}

	string listMoves();

	Color getSideToMove() {return sideToMove; }

	char promoPiece;
	int32_t evaluateMove(Move & m);

	Move bestMove;
	int32_t bestVal;
	uint32_t nodes;
	void setDepth(int val)
	{
		if (val > 0)
		{
			depth = val;
		}
	}

	int getDepth()
	{
		return depth;
	}
	private :
	int depth;
	string strMemory;
	MoveInfo moveInfo;	
	MovesPossiblePerDepth movesInDepth;
	int32_t alphaBeta(int32_t depth, int32_t alpha, int32_t beta);

	int32_t halfRandom = (RAND_MAX / 2);

	Move levelUp;
	Move opponentsMove;
	Move my2Move;

	void init();
	void getSortedLegalMoves (MovesPossible & moves);
	void getLegalMoves (MovesPossible & moves);	

	bool checkMove(MoveInfo & _situation);

	void undoMove(MoveInfo & _situation);

	bool pieceCanMoveTo(Piece * p, Square *s) {
		int32_t x, y;
		s->getXY(x, y);
		SomeSquares ts = p->getCanMoveToSquares();

		return bitIsOn64_2(ts, x, y);
	}
};


#endif





/*
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/chessgame.cpp,v 1.25 2003/04/29 08:01:55 fb Exp $
 *
 * Rivisions:
 * 	$Log: chessgame.cpp,v $
 * 	Revision 1.25  2003/04/29 08:01:55  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 * 	
 * 	Revision 1.22  2003/04/22 15:54:52  fb
 * 	made all the functions in utils.h macros and put in macros.h
 * 	
 * 	Revision 1.19  2003/04/17 11:23:45  fb
 * 	improved the evaluation function and functions in the utils.cpp file
 *
 * 	Revision 1.14  2003/04/07 14:22:19  fb
 * 	Improved the alphaBeta function and trying to improve the code
 *
 * 	Revision 1.13  2003/03/26 09:52:54  fb
 * 	Removed the setPosition method and made constructor of ChessGame
 * 	available with a fen string
 *
 * 	Revision 1.12  2003/03/25 16:59:53  fb
 * 	Trying to locate the getId() 's and eliminate them.
 *
 * 	Revision 1.11  2003/03/25 13:11:10  fb
 * 	Compilable under linux too
 *
 * 	Revision 1.8  2003/03/21 19:43:41  fb
 * 	Trying to accelerate the machine!
 *
 * 	Revision 1.7  2003/03/21 09:19:34  fb
 * 	Added the alpha beta prunning algorithm
 *
 *
 *
 *
 */

#include "chessgame.h"
#include <iterator>


const char possiblePromotion[2][4] =
	{
		{'Q', 'R', 'B', 'N'},
		{'q', 'r', 'b', 'n'}
	};


extern const char * squareNames[8][8];


ostream & operator<< (ostream & os, ChessGame & cg) {
	os << "\t\t    A  B  C  D  E  F  G  H  \n";
	os << "\t\t  --------------------------\n";
	for (int32_t jj = BOARD_SIZE - 1; jj >= 0; jj--) {
		os << "\t\t";
		os << (int) (jj + 1) << " |";
		for (int32_t ii = 0; ii < BOARD_SIZE; ii++) {
			if (!cg.squares[ii][jj]->getPiece())
				os << " . ";
			else if (cg.squares[ii][jj]->getPiece()->getColor() == BLACK) {
				os << "(" << (cg.squares[ii][jj]->getPiece()->getName()) << ")";
			} else {
				os << " " << (cg.squares[ii][jj]->getPiece()->getName()) << " ";
			}
		}
		os << "| "  << (int) (jj + 1) << endl;
		if (jj > 0)
			cout << endl;
	}
	os << "\t\t  --------------------------\n";
	os << "\t\t    A  B  C  D  E  F  G  H  \n";
	os << "\n\t\t";
	os << " No. of moves: White = " << (int) cg.nMoves[WHITE] << " , Black = " << (int) cg.nMoves[BLACK] << endl;
	return os;
}

/*******************************************************************************
 *
 ******************************************************************************/

ChessGame::ChessGame() : Board(), moveInfo(0,0,0,0, 'Q'), movesInDepth(20) {
	init();
}

ChessGame::ChessGame(const char * fen) : Board(string(fen), WHITE, 0, 0, 0, 0, 0), moveInfo(0,0,0,0, 'Q'), movesInDepth(20) {
	init();
}

ChessGame::ChessGame(ChessGame & other) : Board(other), moveInfo(0,0,0,0,'Q'), movesInDepth(20) {
	init();
}

ChessGame::~ChessGame() {
	Piece::number[WHITE] = NO_UID;
	Piece::number[BLACK] = NO_UID;
}

void ChessGame::init()
{
	depth = 5;
	nodes = 0;
}

bool ChessGame::checkMove(Move & m) {
	Square * s1 = getSquare(m.x1, m.y1);
	Square * s2 = getSquare(m.x2, m.y2);

	if (!s1) {
		cout << "No such first square.\n";
		return false;
	}

	MoveInfo situation1(s1, s2, s1->getPiece(), this, m.promotePiece);

	return checkMove(situation1);

}

bool ChessGame::checkMove(MoveInfo & _situation) {
	if (!_situation.s1 || !_situation.s2) {
		cout << "One or both sqaues not valid.\n";
		return false;
	}

	if (_situation.s1->getPiece() == 0) {
		cout << "No piece in the sqaure : " << _situation.s1->getName() << " .\n";
		return false;
	}

	Piece * p = _situation.s1->getPiece();
	Color c = p->getColor();

	if (c != sideToMove) {
		cout << "Not this sides turn to move.\n";
		return false;
	}

	if (!pieceCanMoveTo(_situation.mover, _situation.s2)) {
		cout << "This piece (" << _situation.mover->getName() << " at " << _situation.s1->getName()
			<< ") can not move to that square(" << _situation.s2->getName() << ")\n";

		return false;
	}

	return true;

}

bool ChessGame::makeMoveStr(string & m) {

	string sqn1, sqn2;
	sqn1 = m.substr(0, 2);
	sqn2 = m.substr(2, 2);

	Square * s1 = getSquare(sqn1);
	Square * s2 = getSquare(sqn2);

	if (!s1 || !s2) {
		cout << "Invalid squares in move.\n";
		return false;
	}

	moveInfo = MoveInfo(s1, s2, s1->getPiece(), this, promotionPiece[sideToMove]);

	if (!checkMove(moveInfo))
		return false;

	moveInfo.mover->do_move(moveInfo);

	//Save this situation
	moveInfoStack.push_back(moveInfo);

	return true;
}

void ChessGame::undoMove( MoveInfo & _situation) {


#if defined _DEBUG

	Piece * p = _situation.s2->getPiece();

	Piece * p2 = p->undo_move(_situation);

	if (p2) {
		/* If it is a promoted piece it should come back here */
		assert (p2 == p);
		delete p2;
	}
#else

	Piece * p2 = _situation.s2->getPiece()->undo_move(_situation);

	if (p2) {
		delete p2;
	}

#endif

}

void ChessGame::getSortedLegalMoves (MovesPossible & moves) {

	getLegalMoves(moves);

	MovesPossible::iterator it = moves.begin();
	MovesPossible::iterator endIt = moves.end();

	while (it != endIt) {

		makeMove(*it);
		
		/*if (repeated) {			
		
			repeated = false;
			it->value = 0;

		} else */
		
		if (nMoves[sideToMove] == 0) {
			
			if (KINGS[sideToMove]->isCheck()) {
				it->value = CHECKMATE;
			} else {
				it->value = 0;
			}

		} else {

			it->value = -evaluatePosition();

		}

		undoMove();
		++it;
	}

	moves.sort();
}

void ChessGame::getLegalMoves (MovesPossible & moves) {

	Move m;

	for (int32_t pieceIdx = 0; pieceIdx < MAX_PIECES_PER_SIDE; pieceIdx++) {

		Piece * piece = pieceTable[sideToMove][pieceIdx];

		if (!piece) {
			continue;
		}

		SomeSquares to1 = (piece)->getCanMoveToSquares();

		if (!to1) {
			continue;
		}

		piece->getSquare()->getXY(m.x1, m.y1);

		/* yy = situ.col == WHITE ? 6 : 1;  This condition changed */

		if ((piece)->canPromote() && m.y1 == (sideToMove == WHITE ? 6 : 1)/* m.y1 == 6 - 5 * sideToMove*/) {

			uint32_t bitn;

			while (to1) {

				firstBitSet64(to1, bitn); //It does not shift it left now. but sets the bit off
				/* bitn is from 1 to 64 so subtract one */

				int32_t i = (bitn - 1) % BOARD_SIZE;
				int32_t j = (bitn - 1) / BOARD_SIZE;

				m.x2 = i; m.y2 = j;

				for (int32_t pp = 0; pp < 4; pp++) {
					m.promotePiece = possiblePromotion[sideToMove][pp];
					moves.push_back(m);
				}
			}

		} else {

			/* Normal piece */

			m.promotePiece = 0;

			uint32_t bitn;

			while (to1) {

				firstBitSet64(to1, bitn); //It does not shift it left now. but sets the bit off

				/* bitn is from 1 to 64 so subtract one */

				int32_t i = (bitn - 1) % BOARD_SIZE;
				int32_t j = (bitn - 1) / BOARD_SIZE;

				m.x2 = i; m.y2 = j;
				moves.push_back(m);
			}
		}
	}
}

string ChessGame::listMoves() {

	Move result;

	MovesPossible m;

	getSortedLegalMoves(m);

	MovesPossible::iterator it = m.begin();
	ostringstream os;
	while (it != m.end()) {
		Move m1 = *it;
		os << m1 << " = " << m1.value << endl;
		++it;
	}

	if (!m.empty()) {
		result = *(m.begin());
	}

	return os.str();

}

int32_t ChessGame::evaluateMove(Move & m) {

	//This function is being called for sideToMove

	makeMove(m);

	//sideToMove has changed now

	int32_t res;

	if (nMoves[sideToMove] == 0) {

		//Is the opponent mate ?
		if (KINGS[sideToMove]->isCheck()) {

			res = CHECKMATE;

		} else {
			
			res = 0;

		}

	} else {
		
		res = -evaluatePosition();

	}

	undoMove();

	return res;
}
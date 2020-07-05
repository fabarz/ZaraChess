/*e
 * File : $Header: /home/barzangy/repo/sources/thesis/chess/board.h,v 1.22 2003/04/29 08:01:54 fb Exp $
 *
 * Rivisions:
 * 	$Log: board.h,v $
 * 	Revision 1.22  2003/04/29 08:01:54  fb
 * 	After shotening the update procedure also fixed some bugs in the evaluation function
 * 	Shortening the update means introducing the squareChanged method which will
 * 	update a piece only in one direction.
 *
 * 	Revision 1.19  2003/04/22 15:54:53  fb
 * 	made all the functions in utils.h macros and put in macros.h
 *
 * 	Revision 1.16  2003/04/17 11:23:46  fb
 * 	improved the evaluation function and functions in the utils.cpp file
 *
 * 	Revision 1.11  2003/03/26 09:52:54  fb
 * 	Removed the setPosition method and made constructor of ChessGame
 * 	available with a fen string
 *
 * 	Revision 1.10  2003/03/25 16:59:53  fb
 * 	Trying to locate the getId() 's and eliminate them.
 *
 * 	Revision 1.9  2003/03/25 13:11:11  fb
 * 	Compilable under linux too
 *
 * 	Revision 1.7  2003/03/21 19:43:40  fb
 * 	Trying to accelerate the machine!
 *
 * 	Revision 1.6  2003/03/21 09:19:35  fb
 * 	Added the alpha beta prunning algorithm
 *
 *
 *
 *
 */

#if !defined BOARD_H_INCLUDED
#define BOARD_H_INCLUDED


/*******************************************************************************
 *
 ******************************************************************************/

#include "square.h"
#include "situation.h"

class King;
class Board;


ostream & operator << (ostream & os, Board & cg);

/*******************************************************************************************************************************
 * squareNames
 * Purpose	: an array of 8 X 8 [0..7][0..7] with the names of the squares of a chess Board as strings
 ******************************************************************************************************************************/

class Board {

	public :

	friend ostream & operator <<(ostream & os, Board & cg);

	Board();

	Board(string & fen, Color sidesMove = WHITE,
			Square * enPas = 0, bool wccqs = false,
			bool wccks = false, bool bccqs = false, bool bccks = false);

	virtual ~Board();

	void initPieces();
	void cleanPieces ();

	void toggleSideToMove() {sideToMove = (sideToMove ? WHITE : BLACK); }

	Square * getSquare(int32_t x, int32_t y) {
		if ((x | y) & 0xFFFFFFF8)
			return 0;
		//This is equivalent to the following
		//if ((uint32_t) x >= BOARD_SIZE || (uint32_t) y >= BOARD_SIZE)
		//	return 0;
		return squares[x][y];
	}

	Square * getSquare(string & name);

	void addPiece(Piece * p, Square * s);
	void removePiece(Piece * p);

	Piece * getPiece(Color c, int32_t arrayIndex) {

		assert (arrayIndex >=0 && arrayIndex < MAX_PIECES_PER_SIDE);
		return pieceTable[c][arrayIndex];

	}

	void getSquares(Square * s1, Square * s2, SomeSquares & to);
		/* Not including s1 but including s2 */

	int32_t evaluatePosition();

	void allPiecesHandicap(Color c, SomeSquares & squaresBetweenKingAndPinner) {
		/* except the king */

		for (int32_t pieceIdx = 0; pieceIdx < MAX_PIECES_PER_SIDE; pieceIdx++) {

			if (pieceTable[c][pieceIdx])
				pieceTable[c][pieceIdx]->makeHandicap(squaresBetweenKingAndPinner);

		}

	}

	void allPownsHandicap(Color c, SomeSquares & squaresBetweenKingAndPinner) {
		/* except the king */

		for (int32_t pieceIdx = 0; pieceIdx < MAX_PIECES_PER_SIDE; pieceIdx++) {

			if (pieceTable[c][pieceIdx] && pieceTable[c][pieceIdx]->canPromote())
				pieceTable[c][pieceIdx]->makeHandicap(squaresBetweenKingAndPinner);

		}

	}

	void allNotPownsHandicap(Color c, SomeSquares & squaresBetweenKingAndPinner) {
		/* except the king */

		for (int32_t pieceIdx = 0; pieceIdx < MAX_PIECES_PER_SIDE; pieceIdx++) {

			if (pieceTable[c][pieceIdx] && !pieceTable[c][pieceIdx]->canPromote())
				pieceTable[c][pieceIdx]->makeHandicap(squaresBetweenKingAndPinner);

		}

	}

	void freeHandicaps(Color c) {

		for (int32_t pieceIdx = 0; pieceIdx < MAX_PIECES_PER_SIDE; pieceIdx++) {

			if (pieceTable[c][pieceIdx] && pieceTable[c][pieceIdx]->getHandicapped()) {
				pieceTable[c][pieceIdx]->update();
			}
		}
	}

	void call(uint32_t pcs) {

		uint16_t * pcopyInts = (uint16_t *) &pcs;

		for (int32_t cc = 0; cc < 2; cc++) {

			if (cc) pcopyInts++;

			uint32_t bitn;

			while (*pcopyInts) {

				firstBitSet16 (*pcopyInts, bitn); //It does not shif left now. but sets the bit off.

				assert(pieceTable[cc][bitn - 1]);

				pieceTable[cc][bitn - 1]->update();

			}
		}
	}

	void changeNMoves(Color c, int32_t amount) { nMoves[c] += amount;}

	Piece * createPiece(char c, int32_t arrayIdx = NO_UID);

	void updateKings();

	void updateEffectedPieces(Situation & newSitu);
	void updateEffectedPiecesByUndo(Situation & newSitu);

	void saveSituation(Situation & situ);
	Piece * restoreSituation(Situation & situ);

	void updateAllPieces(Color c); /* if c == none both */

	/* Public attributes */

	Square * getPassantSquare(Color c) {
		return enPassantSquare[c];
	}

	void setPassantSquare(Color c, Square * s) {
		enPassantSquare[c] = s;
	}

	King * getKing(Color c) {
		return KINGS[c];
	}

	protected :

	Color sideToMove;

	Square * squares[BOARD_SIZE][BOARD_SIZE];

	Piece * pieceTable[2][MAX_PIECES_PER_SIDE];

	King * KINGS[2];


	int32_t nMoves[2];

	char promotionPiece[2];

	bool repeated;

	Situations situs;

	Square * enPassantSquare[2];


	private :

	friend Square;

	int32_t score[2];
	int32_t pieceKilled[2];

	int8_t pownOnX[2][BOARD_SIZE];
	int8_t pownOnY[2][BOARD_SIZE];

	Square * sEval;
	int32_t xEval, yEval;
};

#endif





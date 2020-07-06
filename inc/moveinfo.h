#if !defined SITUATION_INCLUDED
#define SITUATION_INCLUDED

#include "square.h"

class Piece;

#include "chess.h"

class MoveInfo {
	public :

	MoveInfo() {
		s1 = 0; s2 = 0; mover = 0; b = 0;
		s3 = 0; s4 = 0; killed = 0; killedAt = 0; passantSquareDel = 0; passantSquareCre = 0;
		promotePiece = 0;
	}

	MoveInfo(Square * _s1, Square * _s2, Piece * _mover, Board * _board,
		char _promotePiece) {
		s1 = _s1; s2 = _s2; mover = _mover; b = _board;
		s3 = 0; s4 = 0; killed = 0; killedAt = 0; passantSquareDel = 0; passantSquareCre = 0;
		promotePiece = _promotePiece;
	}

	bool operator == (MoveInfo & other) {	/* Is this good enough ???? */
		return other.s1 == s1 && other.s2 == s2 && other.mover == mover && other.killed == killed &&
			other.killedAt == killedAt && other.col == col;
	}

	class Board * b;
	char promotePiece;
	Color col; /* *Side moving */
	Color opponentColor; /* *The opponent */
	class Square * s1; /* *The original place */
	class Square * s2; /* *Piece moved to */
	class Square * s3; /* *Another piece moved from */
	class Square * s4; /* *Another piece moved to */
	class Piece * mover; /* *The piece that moved */
	class Piece * killed; /* *The piece that been killed */
	class Square * killedAt; /* *The square that the killd piece was kileld at */
	class Square * passantSquareDel; /* *Passant square expired */
	class Square * passantSquareCre; /* New passant square created */
	bool pieceWasMoved;
	int32_t king_castleBits[2];
};

ostream & operator<< (ostream & os, MoveInfo & s);

#endif



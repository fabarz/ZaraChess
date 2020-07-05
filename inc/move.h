#if !defined MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include "chess.h"

extern const char * squareNames[8][8];

class Move {
	public :

	int32_t x1;
	int32_t y1;
	int32_t x2;
	int32_t y2;
	int32_t value;
	char promotePiece;

	Move() {
		x1 = -1;
	}

	string toString () {
		string res = string(squareNames[x1][y1]) + string(squareNames[x2][y2]);
		return res;
	}

	Move & operator=(string & ms) {
		x1 = y1 = x2 = y2 = -1;
		string sq1 = ms.substr(0, 2);
		string sq2 = ms.substr(2, 2);
		int32_t xx, yy;
		for (xx = 0; xx < BOARD_SIZE; xx++) {
			for (yy = 0; yy < BOARD_SIZE; yy++) {
				if (sq1 == squareNames[xx][yy]) {
					x1 = xx;
					y1 = yy;
				} else if (sq2 == squareNames[xx][yy]) {
					x2 = xx;
					y2 = yy;
				}
			}
		}

		return *this;
	}
};

ostream & operator <<(ostream & os, Move & m);
ostream & operator <<(ostream & os, const Move & m);
bool operator < (Move & m1, Move & m2);
bool operator < (const Move & m1, const Move & m2);

typedef Moves * MovesPointersArray[100];
typedef MovesPointersArray * PMovesPointersArray;

#endif



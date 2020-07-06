#include "move.h"

ostream & operator << (ostream & os, Move & m) {

	if ((unsigned) m.x1 >= BOARD_SIZE || (unsigned) m.y1 >= BOARD_SIZE
		|| (unsigned) m.x2 >= BOARD_SIZE || (unsigned) m.y2 >= BOARD_SIZE) {
		os << "INV_MOVE";
		return os;
	}
	os << squareNames[m.x1][m.y1] << squareNames[m.x2][m.y2] ;

	if (m.promotePiece)
		os << "(" << m.promotePiece << ")";

	return os;
}

ostream & operator << (ostream & os, const Move & m) {

	os << squareNames[m.x1][m.y1] << squareNames[m.x2][m.y2] ;

	if (m.promotePiece)
		os << "(" << m.promotePiece << ")";

	os << " " << m.value;
	return os;
}


bool operator < (Move & m1, Move & m2) {
	return m1.value > m2.value;
}

bool operator < (const Move & m1, const Move & m2) {
	return m1.value > m2.value;
}


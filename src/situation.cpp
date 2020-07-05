
#if defined WIN32
#pragma warning (disable:4786)
#endif

#include "situation.h"

#define DUMP_SQUARE(x) if (x) cout << "\t" << #x << " =:= " << x->getName() << endl

ostream & operator<< (ostream & os, Situation & s) {
	cout << "Situation dump:\n";
	DUMP_SQUARE(s.s1);
	DUMP_SQUARE(s.s2);
	DUMP_SQUARE(s.s3);
	DUMP_SQUARE(s.s4);
	DUMP_SQUARE(s.mover);
	DUMP_SQUARE(s.killed);
	DUMP_SQUARE(s.killedAt);
	DUMP_SQUARE(s.passantSquareCre);
	DUMP_SQUARE(s.passantSquareDel);
	cout << "\tPromotePiece =:= " << s.promotePiece <<endl;
	return os;
}

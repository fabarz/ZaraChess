#include "chess.h"
#include "move.h"

class ChessGame;

int32_t alphaBeta(ChessGame * cg, int32_t depth, int32_t alpha, int32_t beta, Moves ** ppmvs);



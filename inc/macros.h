#include <assert.h>
#include <common/plftyp.h>
#include "chess.h"

extern const char firstBitSet256[256];
extern const uint16_t mask16On[16];
extern const uint32_t mask16Off[16];
extern const uint32_t mask32On[32];
extern const uint32_t mask32Off[32];
extern const unsigned __int64 mask64On[64];
extern const unsigned __int64 mask64On_2[8][8];
extern const unsigned __int64 mask64Off_2[8][8];
extern const unsigned __int64 mask64Off[64];
extern const char bits[256];    

#define firstBitSet64(val, bitnr) {									\
	bitnr = 0;														\
	if (val) {														\
		unsigned char * Ptr = (unsigned char *) &val;				\
		while (*Ptr == 0) {Ptr++; bitnr += 8;}						\
		bitnr += firstBitSet256[*Ptr];	/* Which byte was it? */	\
		assert (bitnr >= 1 || bitnr <= 64);							\
		val &= mask64Off[bitnr - 1];		/*Set the bit off. */	\
	}																\
}


/* This variant works by repetitively masking off the last bit.
   This can be easily done by subtracting one, then anding. */

#define firstBitSet32(val, bitnr) {									\
	bitnr = 0;														\
	if (val) {														\
		unsigned char * Ptr = (unsigned char *) &val;				\
		while (*Ptr == 0) {Ptr++; bitnr += 8;}						\
		bitnr += firstBitSet256[*Ptr];	/* Which byte was it? */	\
		assert (bitnr >= 1 || bitnr <= 32);							\
		val &= mask32Off[bitnr - 1];		/*Set the bit off. */	\
	}																\
}


#define firstBitSet16(val, bitnr) {									\
	bitnr = 0;														\
	if (val) {														\
		unsigned char * Ptr = (unsigned char *) &val;				\
		while (*Ptr == 0) {Ptr++; bitnr += 8;}						\
		bitnr += firstBitSet256[*Ptr];	/* Which byte was it? */	\
		assert (bitnr >= 1 || bitnr <= 16);							\
		val &= mask16Off[bitnr - 1];		/*Set the bit off. */	\
	}																\
}

//========================================================================

#define bitCount64(n, Accu) {									\
	unsigned char * Ptr = (unsigned char *) &n;					\
	Accu = bits[ *Ptr++ ];										\
	Accu += bits[ *Ptr++ ];										\
	Accu += bits[ *Ptr++ ];										\
	Accu += bits[ *Ptr++ ];										\
	Accu += bits[ *Ptr++ ];										\
	Accu += bits[ *Ptr++ ];										\
	Accu += bits[ *Ptr++ ];										\
	Accu += bits[ *Ptr ];										\
}

//=======================================================================================

#define setBitOn16(n, b) (n |= mask16On[b])

#define setBitOff16(n, b) (n &= mask16Off[b])

#define setBitOn32(n, b) (n |= mask32On[b])

#define setBitOff32(n, b) (n &= mask32Off[b])

#define bitIsOn16(n, b) (0 != (n & mask16On[b]))

#define bitIsOn32(n, b) (0 != (n & mask32On[b]))

#define bitIsOn64(n, b) (0 != (n & mask64On[b]))

#define bitIsOn64_2(n, x, y) ( 0 != (n & mask64On_2[y][x]))

#define setBitOn64(n, b) (n |= mask64On[b])

#define setBitOn64_2(n, x, y) (n |= mask64On_2[y][x])

#define setBitOff64_2(n, x, y) (n &= mask64Off_2[y][x])

#define SIGN(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0) );


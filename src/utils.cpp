#include <common/plftyp.h>
#include <assert.h>
#include "macros.h"

const char firstBitSet256[256] = {
  //0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
	0, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, /* 0- 15 */
	5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
	6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
	5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
	7, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
	5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
	6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
	5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
	8, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
	5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
	6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
	5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
	7, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
	5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
	6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
	5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1

};

const uint16_t mask16On[16] = {

	0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80,
	0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000

};

const uint32_t mask16Off[16] = {

	~((uint16_t) 0x1), ~((uint16_t) 0x2), ~((uint16_t) 0x4), ~((uint16_t) 0x8), ~((uint16_t) 0x10), ~((uint16_t) 0x20), ~((uint16_t) 0x40), ~((uint16_t) 0x80),
	~((uint16_t) 0x100), ~((uint16_t) 0x200), ~((uint16_t) 0x400), ~((uint16_t) 0x800), ~((uint16_t) 0x1000), ~((uint16_t) 0x2000), ~((uint16_t) 0x4000), ~((uint16_t) 0x8000)

};

const uint32_t mask32On[32] = {

	0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80,
	0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000,
	0x10000, 0x20000, 0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000,
	0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000

};

const uint32_t mask32Off[32] = {

	~((uint32_t) 0x1), ~((uint32_t) 0x2), ~((uint32_t) 0x4), ~((uint32_t) 0x8), ~((uint32_t) 0x10), ~((uint32_t) 0x20), ~((uint32_t) 0x40), ~((uint32_t) 0x80),
	~((uint32_t) 0x100), ~((uint32_t) 0x200), ~((uint32_t) 0x400), ~((uint32_t) 0x800), ~((uint32_t) 0x1000), ~((uint32_t) 0x2000), ~((uint32_t) 0x4000), ~((uint32_t) 0x8000),
	~((uint32_t) 0x10000), ~((uint32_t) 0x20000), ~((uint32_t) 0x40000), ~((uint32_t) 0x80000), ~((uint32_t) 0x100000), ~((uint32_t) 0x200000), ~((uint32_t) 0x400000), ~((uint32_t) 0x800000),
	~((uint32_t) 0x1000000), ~((uint32_t) 0x2000000), ~((uint32_t) 0x4000000), ~((uint32_t) 0x8000000), ~((uint32_t) 0x10000000), ~((uint32_t) 0x20000000), ~((uint32_t) 0x40000000), ~((uint32_t) 0x80000000)

};

const unsigned __int64 mask64On[64] = {

	0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80,
	0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000,
	0x10000, 0x20000, 0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000,
	0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000,

	0x100000000, 0x200000000, 0x400000000, 0x800000000, 0x1000000000, 0x2000000000, 0x4000000000, 0x8000000000,
	0x10000000000, 0x20000000000, 0x40000000000, 0x80000000000, 0x100000000000, 0x200000000000, 0x400000000000, 0x800000000000,
	0x1000000000000, 0x2000000000000, 0x4000000000000, 0x8000000000000, 0x10000000000000, 0x20000000000000, 0x40000000000000, 0x80000000000000,
	0x100000000000000, 0x200000000000000, 0x400000000000000, 0x800000000000000, 0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000

};

const unsigned __int64 mask64On_2[8][8] = {

	{0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80},
	{0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000},
	{0x10000, 0x20000, 0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000},
	{0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000},

	{0x100000000, 0x200000000, 0x400000000, 0x800000000, 0x1000000000, 0x2000000000, 0x4000000000, 0x8000000000},
	{0x10000000000, 0x20000000000, 0x40000000000, 0x80000000000, 0x100000000000, 0x200000000000, 0x400000000000, 0x800000000000},
	{0x1000000000000, 0x2000000000000, 0x4000000000000, 0x8000000000000, 0x10000000000000, 0x20000000000000, 0x40000000000000, 0x80000000000000},
	{0x100000000000000, 0x200000000000000, 0x400000000000000, 0x800000000000000, 0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000}

};

const unsigned __int64 mask64Off_2[8][8] = {

	{~((__int64) 0x1), ~((__int64) 0x2), ~((__int64) 0x4), ~((__int64) 0x8), ~((__int64) 0x10), ~((__int64) 0x20), ~((__int64) 0x40), ~((__int64) 0x80) },
	{~((__int64) 0x100), ~((__int64) 0x200), ~((__int64) 0x400), ~((__int64) 0x800), ~((__int64) 0x1000), ~((__int64) 0x2000), ~((__int64) 0x4000), ~((__int64) 0x8000) },
	{~((__int64) 0x10000), ~((__int64) 0x20000), ~((__int64) 0x40000), ~((__int64) 0x80000), ~((__int64) 0x100000), ~((__int64) 0x200000), ~((__int64) 0x400000), ~((__int64) 0x800000) },
	{~((__int64) 0x1000000), ~((__int64) 0x2000000), ~((__int64) 0x4000000), ~((__int64) 0x8000000), ~((__int64) 0x10000000), ~((__int64) 0x20000000), ~((__int64) 0x40000000), ~((__int64) 0x80000000) },

	{~((__int64) 0x100000000), ~((__int64) 0x200000000), ~((__int64) 0x400000000), ~((__int64) 0x800000000), ~((__int64) 0x1000000000), ~((__int64) 0x2000000000), ~((__int64) 0x4000000000), ~((__int64) 0x8000000000) },
	{~((__int64) 0x10000000000), ~((__int64) 0x20000000000), ~((__int64) 0x40000000000), ~((__int64) 0x80000000000), ~((__int64) 0x100000000000), ~((__int64) 0x200000000000), ~((__int64) 0x400000000000), ~((__int64) 0x800000000000) },
	{~((__int64) 0x1000000000000), ~((__int64) 0x2000000000000), ~((__int64) 0x4000000000000), ~((__int64) 0x8000000000000), ~((__int64) 0x10000000000000), ~((__int64) 0x20000000000000), ~((__int64) 0x40000000000000), ~((__int64) 0x80000000000000) },
	{~((__int64) 0x100000000000000), ~((__int64) 0x200000000000000), ~((__int64) 0x400000000000000), ~((__int64) 0x800000000000000), ~((__int64) 0x1000000000000000), ~((__int64) 0x2000000000000000), ~((__int64) 0x4000000000000000), ~((__int64) 0x8000000000000000) }

};

const unsigned __int64 mask64Off[64] = {

	~((__int64) 0x1), ~((__int64) 0x2), ~((__int64) 0x4), ~((__int64) 0x8), ~((__int64) 0x10), ~((__int64) 0x20), ~((__int64) 0x40), ~((__int64) 0x80),
	~((__int64) 0x100), ~((__int64) 0x200), ~((__int64) 0x400), ~((__int64) 0x800), ~((__int64) 0x1000), ~((__int64) 0x2000), ~((__int64) 0x4000), ~((__int64) 0x8000),
	~((__int64) 0x10000), ~((__int64) 0x20000), ~((__int64) 0x40000), ~((__int64) 0x80000), ~((__int64) 0x100000), ~((__int64) 0x200000), ~((__int64) 0x400000), ~((__int64) 0x800000),
	~((__int64) 0x1000000), ~((__int64) 0x2000000), ~((__int64) 0x4000000), ~((__int64) 0x8000000), ~((__int64) 0x10000000), ~((__int64) 0x20000000), ~((__int64) 0x40000000), ~((__int64) 0x80000000),

	~((__int64) 0x100000000), ~((__int64) 0x200000000), ~((__int64) 0x400000000), ~((__int64) 0x800000000), ~((__int64) 0x1000000000), ~((__int64) 0x2000000000), ~((__int64) 0x4000000000), ~((__int64) 0x8000000000),
	~((__int64) 0x10000000000), ~((__int64) 0x20000000000), ~((__int64) 0x40000000000), ~((__int64) 0x80000000000), ~((__int64) 0x100000000000), ~((__int64) 0x200000000000), ~((__int64) 0x400000000000), ~((__int64) 0x800000000000),
	~((__int64) 0x1000000000000), ~((__int64) 0x2000000000000), ~((__int64) 0x4000000000000), ~((__int64) 0x8000000000000), ~((__int64) 0x10000000000000), ~((__int64) 0x20000000000000), ~((__int64) 0x40000000000000), ~((__int64) 0x80000000000000),
	~((__int64) 0x100000000000000), ~((__int64) 0x200000000000000), ~((__int64) 0x400000000000000), ~((__int64) 0x800000000000000), ~((__int64) 0x1000000000000000), ~((__int64) 0x2000000000000000), ~((__int64) 0x4000000000000000), ~((__int64) 0x8000000000000000)

};

const char bits[256] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, /* 0- 15 */
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, /* 16 - 31 */
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, /* 32 - 47 */
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, /* 48 - 63 */
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, /* 64 - 79 */
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, /* 80 - 95 */
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, /* 96 - 111 */
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, /* 112 - 127 */
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, /* 128 - 143 */
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, /* 144 - 159 */
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, /* 160 - 175 */
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, /* 176 - 191 */
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, /* 192 - 207 */
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, /* 208 - 223 */
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, /* 224 - 239 */
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8  /* 240 - 255 */
};

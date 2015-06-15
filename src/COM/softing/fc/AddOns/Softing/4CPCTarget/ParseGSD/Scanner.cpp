/* lex -alTv -LC -o scanner.cpp -D scanner.h scanner.l */
#define INITIAL 0
#define DP_text 2
const yy_endst = 50;
const yy_nxtmax = 1724;
#define YY_LA_SIZE 7

static unsigned int yy_la_act[] = {
 1, 2, 2, 2, 0, 18, 21, 18, 20, 21, 4, 20, 21, 21, 6, 15,
 21, 7, 21, 8, 16, 21, 8, 16, 21, 10, 20, 21, 11, 20, 21, 12,
 20, 21, 13, 20, 21, 14, 20, 21, 16, 21, 19, 21, 20, 21, 21, 19,
 16, 8, 16, 16, 9, 16, 6, 5, 18, 4, 7, 18, 21, 18, 20, 21,
 20, 21, 529, 19, 3, 18, 529, 0
};

static unsigned char yy_look[] = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x8e, 0x00, 0
};

static int yy_final[] = {
 0, 0, 1, 2, 2, 3, 4, 5, 5, 5, 5, 7, 10, 13, 14, 17,
 19, 22, 25, 28, 31, 34, 37, 40, 42, 43, 44, 46, 47, 48, 49, 51,
 52, 54, 55, 55, 55, 56, 56, 57, 58, 59, 59, 61, 64, 66, 68, 69,
 70, 70, 71
};
#ifndef yy_state_t
#define yy_state_t unsigned char
#endif

static yy_state_t yy_begin[] = {
 0, 3, 9, 41, 0
};

static yy_state_t yy_next[] = {
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 4, 28, 50, 50, 50, 50, 50, 50, 6, 50, 6, 6, 6, 6, 6, 6,
 6, 6, 6, 6, 50, 50, 50, 4, 50, 50, 5, 6, 6, 6, 6, 6,
 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
 6, 6, 6, 6, 6, 50, 50, 50, 50, 6, 50, 6, 6, 6, 6, 6,
 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
 6, 6, 6, 6, 6, 7, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 50, 50, 7, 50, 50, 8,
 27, 27, 27, 27, 27, 27, 27, 27, 27, 10, 24, 27, 27, 25, 27, 27,
 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
 11, 26, 13, 26, 26, 26, 26, 26, 20, 21, 26, 26, 19, 22, 23, 26,
 16, 17, 17, 17, 17, 17, 17, 17, 17, 17, 26, 12, 26, 18, 26, 26,
 14, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 26, 15, 26, 26, 23,
 27, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 26, 26, 26, 27, 27,
 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
 27, 27, 27, 27, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
 29, 50, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 50, 50, 50, 50,
 50, 50, 50, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 50, 50, 50,
 50, 29, 50, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 30, 30, 30,
 30, 30, 30, 30, 30, 30, 30, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 42, 45, 50, 50, 50, 50, 50, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 43, 50, 50, 44, 50, 50, 50, 32,
 32, 32, 32, 32, 32, 50, 50, 50, 32, 32, 32, 32, 32, 32, 50, 50,
 50, 50, 50, 50, 50, 31, 50, 50, 50, 50, 50, 50, 50, 50, 50, 32,
 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 50,
 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
 33, 33, 33, 33, 33, 50, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
 33, 33, 33, 33, 33, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
 35, 35, 35, 35, 35, 35, 35, 36, 35, 35, 35, 35, 35, 35, 35, 35,
 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
 35, 34, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
 35, 35, 35, 35, 35, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 50,
 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
 37, 37, 37, 37, 37, 38, 50, 50, 50, 50, 46, 50, 46, 46, 46, 46,
 46, 46, 46, 46, 46, 46, 50, 50, 50, 50, 50, 50, 38, 46, 46, 46,
 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46,
 46, 46, 46, 46, 46, 46, 46, 39, 50, 50, 50, 46, 50, 46, 46, 46,
 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46,
 46, 46, 46, 46, 46, 46, 46, 50, 40, 47, 49, 50, 50, 50, 50, 50,
 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
 47, 50, 50, 48, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 39, 50, 50, 50, 50,
 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 40, 0
};

static yy_state_t yy_check[] = {
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 25, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, 5, ~0U, 5, 5, 5, 5, 5, 5,
 5, 5, 5, 5, ~0U, ~0U, ~0U, 3, ~0U, ~0U, 3, 5, 5, 5, 5, 5,
 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
 5, 5, 5, 5, 5, ~0U, ~0U, ~0U, ~0U, 5, ~0U, 5, 5, 5, 5, 5,
 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
 5, 5, 5, 5, 5, 4, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, ~0U, ~0U, 4, ~0U, ~0U, 4,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 23, ~0U, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, ~0U, ~0U, ~0U, ~0U,
 ~0U, ~0U, ~0U, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, ~0U, ~0U, ~0U,
 ~0U, 23, ~0U, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 16, 16, 16,
 16, 16, 16, 16, 16, 16, 16, 31, 31, 31, 31, 31, 31, 31, 31, 31,
 31, 41, 41, ~0U, ~0U, ~0U, ~0U, ~0U, 31, 31, 31, 31, 31, 31, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 41, ~0U, ~0U, 41, ~0U, ~0U, ~0U, 32,
 32, 32, 32, 32, 32, ~0U, ~0U, ~0U, 31, 31, 31, 31, 31, 31, ~0U, ~0U,
 ~0U, ~0U, ~0U, ~0U, ~0U, 16, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, 32,
 32, 32, 32, 32, 32, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, ~0U,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, ~0U, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, ~0U,
 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
 34, 34, 34, 34, 34, 11, ~0U, ~0U, ~0U, ~0U, 44, ~0U, 44, 44, 44, 44,
 44, 44, 44, 44, 44, 44, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, 11, 44, 44, 44,
 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44,
 44, 44, 44, 44, 44, 44, 44, 11, ~0U, ~0U, ~0U, 44, ~0U, 44, 44, 44,
 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44,
 44, 44, 44, 44, 44, 44, 44, ~0U, 11, 43, 43, ~0U, ~0U, ~0U, ~0U, ~0U,
 ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U,
 43, ~0U, ~0U, 43, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U,
 ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, 43, ~0U, ~0U, ~0U, ~0U,
 ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U,
 ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, 43, 0
};

static yy_state_t yy_default[] = {
 50, 50, 50, 0, 50, 50, 5, 4, 5, 50, 11, 50, 50, 50, 50, 50,
 23, 23, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 23, 23, 23,
 23, 14, 50, 13, 50, 13, 11, 50, 50, 9, 43, 50, 50, 50, 44, 43,
 44, 50, 0
};

static int yy_base[] = {
 0, 1725, 1725, 247, 332, 218, 1725, 1725, 1725, 368, 1725, 1548, 1725, 1045, 789, 1725,
 653, 294, 1725, 1725, 1725, 1725, 1725, 578, 1725, 247, 1725, 1725, 1725, 1725, 304, 663,
 686, 1725, 1301, 1725, 1725, 1725, 1725, 1725, 1725, 712, 1725, 1632, 1516, 1725, 1725, 1725,
 1725, 1725, 1725
};



// MKS LEX prototype scanner code
// Copyright 1991 by Mortice Kern Systems Inc.
// All rights reserved.

// You can redefine YY_INTERACTIVE to be 0 to get a very slightly
// faster scanner:
#ifndef YY_INTERACTIVE
#define	YY_INTERACTIVE	1
#endif

// You can compile with -DYY_DEBUG to get a print trace of the scanner
#ifdef YY_DEBUG
#undef YY_DEBUG
#define YY_DEBUG(fmt,a1,a2)	fprintf(stderr,fmt,a1,a2)
#else
#define YY_DEBUG(fmt,a1,a2)
#endif

const MIN_NUM_STATES = 20;

// Do *NOT* redefine the following:
#define	BEGIN		yy_start =
#define	REJECT		goto yy_reject
#define	yymore()	goto yy_more


#pragma warning(disable: 4102)      // 'xxx' : unreferenced label
#pragma warning(disable: 4309)      // 'initializing' : truncation of constant value
#pragma warning(disable: 4305)      // 'initializing' : truncation from 'const unsigned int' to 'unsigned char'
#pragma hdrstop
#include "stdafx.h"
#include <afx.h>
#include <afxwin.h>

#include "gsd_scan.h"
#include "PrmTextDB.h"
#include "ExtParamDB.h"
#include "Parser.h" 
#include "Gsd_Keyw.h"
#include "gsdmessage.h"

//----  Global Variables:   --------------------------------------------------*
YYSTYPE     yylval;                 // union for tokens between LEX and YACC



// Constructor for yy_scan. Set up tables
#pragma argsused
yy_scan::yy_scan(int sz, char* buf, char* sv, yy_state_t* states)
{
	mustfree = 0;
	if ((size = sz) < MIN_NUM_STATES
	  || (yytext = buf) == 0
	  || (state = states) == 0) {
		yyerror("Bad space for scanner!");
		exit(1);
	}
#ifdef YY_PRESERVE
	save = sv;
#endif
}
// Constructor for yy_scan. Set up tables
yy_scan::yy_scan(int sz)
{
	size = sz;
	yytext = new char[sz+1];	// text buffer
	state = new yy_state_t[sz+1];	// state buffer
#ifdef YY_PRESERVE
	save = new char[sz];	// saved yytext[]
	push = save + sz;
#endif
	if (yytext == NULL
#ifdef YY_PRESERVE
	  || save == NULL
#endif
	  || state == NULL) {
		yyerror("No space for scanner!");
		exit(1);
	}
	mustfree = 1;
	yy_end = 0;
	yy_start = 0;
	yy_lastc = YYNEWLINE;
	yyin = stdin;
	yyout = stdout;
	yylineno = 1;
	yyleng = 0;
}

// Descructor for yy_scan
yy_scan::~yy_scan()
{
	if (mustfree) {
		mustfree = 0;
		delete(yytext);
		delete(state);
#ifdef YY_PRESERVE
		delete(save);
#endif
	}
}

// Print error message, showing current line number
void
yy_scan::yyerror(char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
#ifdef LEX_WINDOWS
	// Windows has no concept of a standard error output!
	// send output to yyout as a simple solution
	if (yylineno)
		fprintf(yyout, "%d: ", yylineno);
	(void) vfprintf(yyout, fmt, va);
	fputc('\n', yyout);
#else /* LEX_WINDOWS */
	if (yylineno)
		fprintf(stderr, "%d: ", yylineno);
	(void) vfprintf(stderr, fmt, va);
	fputc('\n', stderr);
#endif /* LEX_WINDOWS */
	va_end(va);
}


#ifdef LEX_WINDOWS

// The initial portion of the lex scanner
// In an windows environment, it will load the desired
// resources, obtain pointers to them, and then call
// the protected member win_yylex() to acutally begin the
// scanning. When complete, win_yylex() will return a
// value back to our new yylex() function, which will 
// record that value temporarily, release the resources
// from global memory, and finally return the value
// back to the caller of yylex().

int
yy_scan::yylex()
{
	int wReturnValue;
	HANDLE hRes_table;
	unsigned short far *old_yy_la_act;	// remember old pointer values
	short far *old_yy_final;
	yy_state_t far *old_yy_begin;
	yy_state_t far *old_yy_next;
	yy_state_t far *old_yy_check;
	yy_state_t far *old_yy_default;
	short far *old_yy_base;

	// the following code will load the required
	// resources for a Windows based parser. 

	hRes_table = LoadResource (hInst,
		FindResource (hInst, "UD_RES_yyLEX", "yyLEXTBL"));
	
	// return an error code if any
	// of the resources did not load 

	if (hRes_table == (HANDLE)NULL) 
		return (0);
	
	// the following code will lock the resources
	// into fixed memory locations for the scanner
	// (remember previous pointer locations)

	old_yy_la_act = yy_la_act;
	old_yy_final = yy_final;
	old_yy_begin = yy_begin;
	old_yy_next = yy_next;
	old_yy_check = yy_check;
	old_yy_default = yy_default;
	old_yy_base = yy_base;

	yy_la_act = (unsigned short far *)LockResource (hRes_table);
	yy_final = (short far *)(yy_la_act + Sizeof_yy_la_act);
	yy_begin = (yy_state_t far *)(yy_final + Sizeof_yy_final);
	yy_next = (yy_state_t far *)(yy_begin + Sizeof_yy_begin);
	yy_check = (yy_state_t far *)(yy_next + Sizeof_yy_next);
	yy_default = (yy_state_t far *)(yy_check + Sizeof_yy_check);
	yy_base = (short far *)(yy_default + Sizeof_yy_default);


	// call the standard yylex() code

	wReturnValue = win_yylex();

	// unlock the resources

	UnlockResource (hRes_table);

	// and now free the resource

	FreeResource (hRes_table);

	//
	// restore previously saved pointers
	//

	yy_la_act = old_yy_la_act;
	yy_final = old_yy_final;
	yy_begin = old_yy_begin;
	yy_next = old_yy_next;
	yy_check = old_yy_check;
	yy_default = old_yy_default;
	yy_base = old_yy_base;

	return (wReturnValue);
}	// end yylex()

// The actual lex scanner
// yy_sbuf[0:yyleng-1] contains the states corresponding to yytext.
// yytext[0:yyleng-1] contains the current token.
// yytext[yyleng:yy_end-1] contains pushed-back characters.
// When the user action routine is active,
// save contains yytext[yyleng], which is set to '\0'.
// Things are different when YY_PRESERVE is defined. 

int 
yy_scan::win_yylex()

#else /* LEX_WINDOWS */

// The actual lex scanner
// yy_sbuf[0:yyleng-1] contains the states corresponding to yytext.
// yytext[0:yyleng-1] contains the current token.
// yytext[yyleng:yy_end-1] contains pushed-back characters.
// When the user action routine is active,
// save contains yytext[yyleng], which is set to '\0'.
// Things are different when YY_PRESERVE is defined. 
int
yy_scan::yylex()
#endif /* LEX_WINDOWS */

{
	int c, i, yybase;
	unsigned  yyst;		/* state */
	int yyfmin, yyfmax;	/* yy_la_act indices of final states */
	int yyoldi, yyoleng;	/* base i, yyleng before look-ahead */
	int yyeof;		/* 1 if eof has already been read */



#ifdef YYEXIT
	yyLexFatal = 0;
#endif
	yyeof = 0;
	i = yyleng;
	YY_SCANNER();

  yy_again:
	if ((yyleng = i) > 0) {
		yy_lastc = yytext[i-1];	// determine previous char
		while (i > 0)	//	// scan previously token
			if (yytext[--i] == YYNEWLINE)	// fix yylineno
				yylineno++;
	}
	yy_end -= yyleng;		// adjust pushback
	if (yy_end > 0)
		memmove(yytext, yytext+yyleng, (size_t) yy_end);
	i = 0;

  yy_contin:
	yyoldi = i;

	/* run the state machine until it jams */
	yyst = yy_begin[yy_start + ((yy_lastc == YYNEWLINE) ? 1 : 0)];
	state[i] = (yy_state_t) yyst;
	do {
		YY_DEBUG("<state %d, i = %d>\n", yyst, i);
		if (i >= size) {
			YY_FATAL("Token buffer overflow");
#ifdef YYEXIT
			if (yyLexFatal)
				return -2;
#endif
		}	/* endif */

		/* get input char */
		if (i < yy_end)
			c = yytext[i];		/* get pushback char */
		else if (!yyeof && (c = yygetc()) != EOF) {
			yy_end = i+1;
			yytext[i] = c;
		} else /* c == EOF */ {
			c = EOF;		/* just to make sure... */
			if (i == yyoldi) {	/* no token */
				yyeof = 0;
				if (yywrap())
					return 0;
				else
					goto yy_again;
			} else {
				yyeof = 1;	/* don't re-read EOF */
				break;
			}
		}
		YY_DEBUG("<input %d = 0x%02x>\n", c, c);

		/* look up next state */
		while ((yybase = yy_base[yyst]+(unsigned char)c) > yy_nxtmax
		    || yy_check[yybase] != (yy_state_t) yyst) {
			if (yyst == yy_endst)
				goto yy_jammed;
			yyst = yy_default[yyst];
		}
		yyst = yy_next[yybase];
	  yy_jammed: ;
	     state[++i] = (yy_state_t) yyst;
	} while (!(yyst == yy_endst || YY_INTERACTIVE &&
		yy_base[yyst] > yy_nxtmax && yy_default[yyst] == yy_endst));

	YY_DEBUG("<stopped %d, i = %d>\n", yyst, i);
	if (yyst != yy_endst)
		++i;

  yy_search:
	/* search backward for a final state */
	while (--i > yyoldi) {
		yyst = state[i];
		if ((yyfmin = yy_final[yyst]) < (yyfmax = yy_final[yyst+1]))
			goto yy_found;	/* found final state(s) */
	}
	/* no match, default action */
	i = yyoldi + 1;
	output(yytext[yyoldi]);
	goto yy_again;

  yy_found:
	YY_DEBUG("<final state %d, i = %d>\n", yyst, i);
	yyoleng = i;		/* save length for REJECT */
	
	// pushback look-ahead RHS, handling trailing context
	if ((c = (int)(yy_la_act[yyfmin]>>9) - 1) >= 0) {
		unsigned char *bv = yy_look + c*YY_LA_SIZE;
		static unsigned char bits [8] = {
			1<<0, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1<<7
		};
		while (1) {
			if (--i < yyoldi) {	/* no / */
				i = yyoleng;
				break;
			}
			yyst = state[i];
			if (bv[(unsigned)yyst/8] & bits[(unsigned)yyst%8])
				break;
		}
	}

	/* perform action */
	yyleng = i;
	YY_USER();
	switch (yy_la_act[yyfmin] & 0777) {
	case 0:
	{
                                            if (stricmp(yytext, "#Profibus_DP") == 0)
                                            {
                                                /* start scanning in DP text body */
                                                BEGIN(DP_text);
                                                return _PROFIBUS_DP;
                                            }
                                        }
	break;
	case 1:
	{   ; }
	break;
	case 2:
	{   /* skip text until DP marker is found */
                                            ; 
                                        }
	break;
	case 3:
	{
                                            if (stricmp(yytext, "#Profibus_DP") != 0)
                                            {
                                                /* ignoring rest of text */
                                                BEGIN(0);
                                            }
                                        }
	break;
	case 4:
	{   
                                            yycomment("\n");
                                            return _COMMENT;
                                        }
                                            /* just eat comment line */
	break;
	case 5:
	{
                                            // strip leading and trailing quotes!
                                            if (*yytext == _T('\"')) {
                                                yylval.pszText = yytext + 1;
                                            }
                                            else {
                                                yylval.pszText = yytext;
                                            }
                                            int len = _tcslen(yylval.pszText);
                                            if (yylval.pszText[len - 1] == _T('\"')) {
                                                yylval.pszText[len - 1] = _T('\0');
                                            }
                                            return _VISIBLE_STRING;
                                        }
	break;
	case 6:
	{
											yylval.pszText = yytext;
                                            return _SUB_FAMILY;
                                        }
	break;
	case 7:
	{ yycomment("\n"); }     /* eat to join with next line */
	break;
	case 8:
	{
                                            sscanf(yytext,"%d",&yylval.ulVal);
                                            return _INTEGER_LITERAL;
                                        }
	break;
	case 9:
	{
                                            yylval.ulVal = strtol(yytext, NULL, 0);
                                            return _INTEGER_LITERAL;
                                        }
	break;
	case 10:
	{   return '='; }
	break;
	case 11:
	{   return ','; }
	break;
	case 12:
	{   return '('; }
	break;
	case 13:
	{   return ')'; }
	break;
	case 14:
	{   return '-'; }
	break;
	case 15:
	{   return '@'; }
	break;
	case 16:
	{
                                            CGsdKeyword     keywords;  // should be a global instance, init/term when starting/stopping DLL!!!
                                            int             iRevision;
                                            yylval.pszText = yytext;
                                            int iToken = keywords.LookupKeyword(yytext, iRevision);
                                            if (iToken < 0) {
                                                // must be a user definition, forget this line
                                                yyerror("001"); //special keys for localization.
                                                yycomment("\n");
                                                return _USER_KEYWORD;
                                            }
                                            else {
                                                return iToken;
                                            }
                                        }
	break;
	case 17:
	{   
                                            yycomment("\n");
                                        }
                                            /* eat the empty line */
	break;
	case 18:
	{   ; }
                                            /* eat all whitespaces */
	break;
	case 19:
	{   
                                            return CRLF;
                                        }
	break;
	case 20:
	{   
                                            yylval.pszText = yytext;
                                            return _OTHER_CHAR;
                                        }
	break;
	case 21:
	{
#ifdef _DEBUG
                                            yyerror("003");
#endif //_DEBUG
                                        }
	break;


	}
	YY_SCANNER();
	i = yyleng;
	goto yy_again;			/* action fell though */

  yy_reject:
	YY_SCANNER();
	i = yyoleng;			/* restore original yytext */
	if (++yyfmin < yyfmax)
		goto yy_found;		/* another final state, same length */
	else
		goto yy_search;		/* try shorter yytext */

  yy_more:
	YY_SCANNER();
	i = yyleng;
	if (i > 0)
		yy_lastc = yytext[i-1];
	goto yy_contin;
}

/*
 * user callable input/unput functions.
 */
void
yy_scan::yy_reset()
{
	YY_INIT();
	yylineno = 1;
}
/* get input char with pushback */
int
yy_scan::input()
{
	int c;
#ifndef YY_PRESERVE
	if (yy_end > yyleng) {
		yy_end--;
		memmove(yytext+yyleng, yytext+yyleng+1,
			(size_t) (yy_end-yyleng));
		c = save;
		YY_USER();
#else
	if (push < save+size) {
		c = *push++;
#endif
	} else
		c = yygetc();
	yy_lastc = c;
	if (c == YYNEWLINE)
		yylineno++;
	return c;
}

/* pushback char */
int
yy_scan::unput(int c)
{
#ifndef YY_PRESERVE
	if (yy_end >= size) {
		YY_FATAL("Push-back buffer overflow");
	} else {
		if (yy_end > yyleng) {
			yytext[yyleng] = save;
			memmove(yytext+yyleng+1, yytext+yyleng,
				(size_t) (yy_end-yyleng));
			yytext[yyleng] = 0;
		}
		yy_end++;
		save = c;
#else
	if (push <= save) {
		YY_FATAL("Push-back buffer overflow");
	} else {
		*--push = c;
#endif
		if (c == YYNEWLINE)
			yylineno--;
	}	/* endif */
	return c;
}


/*-------------------------- FUNCTION ---------------------------------------*/
CGsdScan::CGsdScan(LPCTSTR pszGsdFile, LPCTSTR pszLogFile /*= NULL*/)
/*>>>>
ctor

I   pszGsdFile:     name of GSD file
I   pszLogFile:     name of log file; if NULL, create temporary name

Result
  void
<<<<*/
  : yy_scan(2000),      // token buffer size
    m_hGsdFile(NULL),
    m_hLogFile(NULL),
    m_bUseTmpFile(FALSE),
    m_lErrorCount(0),
    eof_found(0),
	m_messages()
{
	CString str;

    if (pszLogFile == NULL) {
        // build temporary file name for log file:
        LPTSTR pszTmpName = _ttempnam(NULL, GSD_PARSE_LOGFILE);
        m_sLogFile = pszTmpName;
        free(pszTmpName);
        m_sLogFile += GSD_PARSE_LOGFILE_EXT;
        m_bUseTmpFile = TRUE;
    }
    else {
        m_sLogFile = pszLogFile;
        m_bUseTmpFile = FALSE;
    }
    
	m_hLogFile = ::_tfopen(m_sLogFile, _T("w"));
    ASSERT(m_hLogFile != NULL);

    m_sGsdFile = pszGsdFile;
    str.Format(_T("Parsing GSD file %s ...\n"), (LPCTSTR) m_sGsdFile);
    WriteToLog(str);

    // try opening GSD file:
    m_hGsdFile = ::_tfopen(pszGsdFile, _T("r"));
    if (m_hGsdFile == NULL) {
		//only for the log file.
		GSD_FormatMessage(str, E_GSD_FILE_NOT_FOUND,(LPCSTR)m_sGsdFile);
		WriteToLog(str);
    }
    setinput(m_hGsdFile);
}

/*-------------------------- FUNCTION ---------------------------------------*/
CGsdScan::~CGsdScan() 
/*>>>>
dtor

Result
  void
<<<<*/
{
    if (m_hGsdFile != NULL) {
        ::fclose(m_hGsdFile);
    }
    if (m_hLogFile != NULL) {
        ::fclose(m_hLogFile);
    }
    if (m_bUseTmpFile) {
        _tunlink(m_sLogFile);
    }

	m_messages.RemoveAll();
}

//------------------------------------------------------------------*
/**
 * Print error message, showing current line number if known.
 *
 * @param			fmt: format string (see wsprintf).
 * @param			...	 additional vargs - not used.
 * @see				Message()
*/
void  CGsdScan::yyerror(char *fmt, ...)
{
	try {
		CString strMsg;
		HRESULT hr = S_OK;

		strMsg = fmt;
		if(!strMsg.CompareNoCase("001")) {
			//skip user definition is a warning
			hr = W_GSD_SKIP_USERDEF;
			GSD_FormatMessage(strMsg,hr,m_sGsdFile, yytext);
		}

		if(!strMsg.CompareNoCase("002")) {
			//skip user definition is a warning
			hr = E_GSD_EOF_COMMENT;
			GSD_FormatMessage(strMsg,hr,m_sGsdFile);
		}
		
		if(!strMsg.CompareNoCase("003")) {
			//skip user definition is a warning
			hr = E_GSD_BAD_CHAR;
			GSD_FormatMessage(strMsg,hr,m_sGsdFile,yylineno,yytext);
		}

		if(hr == S_OK) {
			strMsg.Empty();
			if ((yylineno > 0) && (yyleng > 0)) {
				hr = E_GSD_YYERR_FMT1;
				GSD_FormatMessage(strMsg,hr,m_sGsdFile, yylineno, fmt, yytext);
			}
			else {
				hr = E_GSD_YYERR_FMT2;
				GSD_FormatMessage(strMsg,hr,m_sGsdFile, fmt);
			}
		}

		Message(hr,strMsg);
	}
	catch(CMemoryException *e) {
		e->Delete();
	}

}

/*-------------------------- FUNCTION ---------------------------------------*/
int CGsdScan::yygetc(void)
{
    if (eof_found) return EOF;
    int c = getc(yyin);
    if (c==EOF)
    {
        eof_found = 1;
        return '\n';
    }
    return c;
}

//------------------------------------------------------------------*
/**
 * Issues a error/warning message and adds the message to the 
 * error list.
 * @see	yyerror()
*/
void CGsdScan::Message(HRESULT hrMessage, LPCTSTR pszMessage)
{
	try {
		CString strMsg = pszMessage;
		VARIANT_BOOL bError;
		ParseMessage * pmsg = NULL;

		WriteToLog(pszMessage);

		pmsg = ::CreateParseMessage(hrMessage,pszMessage);
		if(!pmsg) {
			return;
		}

		m_messages.Add(pmsg);

		pmsg->IsError(&bError);
		if(bError == VARIANT_TRUE) {
			m_lErrorCount++;	
		}
	}
	catch(CMemoryException* e) {
		e->Delete();
	}
}

/*-------------------------- FUNCTION ---------------------------------------*/
void
    CGsdScan::WriteToLog(LPCTSTR pszLine)
/*>>>>
write line to log file

I   pszLine

Result
  void
<<<<*/
{
    if (m_hLogFile != NULL) {
        _fputts(pszLine, m_hLogFile);
        _fputtc(_T('\n'), m_hLogFile);
        fflush(m_hLogFile);
    }
}

void CGsdScan::SetMessages(CStationCallback *pCB)
{
	int iCount = m_messages.GetCount();
	for(int i = 0; i< iCount; i++) {
		//[] operator has implicit addref.
		ParseMessage * pMessage = m_messages[i]; 
		if(pMessage) {
			pCB->AddMessage(pMessage);
		}
	}

	pCB->SetErrorCount(m_lErrorCount);
} 

#ifdef _SHOW_TOKENS
/*-------------------------- FUNCTION ---------------------------------------*/
void
    CGsdScan::ShowToken(int token)
{
    static int iOldLine = -1;

    ASSERT(m_hLogFile != NULL);
    if (iOldLine != yylineno) {
        _ftprintf(m_hLogFile, _T("\n*TOK: line %d\n** TOK: "), yylineno);
        iOldLine = yylineno;
    }

    switch (token) {
    case _PROFIBUS_DP:
        _ftprintf(m_hLogFile, _T("<PROFIBUS_DP>\n"));
        break;
    case _INTEGER_LITERAL:
        _ftprintf(m_hLogFile, _T("<INTEGER_LITERAL=%d>"), yylval.ulVal);
        break;
    case CRLF:
        _ftprintf(m_hLogFile, _T("<CRLF>\n"));
        break;
    case _COMMENT:
        _ftprintf(m_hLogFile, _T("<COMMENT>\n"));
        break;
    case _VISIBLE_STRING:
        _ftprintf(m_hLogFile, _T("<VISIBLE_STRING=%s>"), yylval.pszText);
        break;
    case _USER_KEYWORD:
        _ftprintf(m_hLogFile, _T("<USER KEYWORD=%s>"), yylval.pszText);
        break;
    case '=':
    case ',':
    case '(':
    case ')':
    case '-':
        _ftprintf(m_hLogFile, _T("<\"%c\">"), token);
        break;
    default:
        {
        CGsdKeyword keywords;
        LPCSTR pszKeyword = keywords.KeywordFromToken(token);
        if (pszKeyword != NULL) {
            _ftprintf(m_hLogFile, _T("<%s (%d)>"), pszKeyword, token);
        }
        else {
            _ftprintf(m_hLogFile, _T("<!!ERROR!! Unknown Token\"%c\" (%d)>"), token, token);
        }
        }
        break;
    }
    fflush(m_hLogFile);
}
#endif // _SHOW_TOKENS


/*-------------------------- FUNCTION ---------------------------------------*/
void
    yy_scan::yycomment(char *const mat)
// skip comment input (see MKSlib)
{
    register int c;
    register char *cp;

    for (cp = mat; *cp != 0; ) {
        c = input();
        if (c == EOF) {
            yyerror("002");
            break;
        }
        if (c != *cp++) {
            cp = mat;
            if (c == *cp)
                cp++;
        }
    }
}



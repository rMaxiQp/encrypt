#include <ctime.h>

static int round;

static void subbytes(unsigned char state[4][4]);

static void shift_rows(unsigned char state[4][4]);

//s'(x) = c(x) * s(x)mod(x^4 + 1)
static void mix_columns(unsigned char state[4][4]);

static void add_round_key(unsigned char state[4][4], unsigned char w[4][4]);

static void key_expansion(unsigned char w[4][4], int count);

static void inv_subbytes(unsigned char state[4][4]);

static void inv_shift_rows(unsigned char state[4][4]);

static void inv_mix_columns(unsigned char state[4][4]);

static void AES_encrypt(unsigned char state[][N], unsigned char round_key[][N]);

static void AES_decrypt(unsigned char state[][N], unsigned char w[][N]);

static void print_state(unsigned char state[4][4]);

#include <stdio.h>

#define BYTE 8

const static int IP_Table[64] = {
  58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
  62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
  57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
  61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

const static int P_Table[32] = {
  16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
  2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
};

const static int PC1_Table[56] = {
  57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
  10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
  63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
  14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
};

const static int PC2_Table[48] = {
  14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
  23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
  41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
  44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};


static void Char_To_Bit(const char input[], int output[], int bits);

static void Bit_To_Char(const int input[], char output[], int bits);

//similar to E(), P(), IP_IN()
static void IP(const int input[64], int output[64], int table[64]);

static void F_func(int input[32], int output[32], int subkey[48]);

//similar to IP(), P(), IP_IN()
static void E(const int input[32], int output[48], int table[48]);

static void XOR(int *INA, int *INB, int len);

static void S(const int input[48], int output[32], int table[8][4][16]);

//similar to IP(), E(), IP_IN()
static void P(const int input[32], int output[32], int table[32]);

//similar to P(), E(), IP()
static void IP_In(const int input[64], int output[64], int table[64]);

static void subkey_func(const int input[64], int subkey[16][48]);

static void PC_1(const int input[64], int output[56], int table[56]);

static void rotate_L(const int input[28], int output[28], int left_count);

static void PC_2(const int input[56], int output[48], int table[48]);

static void DES_Efun(char input[8], char key_in[8], int output[64]);

static void DES_Dfun(int input[64], char key_in[8], char output[8]);

#include "DES.h"

static void Char_To_Bit(const char input[], int output[], int bits) {
  int temp, i, j;
  for (i = 0; i < BYTE; i++) {
    temp = (int)input[i];
    for (j = i * BYTE + 7; j >= i * BYTE; j--) {
      output[j] = temp % 2;
      temp /= 2;
    }
  }
}

static void Bit_To_Char(const int input[], char output[], int bits) {
  int i, j;
  for (j = 0; j < BYTE; j++) {
    for (i = 0; i < 8; i++) {
      output[j] = output[j] * 2 + intput[i + BYTE * j];
    }
  }
}

static void IP(const int input[64], int output[64], int table[64]) {
  int i;
  for (i = 0; i < 64; i++) {
    output[i] = input[table[i] - 1];
  }
}

static void F_func(int input[32], int output[32], int subkey[48]) {
  int len = 48;
  int temp[48] = {0};
  int temp_1[32] = {0};
  mix(input, temp, E_Table, 48);//E(input, temp, E_Table);
  XOR(temp, subkey, len);
  S(temp, temp_1, S_BOX);
  mix(temp_1, output, P_TABLE, 32);//P(temp_1, output, P_TABLE);
}

static void E(const int input[32], int output[48], int table[48]) {
  int i;
  for (i = 0; i < 48; i++) {
    output[i] = input[table[i] - 1];
  }
}

static void XOR(int *INA, int *INB, int len) {
  int i;
  for (i = 0; i < len; i++) {
    *(INA + i) ^= *(INB + i);
  }
}

static void S(const int input[48], int output[32], int table[8][4][16]) {
  int i = 0;
  int j = 0;
  int INT[8];
  for (; i < 48; i += 6) {
    INT[j] = table[j][2 * input[i] + input[i + 5]][2 * 2 * 2 * input[i + 1] + 2 * 2 * input[i + 2] + 2 * input[i + 3] + input[i + 4]];
    j++;
  }
  for (i = 0; i < BYTE; i++) {
    j = 4 * (i + 1) - 1;
    while(INT[i]) {
      output[j] = INT[i] % 2;
      INT[i] = INT[i] / 2;
    }
  }
}

static void P(const int input[32], int output[32], int table[32]) {
  int i;
  for (i = 0; i < 32; i++) {
    output[i] = input[table[i] - 1];
  }
}

static void IP_In(const int input[64], int output[64], int table[64]) {
  int i;
  for (i = 0; i < 64; i++) {
    output[i] = input[table[i] - 1];
  }
}

static void subkey_func(const int input[64], int subkey[16][48]) {
  int loop = 1;
  int loop_2 = 2;
  int i,j;
  int c[28], d[28];
  int pc_1[56] = {0};
  int pc_2[16][56] = {0};
  int rotate_c [16][28] = {0};
  int rotate_d [16][28] = {0};
  mix(input, pc_1, PC1_Table, 56);//PC_1(input, pc_1, PC1_Table);
  for (i = 0; i < 28; i++) {
    c[i] = pc_1[i];
    d[i] = pc_1[i + 28];
  }
  int left_count = 0;
  for (i = 1; i < 17; i++) {
    if( i == 1 || i == 2 || i == 9 || i == 16)
      left_count += loop;
    else
      left_count += loop_2;

    rotate_L(c, rotate_c[i - 1], left_count);
    rotate_L(d, rotate_d[i - 1], left_count);
  }

  for (i = 0; i < 16; i++) {
    for (j = 0; j < 28; j++) {
      pc_2[i][j] = rotate_c[i][j];
      pc_2[i][j + 28] = rotate_d[i][j];
    }
  }

  for (i = 0; i < 16; i++) {
    mix(pc_2[i], subkey[i], PC2_Table, 48);//PC_2(pc_2[i], subkey[i], PC2_Table);
  }
}

static void PC_1(const int input[64], int output[56], int table[56]) {
  int i;
  for (i = 0; i < 56; i++) {
    output[i] = input[table[i] - 1];
  }
}

static void rotate_L(const int input[28], int output[28], int left_count) {
  int len = 28;
  int i;
  for (i = 0; i < len; i++) {
    output[i] = input[(i + left_count) % len];
  }
}

static void PC_2(const int input[56], int output[48], int table[48]) {
  int i;
  for (i = 0; i < 48; i++) {
    output[i] = input[table[i] - 1];
  }
}

static void DES_Efun(char input[8], char key_in[8], int output[64]) {
  int Ip[64] = {0};
  int output_l[64] = {0};
  int subkey[16][48];
  int chartobit[64] = {0};
  int key[64];
  int n;
  int l[17][32], r[17][32];
  Char_To_Bit(input, chartobit, 8);
  mix(chartobit, Ip, IP_Table, 64);//IP(chartobit, Ip, IP_Table);
  Char_To_Bit(key_in, key, 8);
  subkey_func(key, subkey);
  for (int i = 0; i < 32; i++) {
    l[0][i] = Ip[i];
    r[0][i] = Ip[i + 32];
  }
  for (int j = 1; j < 16; j++) {
    for (int k = 0; k < 32; k++) {
      l[j][k] = r[j - 1][k];
    }
    F_func(r[j - 1], r[j], subkey[j - 1]);
    XOR(r[j], l[j-1], 32);
  }

  int t;
  for (t = 0; t < 32; t++) {
    r[16][t] = r[15][t];
  }
  F_func(r[15], l[16], subkey[15]);
  XOR(l[16], l[15], 32);
  for (t = 0; t < 32; t++) {
    output_l[t] = l[16][t];
    output_l[t + 32] = r[16 + t];
  }
  mix(output_l, output, IP_Table_Inv, 64);//IP_In(output_l, output, IP_Table_Inv);
}

static void DES_Dfun(int input[64], char key_in[8], char output[8]) {
  int Ip[64] = {0};
  int output_1[64] = {0};
  int output_2[64] = {0};
  int subkey[16][48];
  int chartobit[64] = {0};
  int key[64];
  int l[17][32], r[17][32];

  mix(input, Ip, IP_Table, 64);//IP(input, Ip, IP_Table);

  Char_To_Bit(key_in, key, 8);
  subkey_func(key, subkey);
  for (int i = 0; i < 32; i++) {
    l[0][i] = Ip[i];
    r[0][i] = Ip[i + 32];
  }

  for (int j = 1; j < 16; j++) {

    for (int k = 0; k < 32; k++) {
      l[j][k] = r[j - 1][k];
    }

    F_func(r[j - 1], r[j], subkey[j - 1]);
    XOR(r[j], l[j-1], 32);
  }

  int t;
  for (t = 0; t < 32; t++) {
    r[16][t] = r[15][t];
  }

  F_func(r[15], l[16], subkey[0]);
  XOR(l[16], l[15], 32);

  for (t = 0; t < 32; t++) {
    output_l[t] = l[16][t];
    output_l[t + 32] = r[16 + t];
  }

  mix(output_l, output_2, IP_Table_Inv, 64);//IP_In(output_l, output_2, IP_Table_Inv);
  Bit_To_Char(output_2, output, 8);
}

static void mix(const int input[], int output[], int table[], int len) {
  int i;
  for (i = 0; i < len; i++) {
    output[i] = input[table[i] - 1];
  }
}

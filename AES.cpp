#include "AES.h"

static void subbytes(unsigned char state[4][4]) {
  printf("after subbytes:\n");
  for (int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      state[i][j] = sbox[state[i][j]];
    }
  }

  print_state(state);
}

static void shift_rows(unsigned char state[4][4]) {
  printf("after shift_rows:\n");
  unsigned char k = state[1][0];
  state[1][0] = state[1][1];
  state[1][1] = state[1][2];
  state[1][2] = state[1][3];
  state[1][3] = k;

  k = state[2][0];
  state[2][0] = state[2][2];
  state[2][2] = k;

  k = state[2][1];
  state[2][1] = state[2][3];
  state[2][3] = k;

  k = state[3][0];
  state[3][0] = state[3][3];
  state[3][3] = state[3][2];
  state[3][2] = state[3][1];
  state[3][1] = k;

  print_state(state);
}

static void mix_columns(unsigned char state[4][4]) {
  printf("after mix_columns:\n");
  unsigned char temp[2] = {0};
  for(int i = 0; i < 4; i++) {
    k = state[0][i];
    temp[0] = state[0][i] ^ state[1][i] ^ state[2][i] ^ state[3][i];

    temp[1] = state[0][i] ^ state[1][i];
    temp[1] = xtime(temp[1]);
    state[0][i] ^= temp[1] ^ temp[0];

    temp[1] = state[1][i] ^ state[2][i];
    temp[1] = xtime(temp[1]);
    state[1][i] ^= temp[1] ^ temp[0];

    temp[1] = state[2][i] ^ state[3][i];
    temp[1] = xtime(temp[1]);
    state[2][i] ^= temp[1] ^ temp[0];

    temp[1] = state[3][i] ^ k;
    temp[1] = xtime(temp[1]);
    state[3][i] ^= temp[1] ^ temp[0];
  }

  print_state(state);
}

static void add_round_key(unsigned char state[4][4], unsigned char w[4][4]) {

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[i][j] ^= w[i][j];
    }
  }

  print_state(state);
}

static void key_expansion(unsigned char w[4][4], int count) {
  printf("after key_expansion: \n");

  unsigned char rotword[4] = {0};
  unsigned char subword[4] = {0};

  for (int i = 4; i < 8; i++) {
    if( (i % 4) == 0 ) {
      printf("rotword(): ");
      rotword[0] = w[1][i - 1];
      printf("%02x  ", rotword[0]);
      rotword[1] = w[2][i - 1];
      printf("%02x  ", rotword[1]);
      rotword[2] = w[3][i - 1];
      printf("%02x  ", rotword[2]);
      rotword[3] = w[0][i - 1];
      printf("%02x  \n\nsubwords(): ", rotword[3]);

      for (int j = 0; j < 4; j++) {
        subword[j] = sbox[rotword[j]];
        printf("%02x  ", subword[j]);
      }

      printf("\n\n\nafter ^Rcon(): ");
      for (int j = 0; j < 4; j++) {
        rcon[j] = subword[j] ^ Rcon[N][j];
        printf("%02x  ", rcon[j]);
      }

      printf("\n\nw[%d] :" count);
      for (int j = 0; j < 4; j++) {
        w[j][i % 4] = rcon[j] ^ w[j][i - 4];
        printf("%02x  ", w[j][i % 4]);
      }
      count++;

    }
    else {
      printf("w[%d]: ", count);
      for (int j = 0; j < 4; j++) {
        w[j][i % 4] = w[j][i % 4] ^ w[j][(i % 4) - 1];
      }
      count++;
    }
  }

  printf("round_key:\n");
  print_state(w);
}

static void inv_subbytes(unsigned char state[4][4]) {
  printf("after inv_subbytes:\n");

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[i][j] = rsbox[state[i][j]];
    }
  }

  print_state(state);
}

static void inv_shift_rows(unsigned char state[4][4]) {
  printf("after inv_shift_rows:\n");
  unsigned char k = state[1][3];
  state[1][3] = state[1][2];
  state[1][2] = state[1][1];
  state[1][1] = state[1][0];
  state[1][0] = k;

  k = state[2][0];
  state[2][0] = state[2][2];
  state[2][2] = k;

  k = state[2][1];
  state[2][1] = state[2][3];
  state[2][3] = k;

  k = state[3][0];
  state[3][0] = state[3][1];
  state[3][1] = state[3][2];
  state[3][2] = state[3][3];
  state[3][3] = k;
  print_state(state);
}

static void inv_mix_columns(unsigned char state[4][4]) {
  printf("after inv_mix_columns:\n");
  unsigned char temp[4] = {0};

  for(int i = 0; i < 4; i++) {
    temp[0] = state[0][i];
    temp[1] = state[1][i];
    temp[2] = state[2][i];
    temp[3] = state[3][i];

    state[0][i] = multiply(temp[0], 0x0e) ^ multiply(temp[1], 0x0b) ^ multiply(temp[2], 0x0d) ^ multiply(temp[3], 0x09);
    state[1][i] = multiply(temp[0], 0x09) ^ multiply(temp[1], 0x0e) ^ multiply(temp[2], 0x0b) ^ multiply(temp[3], 0x0d);
    state[2][i] = multiply(temp[0], 0x0d) ^ multiply(temp[1], 0x09) ^ multiply(temp[2], 0x0e) ^ multiply(temp[3], 0x0b);
    state[3][i] = multiply(temp[0], 0x0b) ^ multiply(temp[1], 0x0d) ^ multiply(temp[2], 0x09) ^ multiply(temp[3], 0x0e);
  }

  print_state(state);
}

static void AES_encrypt(unsigned char state[][N], unsigned char round_key[][N]) {
  unsigned char message[16] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
  unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xef, 0x4f, 0x3c};

  unsigned char state[4][4];
  unsigned char w[4][4];

  int m = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[j][i] = message[m];
      w[j][i] = key[m];
      m++;
    }
  }

  add_round_key(state, w);
  for (round = 2; rount < 11; round++) {
    printf("%d round encryption:\n", round);
    subbytes(state);
    shift_rows(state);
    mix_columns(state);
    key_expansion(w, round);
    add_round_key(state, w);
  }

  subbytes(state);
  shift_rows(state);
  key_expansion(w, 10);
  add_round_key(state, w);
}

static void AES_decrypt(unsigned char state[][N], unsigned char w[][N]) {
  unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3e};
  unsigned char cipher[16] = {0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32};

  unsigned char state[4][4];
  unsigned char w[4][4];

  int m = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[j][i] = cipher[m];
      w[j][i] = key[m];
      m++;
    }
  }

  key_expansion(w, round);
  add_round_key(state, w);

  for (int i = 9; i > 0; i--) {
    inv_shift_rows(state);
    inv_subbytes(state);
    key_expansion(w, round);
    add_round_key(state, w);
    inv_mix_columns(state);
  }

  inv_shift_rows(state);
  inv_subbytes(state);
  key_expansion(w, 0);
  add_round_key(state, w);
}


static void print_state(unsigned char state[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printf("\t\t%02x  \n", state[i][j]);
    }
  }
}

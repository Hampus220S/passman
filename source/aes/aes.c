#include "../aes.h"
#include "aes-intern.h"

/*
 * SubBytes
 */
void sub_bytes(uint8_t block[16])
{
  for(uint8_t index = 0; index < 16; index++)
  {
    block[index] = sbox[block[index]];
  }
}

/*
 * SubBytes - inverse
 */
void sub_bytes_inverse(uint8_t block[16])
{
  for(uint8_t index = 0; index < 16; index++)
  {
    block[index] = sbox_inv[block[index]];
  }
}

void bytes_switch(uint8_t block[16], uint8_t a, uint8_t b)
{
  uint8_t temp = block[a];

  block[a] = block[b];

  block[b] = temp;
}

/*
 * ShiftRows
 */
void shift_rows(uint8_t block[16])
{
  bytes_switch(block, 4, 5);
  bytes_switch(block, 4, 6);
  bytes_switch(block, 4, 7);

  bytes_switch(block, 8, 10);
  bytes_switch(block, 9, 11);

  bytes_switch(block, 12, 13);
  bytes_switch(block, 12, 14);
  bytes_switch(block, 12, 15);
}

/*
 * ShiftRows - inverse
 */
void shift_rows_inverse(uint8_t block[16])
{
  bytes_switch(block, 4, 7);
  bytes_switch(block, 4, 6);
  bytes_switch(block, 4, 5);

  bytes_switch(block, 9, 11);
  bytes_switch(block, 8, 10);

  bytes_switch(block, 12, 15);
  bytes_switch(block, 12, 14);
  bytes_switch(block, 12, 13);
}

#define ROW(a, b, c, d)     (a        ^ b         ^ mult2[c]  ^ mult3[d])
#define ROW_INV(a, b, c, d) (mult9[a] ^ mult11[b] ^ mult13[c] ^ mult14[d])

/*
 * MixColumns
 */
void mix_columns(uint8_t block[16])
{
  for(uint8_t column = 0; column < 4; column++)
  {
    uint8_t a = block[column];
    uint8_t b = block[column +  4];
    uint8_t c = block[column +  8];
    uint8_t d = block[column + 12];

    block[column]      = ROW(c, d, a, b);
    block[column +  4] = ROW(d, a, b, c);
    block[column +  8] = ROW(a, b, c, d);
    block[column + 12] = ROW(b, c, d, a);
  }
}

/*
 * MixColumns - inverse
 */
void mix_columns_inverse(uint8_t block[16])
{
  for(uint8_t column = 0; column < 4; column++)
  {
    uint8_t a = block[column];
    uint8_t b = block[column +  4];
    uint8_t c = block[column +  8];
    uint8_t d = block[column + 12];

    block[column]      = ROW_INV(d, b, c, a);
    block[column +  4] = ROW_INV(a, c, d, b);
    block[column +  8] = ROW_INV(b, d, a, c);
    block[column + 12] = ROW_INV(c, a, b, d);
  }
}

/*
 * AddRoundKey
 */
void add_round_key(uint8_t block[16], const uint8_t rkey[16])
{
  for(uint8_t index = 0; index < 16; index++)
  {
    block[index] ^= rkey[index];
  }
}

/*
 *
 */
void aes_block_encrypt(uint8_t result[16], const uint8_t input[16], const uint8_t* rkeys, uint8_t rounds)
{
  uint8_t block[16];
  memcpy(block, input, 16);

  add_round_key(block, rkeys);

  for(int index = 1; index < (rounds - 2); index++)
  {
    sub_bytes(block);

    shift_rows(block);

    mix_columns(block);
    
    add_round_key(block, rkeys + index * 16);
  }

  sub_bytes(block);

  shift_rows(block);

  add_round_key(block, rkeys + 16 * (rounds - 1));

  memcpy(result, block, 16);
}

/*
 *
 */
void aes_block_decrypt(uint8_t result[16], const uint8_t input[16], const uint8_t* rkeys, uint8_t rounds)
{
  uint8_t block[16];
  memcpy(block, input, 16);

  add_round_key(block, rkeys + 16 * (rounds - 1));

  shift_rows_inverse(block);

  sub_bytes_inverse(block);

  for(uint8_t index = (rounds - 2); index-- > 1;)
  {
    add_round_key(block, rkeys + 16 * index);

    mix_columns_inverse(block);

    shift_rows_inverse(block);

    sub_bytes_inverse(block);
  }
  
  add_round_key(block, rkeys);

  memcpy(result, block, 16);
}

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Failed to expand key
 */
int aes_encrypt(void* result, const void* message, size_t size, const char* key, ksize_t ksize)
{
  uint8_t rounds = ROUND_KEYS(ksize);

  uint32_t rkeys[4 * rounds];
  if(key_expand(rkeys, (uint32_t*) key, ksize) != 0) return 1;

  size_t index;
  uint8_t block[16];

  for(index = 0; index < (size - 16); index += 16)
  {
    memcpy(block, (uint8_t*) message + index, 16);

    aes_block_encrypt(result + index, block, (uint8_t*) rkeys, rounds);
  }

  memset(block, 0, 16);
  memcpy(block, (uint8_t*) message + index, size - index);

  aes_block_encrypt(result + index, block, (uint8_t*) rkeys, rounds);

  return 0; // Success!
}

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Failed to expand key
 */
int aes_decrypt(void* result, const void* message, size_t size, const char* key, ksize_t ksize)
{
  uint8_t rounds = ROUND_KEYS(ksize);

  uint32_t rkeys[4 * rounds];
  if(key_expand(rkeys, (uint32_t*) key, ksize) != 0) return 1;

  size_t index;
  uint8_t block[16];

  for(index = 0; index < (size - 16); index += 16)
  {
    memcpy(block, (uint8_t*) message + index, 16);

    aes_block_decrypt(result + index, block, (uint8_t*) rkeys, rounds);
  }

  memset(block, 0, 16);
  memcpy(block, (uint8_t*) message + index, size - index);

  aes_block_decrypt(result + index, block, (uint8_t*) rkeys, rounds);

  return 0; // Success!
}

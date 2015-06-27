#include <stdint.h>
#include "aes.h"

// this looks more like pseudo code notes right now

// Nb = 4           ; number of 32 bit words in the state
// Nk = 4, 6, or 8  ; number of 32 bit words in the Cipher key
// Nr = 10, 12, 14  ; number of rounds to be performed

// this is the function that takes a block of plaintext and the key schedule
// and encrypts it. the next level of abstraction would be to implement block
// modes using this function on each block
void EncryptBlock(uint8_t* block, uint8_t* key_schedule)
{
    // the state is always 128 bits for AES. we are going to represent that as
    // an array of 16 bytes. conceptually it can be useful to think of it as a
    // 4x4 matrix
    //
    // step 1 of the cipher is to initialize the state as the input block of
    // plaintext. we are just going to operate directly on the input block

    // step 2 is to do an initial round key addition. the first round key is
    // added by a simple bitwise xor operation
    AddRoundKey(block, key_schedule[0, (4*4) - 1]);

    // step 3 is Nr-1 rounds where Nr is the total number of rounds we will be
    // performing. Nr is 10, 12, and 14 for keysizes of 128, 192, and 256
    // respectively.
    for (uint8_t round = 0; round < (10 - 1); round++) {

        // the round function consists of four operations
        //
        // SubBytes subsitutes bytes in the state based on the standardized
        // substitution boxes or S-Boxes
        SubBytes(block);

        // ShiftRows cyclically shifts each of the last three rows of state
        // over by a different offset
        ShiftRows(block);

        // MixColumns does some math on the state, column by column
        MixColumns(block);

        // finally, we add the next round key to the state
        AddRoundKey(block, key_schedule[round * (4*4), (round + 1) * 4 - 1]);
    }

    // step 4 is the final round. the only difference is that we do not
    // perform the MixColumns operation on this one
    SubBytes(block);
    ShiftRows(block);
    AddRoundKey(block, key_schedule[10 * (4*4), (10 + 1) * (4*4) - 1]);

    // all of that fiddling with the state leaves us with the encrypted
    // block
}

void SubBytes(uint8_t* state)
{
    // replace each byte of state with the appropriate byte in the s-box
    for (uint8_t i = 0; i < (4*4); i++) {
        state[i] = s[state[i]];
    }
}

void AddRoundKey(uint8_t* state, uint32_t* round_key)
{
    // XOR each column of the state with a word (4 bytes) from the round
    // key
    for (uint8_t c = 0; c < 4; c++) {
        // TODO this does not seem like it will do what I am trying to do here
        // I should start actually running this to test it...
        state[0+c] ^= (round_key[c] & 0xFF000000);
        state[4+c] ^= (round_key[c] & 0xFF0000);
        state[8+c] ^= (round_key[c] & 0xFF00);
        state[12+c] ^= (round_key[c] & 0xFF);
    }
}

void ShiftRows(uint8_t* state)
{
    for (uint8_t c = 0; c < 4; c++) {
        state[4+c] = state[(4+(c+1)) % 4];
    }

    for (uint8_t c = 0; c < 4; c++) {
        state[8+c] = state[(8+(c+2)) % 4];
    }

    for (uint8_t c = 0; c < 4; c++) {
        state[12+c] = state[(12+(c+3)) % 4];
    }
}

void MixColumns(uint8_t* state)
{
    for (uint8_t c = 0; c < 4; c++ ) {
        for (uint8_t r = 0; r < 4; r++) { 
            state[(4*r) + c] = (0x02 * state[(4*r) + c]) ^ (0x03 * state[(4*(r+1)) + c]) ^ state[(4*(r+2)) + c] ^ state[(4*(r+3)) + c];
        }
    }
}

void KeyExpansion(uint8_t* key, uint32_t* key_schedule, uint8_t Nk)
{
    uint32_t temp;
    uint8_t i = 0;

    for (; i < Nk; i++) {
        key_schedule[i] = word(key[4 * i], key[(4 * i) + 1], key[(4 * i) + 2], key[(4 * i) + 3]);
    }

    i = Nk;

    // 4 is Nb
    // 10 is Nr
    // TODO figure out Nr based on Nk
    for (; i < (4 * (10 + 1)); i++) {

        temp = key_schedule[i-1];

        if ((i % Nk) == 0) {
            RotWord(temp);
            SubWord(temp);
            temp ^= Rcon[i/Nk];
        }
        else if ((Nk > 6) && ((i % Nk) == 4)) {
            SubWord(temp);
        }

        key_schedule[i] = key_schedule[i-1] ^ temp;
    }
}

void SubWord(uint32_t in)
{
    uint8_t the_bytes[4];

    bytes(in, the_bytes);
    SubBytes(the_bytes);

    in = word(the_bytes[0], the_bytes[1], the_bytes[2], the_bytes[3]);
}

void RotWord(uint32_t in)
{
    uint8_t out[4];
    uint8_t temp;

    bytes(in, out);

    temp = out[0];
    out[0] = out[1];
    out[1] = out[2];
    out[2] = out[3];
    out[3] = temp;

    in = word(out[0], out[1], out[2], out[3]);
}

uint32_t word(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    uint32_t the_word;
    
    the_word = (a << 24);
    the_word = the_word & (b << 16);
    the_word = the_word & (c << 8);
    the_word = the_word & (d);

    return the_word;
}

void bytes(uint32_t in, uint8_t* out)
{
    out[0] = (in >> 24) & 0xFF;
    out[1] = (in >> 16) & 0xFF;
    out[2] = (in >> 8) & 0xFF;
    out[3] = in & 0xFF;
}

int main(void)
{
    uint8_t block[16] = {0};
    uint8_t key[16] = {0};
    uint8_t Nk = 4;

    uint32_t key_schedule[4];

    KeyExpansion(key, key_schedule, Nk);
    EncryptBlock(block, key_schedule);

}

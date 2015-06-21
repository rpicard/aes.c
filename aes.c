// this looks more like pseudo code notes right now

// Nb = 4           ; number of 32 bit words in the state
// Nk = 4, 6, or 8  ; number of 32 bit words in the Cipher key
// Nr = 10, 12, 14  ; number of rounds to be performed

// this is the function that takes a block of plaintext and the key schedule
// and encrypts it. the next level of abstraction would be to implement block
// modes using this function on each block
void EncryptBlock(uint8_t[16] in, uint8_t[16] out, uint8_t[] key_schedule)
{
    // the state is always 128 bits for AES. we are going to represent that as
    // an array of 16 bytes. conceptually it can be useful to think of it as a
    // 4x4 matrix
    uint8_t[16] state;

    // step 1 of the cipher is to initialize the state as the input block of
    // plaintext
    state = in;

    // step 2 is to do an initial round key addition. the first round key is
    // added by a simple bitwise xor operation
    AddRoundKey(state, key_schedule[0, Nb_bytes-1]);

    // step 3 is Nr-1 rounds where Nr is the total number of rounds we will be
    // performing. Nr is 10, 12, and 14 for keysizes of 128, 192, and 256
    // respectively.
    for (uint8_t round = 0; i < (Nr - 1); i++) {

        // the round function consists of four operations
        //
        // SubBytes subsitutes bytes in the state based on the standardized
        // substitution boxes or S-Boxes
        SubBytes(state);

        // ShiftRows cyclically shifts each of the last three rows of state
        // over by a different offset
        ShiftRows(state);

        // MixColumns does some math on the state, column by column
        MixColumns(state);

        // finally, we add the next round key to the state
        AddRoundKey(state, key_schedule[round * Nb_bytes, (round + 1) * Nb - 1]);
    }

    // step 4 is the final round. the only difference is that we do not
    // perform the MixColumns operation on this one
    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, key_schedule[Nr * Nb_bytes, (Nr + 1) * Nb_bytes - 1]);

    // all of that fiddling with the state leaves us with the encrypted
    // block
    out = state;
}

void SubBytes(uint8_t[] state)
{
    // replace each byte of state with the appropriate byte in the s-box
    for (uint8_t i = 0; i < (Nb_bytes); i++) {
        state[i] = s[state[i]];
    }
}

void AddRoundKey(uint8_t[16] state, uint32_t[4] round_key)
{
    // XOR each column of the state with a word (4 bytes) from the round
    // key
    for (uint8_t c = 0; i < Nb; i++) {
        // TODO this does not seem like it will do what I am trying to do here
        // I should start actually running this to test it...
        state[0+c] ^= (round_key[i] && 0xFF000000);
        state[4+c] ^= (round_key[i] && 0xFF0000);
        state[8+c] ^= (round_key[i] && 0xFF00);
        state[12+c] ^= (round_key[i] && 0xFF);
    }
}

void ShiftRows(uint8_t[16] state)
{
    for (uint8_t c = 0; c < Nb; c++) {
        state[4+c] = state[(4+(c+1)) % Nb];
    }

    for (uint8_t c = 0; c < Nb; c++) {
        state[8+c] = state[(8+(c+2)) % Nb];
    }

    for (uint8_t c = 0; c < Nb; c++) {
        state[12+c] = state[(12+(c+3)) % Nb];
    }
}

void MixColumns(uint8_t[16] state)
{
    for (uint8_t c = 0; c < Nb; c++ ) {
        for (uint8_t r = 0; r < Nb; r++) { 
            state[(4*r) + c] = (0x02 * state[(4*r) + c]) ^ (0x03 * state[(4*(r+1)) + c]) ^ state[(4*(r+2)) + c] ^ state[(4*(r+3)) + c];
        }
    }
}

int main(void)
{
    uint8_t[16] in_block = {0};
    uint8_t[16] out_block;

}

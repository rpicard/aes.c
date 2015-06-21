// this looks more like pseudo code notes right now

// Nb = 4           ; number of 32 bit words in the state
// Nk = 4, 6, or 8  ; number of 32 bit words in the Cipher key
// Nr = 10, 12, 14  ; number of rounds to be performed

// this is the function that takes a block of plaintext and the key schedule
// and encrypts it. the next level of abstraction would be to implement block
// modes using this function on each block
void EncryptBlock(uint8_t[16] in, uint8_t[16] out, uint32_t[] key_schedule)
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
    AddRoundKey(state, key_schedule[0, Nb-1]);

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
        AddRoundKey(state, key_schedule[round * Nb, (round + 1) * Nb - 1]);
    }

    // step 4 is the final round. the only difference is that we do not
    // perform the MixColumns operation on this one
    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, key_schedule[Nr * Nb, (Nr + 1) * Nb - 1]);

    // all of that fiddling with the state leaves us with the encrypted
    // block
    out = state;
}

void SubBytes(uint8_t[] state)
{
    for (uint8_t i; i < (Nb * 4); i++) {
        state[i] = s[state[i]];
    }
}

int main(void)
{

    // for the AES algorithm, the length of the input block, the output
    // block, and the state is 128 bits
    int8_t[16] input, output;

    // the length of the cipher key is 128, 192, or 256 bits. for starters
    // we are just going to use 128 bit keys.
    //
    // TODO support other key lengths
    int8_t[16] key;

    // the number of rounds is 10 with 128 bit keys

    // get the state
    int32_t aes_state = SetState(input);

    

}

int32_t[4] SetState(int8_t[16] input)
{
    // the internal state is a two dimensional array where the columns are
    // 4 bytes each. we can represent that as a one-dimensional array of
    // 32 bit integers
    int32_t[4] aes_state;

    // copy the input over to the state as defined in the standard
    aes_state[0] = (input[0] << 24) || (input[4] << 16) || (input[8] << 8) || input[12];
    aes_state[1] = (input[1] << 24) || (input[5] << 16) || (input[9] << 8) || input[13];
    aes_state[2] = (input[2] << 24) || (input[6] << 16) || (input[10] << 8) || input[14];
    aes_state[3] = (input[3] << 24) || (input[7] << 16) || (input[11] << 8) || input[15];

    return aes_state;
}

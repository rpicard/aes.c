// this looks more like pseudo code notes right now

// these are the functions we will use
void AddRoundKey();
void SubBytes();
void ShiftRows();
void MixColumns();

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

// this looks more like pseudo code notes right now

// these are the functions we will use
void AddRoundKey();
void SubBytes();
void ShiftRows();
void MixColumns();

// state is an array of uint32
// each uint32 is a column of 3 bytes
uint32[] state;


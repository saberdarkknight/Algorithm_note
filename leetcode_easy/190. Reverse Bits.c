uint32_t reverseBits(uint32_t n) {
    uint32_t output =0;

    for ( int i =0; i<32; i++){
        uint32_t bit = (n >> i) &1;
        output = ( bit << (31-i) ) | output;
    }
    
    return output;
}
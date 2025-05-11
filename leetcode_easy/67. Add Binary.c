char* addBinary(char* a, char* b) {

    int over = 0;
    int a_size = strlen(a);
    int b_size = strlen(b);
    int total_size = fmax(a_size, b_size) + 2; // add overflow digit and \0 position
    char* result = (char*) malloc (sizeof(char) * total_size);

    result[--total_size] = '\0';
    while ( (total_size >0) || (a_size > 0 ) || (b_size > 0) ){
        if ( a_size > 0 ){
            over = over  + a [ -- a_size] - '0';
        }
        if ( b_size > 0 ){
            over = over  + b [ -- b_size] - '0';
        }
        result [ --total_size ] = over % 2 + '0';
        over /=2;
    }
    if(result[0] == '0') return result+1; // to prevent from first digit is zero
    return result ;
}
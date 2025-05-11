int numDecodings(char* s) {

    // check early return condition
    if ( s[0] == '0' ){
        return 0;
    }

    // start the decoding
    int size = strlen(s);
    int * dp = (int *)malloc(sizeof(int) * (size+1) );
    dp[0] = 1;
    dp[1] = s[0] != '0' ? 1 : 0;  
    int digit;
    
    for (int i = 2; i <= size; i++){
        // initialize
        dp[i] = 0;
        // calculate the two number
        digit = (s[i-2] - '0') * 10 + (s[i -1] - '0');
        
        // if s[i-1]!= 0 : we can have at least dp[i-1] method
        // if digit is between 10 and 26 : we can also have at least dp[i-2 method]
        if ( s[i-1] != '0' ){
            dp[i] += dp[i - 1];
        }
        if (digit >= 10 && digit <= 26) {
            dp[i] += dp[i - 2];
        }
    }
    int result = dp[size];
    free (dp);

    return result;
}
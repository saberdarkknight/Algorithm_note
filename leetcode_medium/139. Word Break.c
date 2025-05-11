
// my attempt with greedy method

bool dict_check ( char * input, int start, int end, int* Dict_size, char** wordDict, int wordDictSize ){

    int len = end - start + 1;
    for (int i = 0; i < wordDictSize; i++) {
        if (Dict_size[i] == len && strncmp(wordDict[i], input + start, len) == 0) {
            return true;
        }
    }
    return false;
}

bool wordBreak(char* s, char** wordDict, int wordDictSize) {

    int Dict_size [wordDictSize];
    for ( int  i = 0; i < wordDictSize; i++){
        Dict_size [i] = strlen( wordDict[i] );
    }

    int size = strlen (s);
    int l = 0; 
    int r = 0;
    bool output = false;

    while(  r <= size ){
        output = dict_check (s, l, r-1, Dict_size, wordDict, wordDictSize);

        if ( output ){
            l=r;
        } else {
            r++;
        }
    }

    return l == size;
}


/*
In the greedy method, if `s="aaaaaaa"` and `Dict` is`[aaa, aaaa]`.
We may seperate into `aaa`, `aaa`, and `a` and return false;
It is necessary to use dynamic programming method
*/

bool dict_check ( char * input, int start, int end, int* Dict_size, char** wordDict, int wordDictSize ){

    int len = end - start + 1;
    for (int i = 0; i < wordDictSize; i++) {
        if (Dict_size[i] == len && strncmp(wordDict[i], input + start, len) == 0) {
            return true;
        }
    }
    return false;
}

bool wordBreak(char* s, char** wordDict, int wordDictSize) {

    int Dict_size [wordDictSize];
    for ( int  i = 0; i < wordDictSize; i++){
        Dict_size [i] = strlen( wordDict[i] );
    }

    int size = strlen (s);
    bool dp[size + 1];
    memset(dp, false, sizeof(dp));
    dp[0] = true; 

    for (int i = 1; i <= size; i++) {
        for (int j = 0; j < i; j++) {
            if (dp[j] && dict_check(s, j, i - 1, Dict_size, wordDict, wordDictSize)) {
                dp[i] = true;
                break;
            }
        }
    }

    return dp[size];
}

/*
Time complexity : O(4^N)
Space complexity : O(2N)

*/


// open stands for (   and  close stands for )
void backtrack(char** result, int* returnSize, char* path, int pos, int open, int close, int n){
    if ( open == n && close == n ){
        path[pos] = '\0';
        result[*returnSize] = strdup(path);
        (* returnSize) ++;
        return;
    }
    if (open < n) {
        path[pos] = '(';
        backtrack(result, returnSize, path, pos + 1, open + 1, close, n);
    }
    if (close < open) {
        path[pos] = ')';
        backtrack(result, returnSize, path, pos + 1, open, close + 1, n);
    }
}


char** generateParenthesis(int n, int* returnSize) {

    *returnSize = 0;
    int maxSize = 1 << (2 * n);  //
    char** output=(char**)malloc(sizeof(char*)*maxSize);
    char* path = (char*)malloc(sizeof(char)*(2*n+1));
    backtrack(output, returnSize, path, 0, 0, 0, n);
    free(path);
    return output;    
}

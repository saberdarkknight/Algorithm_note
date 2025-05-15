
/*
use stack
Time complexity : O(N)
Space complexity : O(N)
*/

char* removeOuterParentheses(char* s) {

    int len = strlen(s);
    int pos = -1;
    char stack[len];
    int resIndex = 0;   // index of output
    char* output = (char*)malloc(sizeof(char)*(len+1));


    // use stack to store the char
    for (int i = 0; i < len; i++) {
        if (s[i] == '(') {
            if (pos >= 0) {
                output[resIndex++] = '(';  // not the first left part
            }
            stack[++pos] = '(';
        } else if (s[i] == ')') {
            pos--;
            if (pos >= 0) {
                output[resIndex++] = ')';  // the first right part
            }
        }
    }

    output[resIndex] = '\0';
    return output;
}


/*
use stack
Time complexity : O(N)
Space complexity : O(1)
*/

char* removeOuterParentheses(char* s) {

    int len = strlen(s);
    int pos = -1;
    int resIndex = 0;
    char* output = (char*)malloc(sizeof(char)*(len+1));


    // use stack to store the char
    for (int i = 0; i < len; i++) {
        if (s[i] == '(') {
            if (pos >= 0) {
                output[resIndex++] = '(';  // not the first left part
            }
            pos ++;
        } else if (s[i] == ')') {
            pos--;
            if (pos >= 0) {
                output[resIndex++] = ')';  // the first right part
            }
        }
    }

    output[resIndex] = '\0';
    return output;
}
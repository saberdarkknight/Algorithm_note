/*
Time complexity : O(N)
Space complexity : O(1)


Edge case 
Input : ())* 
output should be false

*/


bool checkValidString(char* s) {
    
    int left = 0;
    int right = 0;
    int len = strlen(s);

    // check from left to right
    for (int i = 0; i < len; i++){

        if (s[i] == '(' || s[i] == '*') {
            left++;
        } else {
            left--;
        }
        if (left < 0) return false;
    }

    // check from right to left
    for (int i = len-1; i >= 0; i--){
        if (s[i] == ')' || s[i] == '*') {
            right++;
        } else {
            right--;
        }
        if (right < 0) return false;
    }

    return true;

}



// Timmy

int srchLeftParenthesis(char* stack, int top) {
    int idx = top - 1;

    while (idx >= 0) {
        if (stack[idx] == '(') {
            return idx;
        } else {
            idx -= 1;
        }
    }

    return -1;
}

void sortStack(char* stack, int srchIdx, int top) {
    for (int i = srchIdx; srchIdx < (top - 1); srchIdx++) {
        stack[i] = stack[i + 1];
    }
}

bool checkValidString(char* s) {
    int len = strlen(s);

    char stack[100];
    int top = 0;

    for (int idx = 0; idx < len; idx++) {
        if (s[idx] == '(' || s[idx] == '*') {
            stack[top] = s[idx];
            top += 1;
        } else if (s[idx] == ')') {
            int srchIdx = srchLeftParenthesis(stack, top);
            if (-1 != srchIdx) {
                sortStack(stack, srchIdx, top);
                top -= 1;
            } else {
                if (top) {
                    top -= 1; // have star sign to delete
                } else {
                    return false;
                }
            }
        }
    }

    // execute remain left parenthese and star sign
    if (top) {
        for (int idx = top - 1; idx >= 0; idx--) {
            if (stack[idx] == '*') {
                int srchIdx = srchLeftParenthesis(stack, top);
                if (-1 != srchIdx) {
                    sortStack(stack, srchIdx, top);
                    top -= 1;
                    idx -= 1;
                }
            } else {
                return false;
            }
        }
        return true;
    } else {
        return true;
    }
}

// len = strlen (s)

// handle 1 char condition

// ( ( ( ( ) ) * *
// 0             len
// l             r
// l = should be ( or *
// r = should be ) or *
// should be pair
// ( ( ) * ( *  ) ) ( ( * * => false
// ( ( ) ( ) )

// '(' stack: push * * ( ( * *
// '*' stack: push * *

// idea
// push stack if met '(' or '*'
// if we met ')', try to find the nearest '(' from the top, move it and sort the
// stack -> top-1 if we met ')' but we cannot find any '(', find '*' in the
// stack and pop it if we met ')' but top==0, return false else return true;

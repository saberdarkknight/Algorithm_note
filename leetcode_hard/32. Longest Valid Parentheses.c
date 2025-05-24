/*

Time complexity : O(N)
Space complexity : O(1)

Hint : use two-pass greedy


*/


int longestValidParentheses(char* s) {
    
    // use depth to check whether we need ( or )

    int left = 0;
    int right = 0;
    int size = 0;
    int maxLen = 0;

    // check from left to right
    for ( int i = 0; s[i]!='\0'; i++){
        size++;
        if (s[i] == '('){
            left++;
        } else {
            right++;
        }
        if (left == right){
            maxLen = (right + left > maxLen) ? (right + left) : maxLen;
        } else if (right > left){
            left = 0;
            right = 0;
        }
    }


    // check from right to left
    left = 0;
    right = 0;
    for (int i = size - 1; i >= 0; i--) {
        if (s[i] == '('){
             left++;
        } else{
             right++;
        }

        if (left == right){
            maxLen = (right + left > maxLen) ? (right + left) : maxLen;
        }else if (left > right) {
            left = right = 0;
        }
    }

    return maxLen;

}
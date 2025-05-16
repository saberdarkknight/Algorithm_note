/*
Time complexity : O(N)
Space complexity : O(1)

*/

int scoreOfParentheses(char* s) {

    int score = 0;
    int depth = 0;
    int left_size = 0;

    // if we meet )(  => use add
    // if we meet (()) => use multiple
    for (int i = 0; s[i]!='\0'; i++){
        if ( s[i] == '(' ){
            depth ++;
        } else {
            depth --;
            if (s[i - 1] == '(') {
                score += 1 << depth;  // add the score with 2^(depth)
            }
        }
    }
    
    return score;
}
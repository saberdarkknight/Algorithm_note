/*

Time complexity : O(N)
Space complexity : O(N)
*/


bool canBeValid(char* s, char* locked) {

    int n = strlen(s);

    // if n is odd, it will never be valid
    if ( n%2 == 1 ){
        return false;
    }

    // check from left to right
    int left = 0;
    for (int i = 0; i < n; i++) {
        if (locked[i] == '1') {
            if (s[i] == '('){
                left++;
            } else {
                left --;
            } 
        } else {
            left++;  // can pretend unlocked to be '(' if needed
        }
        if (left < 0) return false;  // too many ')'
    }

    // check from right to left
    left = 0;  // max close parentheses count
    for (int i = n - 1; i >= 0; i--) {
        if (locked[i] == '1') {
            if (s[i] == ')'){
                left ++;
            } else {
                left --;
            } 
        } else {
            left ++;  // can pretend unlocked to be ')'
        }
        if (left < 0) return false;  // too many '('
    }
    
    

    return true;
}
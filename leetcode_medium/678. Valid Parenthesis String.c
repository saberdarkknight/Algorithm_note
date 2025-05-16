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
/*

Time complexity : O(N)
Space complexity : O(1)
*/

int minInsertions(char* s) {
    int insert = 0; // Total insertions needed, this can be ( or )
    int right = 0; // Number of ')' we need
    for ( int i = 0; s[i]!= '\0'; i ++){
        if ( s[i] =='('){
            right += 2;
            if (right % 2 == 1) {
                // Make even by inserting 1 ')'
                insert++;
                right--;
            }
        } else {
            right--;
            if (right < 0) {
                insert++; // Need to insert one '('
                right = 1;  // Because one ')' already used, we still need one more
            }
        }
    }
    return insert + right ;
}

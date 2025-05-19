/*

Time complexity : O(N)
Space complexity : O(1)
*/

int minInsertions(char* s) {

    int depth = 0; // Total insertions needed
    int right = 0; // Number of ')' we need

    for ( int i = 0; s[i]!= '\0'; i ++){
        if ( s[i] =='('){
            right += 2;
            if (right % 2 == 1) {
                // Make even by inserting 1 ')'
                depth++;
                right--;
            }
        } else {
            right--;
            if (right < 0) {
                // Need to insert one '('
                depth++;
                right = 1;  // Because one ')' already used, we still need one more
            }

        }
        
    }

    return depth + right ;
    
}

/*
Time complexity : O(N)
Space complexity : O(1)
*/

int maxDepth(char* s) {

    int num = 0;
    int max_num = 0;
    for ( int i = 0; s[i]!= '\0'; i++ ){
        if ( s[i] == '(' ){
            num ++;
        }

        if ( s[i] == ')' ){
            num --;
        }

        if ( num > max_num ){
            max_num = num;
        }
    }
    
    return max_num;
}
/*
Time complexity : O(N)
Space complexity : O(1)

Edge case
()))(( : output should be 4 
*/
int minAddToMakeValid(char* s) {
    
    int depth = 0;
    int sum = 0;
    for (int i = 0; s[i] != '\0'; i ++){
        if ( s[i] == '(' ){
            depth ++;
        } else{
            depth --;
        }
        if (depth < 0){
            sum = sum - depth;
            depth = 0;
        }
    }
    return depth + sum;
}


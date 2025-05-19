/*

Time complexity : O(N)
Space complexity : O(N)
*/


void swap(char* s, int left, int right) {
    while (left < right) {
        char tmp = s[left];
        s[left] = s[right];
        s[right] = tmp;
        left++;
        right--;
    }
}

char* reverseParentheses(char* s) {
    
    // start process of the s
    int size = strlen(s);
    int left_stack [size];
    int left_size = 0; 
    int left_pos = 0;
    int right_pos = 0;

    // add stack and index
    for ( int i = 0; s[i] != '\0'; i ++){
        if ( s[i] == '(' ){
            left_stack [left_size++] = i;
        } else if ( s[i] == ')' ){
            left_pos = left_stack[--left_size];
            right_pos = i;
            swap(s, left_pos + 1, right_pos - 1);
        }
    }

    // make new string
    char output[size+1];
    int cur_index = 0;
    for (int i = 0; i < size; i ++){
        if ( (s[i]!='(') && (s[i]!=')') ){
            output[cur_index++] = s[i];
        }
    }
    output[cur_index] = '\0';
    return output;
}
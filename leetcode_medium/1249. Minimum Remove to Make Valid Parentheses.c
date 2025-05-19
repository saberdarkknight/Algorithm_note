/*

Time complexity : O(N)
Space complexity : O(N)
*/


char* minRemoveToMakeValid(char* s) {

    int size = strlen(s);
    int depth = 0;
    char* temp = (char*)malloc(sizeof(char)*(size+1));
    int index = 0;

    for (int i = 0; i < size; i ++){
        if ( s[i] == '(' ){
            depth ++;
        }
        if ( s[i] == ')' ){
            depth --;
        }

        if ( depth >= 0 ){
            temp[index++] = s[i];
        } else {
            depth = 0;
        }

    }


    // start from right to left
    char* result = (char*)malloc(sizeof(char)*(index + 1));
    int right = 0;
    int res_idx = index;
    result[res_idx] = '\0';
    for (int j = index - 1; j >= 0; j--) {
        if (temp[j] == ')') {
            right++;
            result[--res_idx] = temp[j];
        } else if (temp[j] == '(') {
            if (right > 0) {
                right--;
                result[--res_idx] = temp[j];
            }
            // else skip unmatched '('
        } else {
            result[--res_idx] = temp[j];
        }
    }

    free (temp);
    return result+res_idx;
}
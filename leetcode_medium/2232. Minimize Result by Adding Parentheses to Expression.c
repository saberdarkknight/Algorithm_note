/*

Time complexity : O(N)
Space complexity : O(N)
*/

char* minimizeResult(char* expression) {

    // find the posistion of +
    int mid = 0;
    int size = 0;
    for (int i = 0; expression[i]!= '\0'; i++){
        if ( expression[i]=='+' ){
            mid = i;
        }
        size++;
    }
    

    // use brute force method to find the min value
    // the expression can be separated into prefix * (mid_left + mid_right) * suffix
    char left[mid + 1];
    char right[size - mid];
    strncpy(left, expression, mid);
    left[mid ] = '\0';
    strcpy(right, expression + mid + 1);
    int minValue = INT_MAX;
    int bestL = 0, bestR = 0;


for (int i = 0; i < mid; i++) {
        for (int j = 1; j <= strlen(right); j++) {
            // construct 3 parts
            // prefix (left[0..i-1])、infix (left[i..] + right[0..j-1])、suffix (right[j..])
            char prefix[20] = {0}, midLeft[20] = {0}, midRight[20] = {0}, suffix[20] = {0};
            
            strncpy(prefix, left, i);
            prefix[i] = '\0';
            
            strcpy(midLeft, left + i);
            midLeft[mid - i] = '\0';

            strncpy(midRight, right, j);
            midRight[j] = '\0';

            strcpy(suffix, right + j);

            int pre = strlen(prefix) > 0 ? atoi(prefix) : 1;
            int suf = strlen(suffix) > 0 ? atoi(suffix) : 1;
            int mid = atoi(midLeft) + atoi(midRight);
            int total = pre * mid * suf;

            if (total < minValue) {
                minValue = total;
                bestL = i;
                bestR = j;
            }
        }
    }

    // start construct the result
    char* result = (char*)malloc(sizeof(char) * (size + 3)); // string size  + '(' + ')' + '\0'
    int idx = 0;

    for (int i = 0; i < bestL; i++){
        result[idx++] = expression[i];
    } 
    result[idx++] = '(';
    for (int i = bestL; i <= mid; i++){
        result[idx++] = expression[i];
    } 
    for (int i = mid + 1; i < mid + 1 + bestR; i++){
        result[idx++] = expression[i];
    } 

    result[idx++] = ')';
    for (int i = mid + 1 + bestR; i < size; i++){
        result[idx++] = expression[i];
    } 
    result[idx] = '\0';

    return result;
}


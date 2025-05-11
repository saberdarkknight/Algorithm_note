char* longestPalindrome(char* s) {
    int size = strlen(s); // size of string
    if (size == 0){
         return "";
    }

    if (size == 1){
        return s;
    }

    int r = size -1;      // current left
    int l = 0;            // starting point
    bool check_result = false;
    int l_final;
    int r_final;
    int r_temp = r; 
    int max_size = 0;
    int cur_size = 0;

    while (r > l) {
        r_temp = r;       // reset temp pointer for every l
        while (r_temp >= l) {
            if (s[l] == s[r_temp]) {
                // check palindrome between l and r_temp
                check_result = true;
                for (int j = 0; j <= (r_temp - l) / 2; j++) {
                    if (s[l + j] != s[r_temp - j]) {
                        check_result = false;
                        break;
                    }
                }

                // if it's a palindrome and longer than current max
                if (check_result) {
                    cur_size = r_temp - l + 1;
                    if (cur_size > max_size) {
                        max_size = cur_size;
                        l_final = l;
                        r_final = r_temp;
                    }
                    break;  // get the max value
                }
            }
            r_temp--; // start from r_temp -1
        }

        l++;  // get next value of l
    }

    char *output = (char*) malloc(max_size + 1);
    for (int k=0; k<=max_size;k++){
        output[k] = s[l_final+k];
    }
    output[max_size] = '\0';
    return output;
}
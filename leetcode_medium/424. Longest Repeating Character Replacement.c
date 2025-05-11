#define alphabet_num 26

int characterReplacement(char* s, int k) {
    
    // convert char array to int array
    int size = strlen(s);

    // start the iteration from the left 
    int output = 0;
    int maxCount = 0;
    int left = 0;
    int * freq = (int *) malloc(sizeof(int)*alphabet_num);
    memset(freq, 0, sizeof(int)*alphabet_num);


    for ( int right = 0; right < size; right ++){
        freq[s[right] - 'A']++;

        // check if the current char 
        if ( freq[s[right] - 'A'] > maxCount){
            maxCount = freq[s[right] - 'A'];
        }

        // check if we need to shift left
        while (  (right - left + 1) - maxCount > k ) {
            freq[s[left] - 'A']--;
            left++;
        }

        // update output
        if ((right - left + 1) > output) {
            output = right - left + 1;
        }
    }
    
    free(freq);
    return output;
}
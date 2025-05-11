
// Need to take care of the repeat position.
// The repeat position will not always in the first position. 


int  check_repeat ( char check, char * result, int cur_size ){
    for (int i = 0; i< cur_size; i++){
        if ( check == result[i] ) {
            return i;
        }
    }
    return -1;
}

int lengthOfLongestSubstring(char* s) {

    int size = strlen(s);
    if ( size == 0){
        return 0;
    }

    char *result = (char *) malloc(sizeof(char)*size);
    int output_size = 0;
    int max_size =0;
    int check;

    for (int i=0; i< size; i++){
        check =  check_repeat (s[i], result, output_size);

        if ( check == -1 ){
            result [output_size++] = s[i];  
        } else {
            for (int j = check + 1; j < output_size; j++) {
                result[j - check - 1] = result[j];
            }
            output_size = output_size - check - 1;
            result[output_size++] = s[i]; 
        }
        if ( output_size > max_size ){
            max_size = output_size;
        }
    }
    return max_size;
}
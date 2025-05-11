int myAtoi(char* s) {
    
    int size = strlen(s);
    int  result = 0; 
    bool negative = false; // check if the number is negative
    bool started = false; // indicate whether we start the reading

    for ( int i=0; i< size; i++){
        // skip the check if we do not start and it is enmpty
        if ( (s[i] == ' ') && (!started) ){
            continue;
        } 
        // check sign if we do not start reading
        if ( (s[i]== '-') && (!started) ){
            negative = true;
            started = true;
            continue;
        } 
        if ( (s[i]== '+') && (!started) ){
            negative = false;
            started = true;
            continue;
        } 
        // start reading
        if ( ( s[i] >= '0') && ( s[i] <= '9') )  {
            started = true;
            int digit = s[i] - '0';
            if (result > (INT_MAX - digit ) / 10 ){
                return negative ? INT_MIN : INT_MAX;
            }
            result = result * 10 + digit;
        } else {
            break;
        }
    }

return negative ? -result : result;
}
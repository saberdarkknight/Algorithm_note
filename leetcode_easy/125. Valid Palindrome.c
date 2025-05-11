int isAlphanum(char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); 
}

int convertLower( char c){
    return ( (c >= 'A') && ( c <= 'Z' ) )? c+32 : c;
}

bool isPalindrome(char* s) {
    int r = strlen(s) -1;
    int l = 0;
    while ( r > l ){
        if ( isAlphanum (s[r]) ){
            if (  isAlphanum (s[l]) ){
                if ( convertLower (s[r]) != convertLower (s[l])  ){
                    return false;
                }
                l++;
                r--;
            }else{
                l++;
            }
        } else {
            r--;
        }
    }
    return true;
}
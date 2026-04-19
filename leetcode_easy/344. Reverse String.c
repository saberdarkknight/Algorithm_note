
// My attempt

void reverseString(char* s, int sSize) {

    int left = 0; 
    int right = sSize-1;

    while( right > left ){
        char temp = s[left];
        s[left] = s[right];
        s[right] = temp;
        left++;
        right--;
    }
    
}

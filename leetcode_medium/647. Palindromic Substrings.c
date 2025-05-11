
// My attempt of using recursive method

bool isPalindrome(char* s, int left, int right){
    while (left < right){
        if (s[left] != s[right]){
            return false;
        }
        left++;
        right--;
    }
    return true;
}

int countPalindrome(char* s, int start, int size){
    if (start == size){
        return 0;
    }

    int count = 0;
    for (int end = start; end < size; end++){
        if (isPalindrome(s, start, end)){
            count++;
        }
    }

    // start the recursion for the next step
    return count + countPalindrome(s, start + 1, size);
}


int countSubstrings(char* s) {
    
    int size = strlen(s);
    return countPalindrome(s, 0, size);
    
}



// Better answer

int countSubstrings(char * s)
{
    int count = 0;
    int left = 0;
    int right = 0;
    int len = strlen(s);
    for (int i = 0; i < len; i++) {
        left = i;
        right = i;
        while (left >= 0 && right < len && s[left] == s[right]) {
            count++;
            left--;
            right++;
        }
        left = i;
        right = i + 1;
        while (left >= 0 && right < len && s[left] == s[right]) {
            count++;
            left--;
            right++;
        }
    }
    return count;
}

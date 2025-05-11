
// my attemp

bool isPalindrome(int x) {

    char s[64];
    sprintf(s,"%d", x);
    int s_sizr = strlen(s);
    bool check;


    for (int i =0; i <= (int)(s_sizr/2) ; i++ ){

        if ( s[i] == s[s_sizr -1 - i] ){
            check = true;
        } else{
            check = false;
            break;
        }
    }

    return check;
}


// Advanced
// https://leetcode.com/problems/palindrome-number/solutions/3283534/accepted-java-c-code-beats-98/

bool isPalindrome(int x){
    if(x<0 || x!=0 && x%10 ==0 ) return false;
    int check=0;
    while(x>check){
        check = check*10 + x%10;
        x/=10;
    }
    return (x==check || x==check/10);
}
/*

use stack
Time complexity : O(N)
Space complexity : O(N)


*/


bool isValid(char* s) {

int len = strlen(s);
int pos = -1;
char stack[len];

for ( int i =0; i < len; i++ ){    
    if (  (s[i] == '(') || ( s[i] == '[' ) || ( s[i] == '{' ) ){
        stack [++pos] = s[i];
    } else {
        if ( (pos == -1) ||  ( (s[i]==')') && (stack[pos] != '(') ) ||  ( (s[i]=='}')&& (stack[pos] != '{') ) ||  ( (s[i]==']') && (stack[pos] != '[') )  ){
            return false;
        }
        pos -= 1;
    }

}
return (pos == -1);

}



/*
without using stack
Time complexity : O(N)
Space complexity : O(1)

*/

bool checkMatch(char c1, char c2){
    if (c1 == '(' && c2 == ')') return true;
    if (c1 == '[' && c2 == ']') return true;
    if (c1 == '{' && c2 == '}') return true;
    return false;
}


bool isValid(char* s) {
    

int len = strlen(s);
int pos = -1;


for ( int i =0; i < len; i++ ){
    
    if (  (s[i] == '(') || ( s[i] == '[' ) || ( s[i] == '{' ) ){
        s[++pos] = s[i];
    } else {
        if ( (pos == -1) || !checkMatch(s[pos], s[i]) ){
            return false;
        } else{
            pos --;  
        }
    }

}

return (pos == -1);

}
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
int partitionString(char* s) {

    int size = strlen (s);
    int alphabet [26]={0};   // we only has 26 characters, the type can be char
    int number = 1;

    for ( int i = 0; i< size ; i++ ){       
        alphabet [ s[i] - 'a' ] ++;
        if (  alphabet [ s[i] - 'a' ] == 2  ){
            number ++;
            memset(alphabet, 0, sizeof(int) * 26);
            i--;
        }
    }

    return number;
}
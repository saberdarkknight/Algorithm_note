
// My attempt

int compare ( const void *a, const void *b ){
    return  (*(char *) a - *(char *) b );
}

bool isAnagram(char* s, char* t) {

    int r_size = strlen(s);
    int t_size = strlen(t);
    if ( r_size != t_size ){
        return false;
    }

    qsort(s, r_size, sizeof(char), compare);
    qsort(t, t_size, sizeof(char), compare);

    for ( int i =0; i< r_size; i++ ){
        if ( s[i] != t[i] ){
            return false;
        }
    }
    return true;
}


// Another approach

#define alphabet_num 26

bool isAnagram(char* s, char* t) {

    int r_size = strlen(s);
    int t_size = strlen(t);
    if ( r_size != t_size ){
        return false;
    }

    int r_number [alphabet_num] ={0};
    int t_number [alphabet_num] ={0};

    for ( int i =0; i< r_size; i++ ){
        r_number [ s[i] - 'a' ] ++;
        t_number [ t[i] - 'a' ] ++;
    }

    for ( int i =0; i< alphabet_num; i++ ){
        if ( r_number[i] != t_number[i] ){
            return false;
        }
    }
    return true;
}



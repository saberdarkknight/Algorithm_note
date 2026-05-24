/*

Time complexity : O(n · m) where n = len(s), m = len(p).
Space complexity : O(1)

This should use back track inside to check the string

*/

bool isMatch(char* s, char* p) {

    char* star_p = NULL;  // last '*' position in p
    char* match_s = NULL; // s position when '*' was found


    while (*s) {
        if (*p == '?' || *p == *s) {
            // characters match, advance both
            s++; p++;
        } else if (*p == '*') {
            // record the '*' and current s position
            star_p = p;
            match_s = s;
            p++; // try matching '*' with empty string first
        } else if (star_p) {
            // backtrack => important
            // mismatch, but we have a previous '*' to fall back to
            // let '*' consume one more character from s
            p = star_p + 1;
            match_s++;
            s = match_s;
        } else {
            return false; // no '*' to backtrack to
        }
    }

    // consume remaining '*' in p (they can match empty string)
    while (*p == '*') p++;

    return *p == '\0';

}



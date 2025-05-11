#define alphabet_num 26

bool vector_compare ( int *a, int * b, int size){
    for (int i = 0; i < size; i++){
        if ( a[i] != b[i] ){
            return false;
        }
    }
    return true;
}

char*** groupAnagrams(char** strs, int strsSize, int* returnSize, int** returnColumnSizes) {
    
    int groupCount = 0;
    int maxGroups = strsSize;
    char*** output = malloc(sizeof(char**) * maxGroups);
    *returnColumnSizes = malloc(sizeof(int) * maxGroups);
    int** groupKeys = malloc(sizeof(int*) * maxGroups); 
    int size = 0;
    int found = -1;

    // create an int array
    for ( int i = 0; i < strsSize; i++ ){

        // create an int array for the current string
        size = strlen( strs[i] );
        int* currentKey = calloc(alphabet_num, sizeof(int));
        for (int j = 0; j < size; j++) {
            currentKey[strs[i][j] - 'a']++;
        }

        // check if current str is in groupKeys
        found = -1;
        for (int g = 0; g < groupCount; g++) {
            // check whether currentkey is in groupKeys
            if ( vector_compare(currentKey, groupKeys[g], alphabet_num) ){
                found = g;
                break;
            }
        }

        if ( found == -1 ){
            // add a new group
            groupKeys [ groupCount ] = currentKey;
            output[groupCount] = malloc(sizeof(char*) * strsSize);
            output[groupCount][0] = strs[i];
            (*returnColumnSizes)[groupCount] = 1;
            groupCount++;;
        } else {
            // add to existing group
            output[found][(*returnColumnSizes)[found]++] = strs[i];
            free(currentKey);
        }
    }

    *returnSize = groupCount;
    return output;
}
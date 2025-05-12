char* map[10] = {
    "",     // 0
    "",     // 1
    "abc",  // 2
    "def",  // 3
    "ghi",  // 4
    "jkl",  // 5
    "mno",  // 6
    "pqrs", // 7
    "tuv",  // 8
    "wxyz"  // 9
};

void backtrack(char* digits, int index, char* path, int pathLen, char** result, int* returnSize) {
    if (digits[index] == '\0') {
        path[pathLen] = '\0';
        result[*returnSize] = strdup(path); // copy current combination
        (*returnSize)++;
        return;
    }

    char* letters = map[digits[index] - '0'];
    for (int i = 0; letters[i] != '\0'; i++) {
        path[pathLen] = letters[i];
        backtrack(digits, index + 1, path, pathLen + 1, result, returnSize);
    }
}


char** letterCombinations(char* digits, int* returnSize) {
    *returnSize = 0;

    if (digits == NULL || digits[0] == '\0') {
        return NULL;
    }

    // create a new array that keep only digit
    int len = strlen(digits);
    char* filtered = (char*)malloc(len + 1);
    int filteredLen = 0;

    for (int i = 0; i < len; i++) {
        if (digits[i] >= '2' && digits[i] <= '9') {
            filtered[filteredLen++] = digits[i];
        }
    }
    filtered[filteredLen] = '\0';

    // if the filtered size is zero => return NULL
    if (filteredLen == 0) {
        free(filtered);
        return NULL;
    }

    // calculate the maximum combination
    int maxCombinations = 1;
    for (int i = 0; i < filteredLen; i++) {
        if (filtered[i] == '7' || filtered[i] == '9')
            maxCombinations *= 4;
        else
            maxCombinations *= 3;
    }

    char** result = (char**)malloc(sizeof(char*) * maxCombinations);
    char* path = (char*)malloc(sizeof(char) * (filteredLen + 1));

    // use backtrack to find the combination
    backtrack(filtered, 0, path, 0, result, returnSize);


    free(path);
    free(filtered);

    return result;
}
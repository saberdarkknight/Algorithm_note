

/*
My first attempt : fill the matrix first and then read the string 

Space Complexity : O(NM)
Time Complexity: O(N)

*/


char* convert(char* s, int numRows) {
    if (numRows == 1) return s;

    int len = strlen(s);
    int numCols = len;  // worst case columns

    // allocate numRows x numCols matrix (flattened)
    char *matrix = calloc(numRows * numCols, 1);

    int row = 0, col = 0, dir = -1;
    int i = 0;

    while (i < len) {
        matrix[row * len + col] = s[i];

        if (row == 0 || row == numRows - 1)
            dir = -dir;

        if (dir == -1)        // going up: each step is a new col
            col++;
        else if (row == 0)    // just bounced back to top: new downstroke needs new col
            col++;

        row += dir;
        i++;
    }

    // read matrix row by row, skipping empty cells
    char *result = calloc(len + 1, 1);
    int k = 0;
    for (int r = 0; r < numRows; r++)
        for (int c = 0; c < numCols; c++)
            if (matrix[r * numCols + c] != '\0')
                result[k++] = matrix[r * numCols + c];

    free(matrix);
    return result;
}




/*


Second attempt : use 2-D char array with each char filled in each row sequencilly and merge together

Space Complexity : O(N)
Time Complexity: O(N)

*/


char* convert(char* s, int numRows) {
    if (numRows == 1) return s;

    int len = strlen(s);

    char **rows = calloc(numRows, sizeof(char *));
    for (int i = 0; i < numRows; i++)
        rows[i] = calloc(len + 1, 1);

    int row = 0, dir = -1;
    int i = 0;

    while (i < len) {
        // append s[i] to the current row's buffer
        int rowLen = strlen(rows[row]);
        rows[row][rowLen] = s[i];

        if (row == 0 || row == numRows - 1)
            dir = -dir;

        row += dir;
        i++;
    }

    // concatenate all rows
    char *result = calloc(len + 1, 1);
    for (int i = 0; i < numRows; i++)
        strcat(result, rows[i]);

    for (int i = 0; i < numRows; i++) free(rows[i]);
    free(rows);

    return result;
}




/*

Third attempt with AI assistant 

Thought : use the math to calcualte the number ordering and filled directed to a char array

Space Complexity : O(1)
Time Complexity: O(N)

*/



char* convert(char* s, int numRows) {
    if (numRows == 1) return s;

    int len = strlen(s);
    int period = 2 * (numRows - 1);
    char *result = calloc(len + 1, 1);
    int k = 0;

    for (int row = 0; row < numRows; row++) {
        int i = row;  // first character of this row
        while (i < len) {
            result[k++] = s[i];         // downstroke character

            // diagonal character exists only for middle rows
            int diag = i + period - 2 * row;
            if (row > 0 && row < numRows - 1 && diag < len)
                result[k++] = s[diag];

            i += period;  // next cycle
        }
    }

    return result;
}







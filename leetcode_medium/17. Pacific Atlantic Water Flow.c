void dfs( int** heights, int row, int col, bool ** ocean, int rows, int cols ){
    ocean[row][col] = true;

    int dirs[4][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    for (int d = 0; d < 4; d++) {
        int newRow = row + dirs[d][0];
        int newCol = col + dirs[d][1];

        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
            if (!ocean[newRow][newCol] && heights[newRow][newCol] >= heights[row][col]) {
                dfs(heights, newRow, newCol, ocean, rows, cols);
            }
        }
    }
}


int** pacificAtlantic(int** heights, int heightsSize, int* heightsColSize, int* returnSize, int** returnColumnSizes) {

    // if heights is NULL => return NULL
    if (heightsSize == 0 || *heightsColSize == 0) {
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }


    // more every index check if the index can be flowed to 
    int rows = heightsSize;
    int cols = *heightsColSize;
    bool **pacific = (bool **)calloc(rows, sizeof(bool*));
    bool **atlantic = (bool **)calloc(rows, sizeof(bool*));
    for ( int i = 0; i < rows; i++ ){
        pacific[i] = (bool *)calloc(cols, sizeof(bool));
        atlantic[i] = (bool *)calloc(cols, sizeof(bool));
    }

    // DFS from pacific (top and left)
    for (int i = 0; i < rows; i++) {
        dfs(heights, i, 0, pacific, rows, cols);        // Left
        dfs(heights, i, cols - 1, atlantic, rows, cols); // Right
    }
    for (int j = 0; j < cols; j++) {
        dfs(heights, 0, j, pacific, rows, cols);         // Top
        dfs(heights, rows - 1, j, atlantic, rows, cols); // Bottom
    }

    // map the result
    int** result = malloc(rows * cols * sizeof(int*));
    *returnColumnSizes = malloc(rows * cols * sizeof(int));
    int count = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (pacific[i][j] && atlantic[i][j]) {
                result[count] = (int*)malloc(2 * sizeof(int));
                result[count][0] = i;
                result[count][1] = j;
                (*returnColumnSizes)[count] = 2;
                count++;
            }
        }
    }

    *returnSize = count;
    return result;
}
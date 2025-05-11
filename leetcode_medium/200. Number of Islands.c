void dfs( char** grid, int col, int row, int start_row, int start_col ){
    if (  start_row < 0 ||  start_row >= row  || start_col < 0  || start_col>= col  || grid[start_row][start_col] == '0' ){
        return;
    }

    // set visited node as 0
    grid[start_row][start_col] = '0';    

    // go to next node
    dfs( grid, col, row, start_row + 1, start_col );
    dfs( grid, col, row, start_row - 1, start_col );
    dfs( grid, col, row, start_row, start_col + 1 );
    dfs( grid, col, row, start_row, start_col - 1 );

}

int numIslands(char** grid, int gridSize, int* gridColSize) {

    // return 0 if grid is null
    if (grid == NULL || gridSize == 0 || *gridColSize == 0) {
        return 0;
    }

    int row_size = gridSize;
    int col_size = * gridColSize;
    int num = 0;

    // find 1 as starting point
    for ( int i = 0; i < row_size; i++ ){
        for  ( int j = 0; j < col_size; j++ ){
            if ( grid[i][j] == '1' ){
                num ++;
                dfs( grid, col_size, row_size, i, j  );
            }
        }
    }
    
    return num;
}
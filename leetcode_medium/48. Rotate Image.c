void rotate(int** matrix, int matrixSize, int* matrixColSize) {
    // use matrix transpose and then swap the column
    int row_size = matrixSize;
    int col_size = * matrixColSize;
    int temp;
    // start the matrix transpose
    for ( int i = 0; i < row_size; i++ ){
        for ( int j = i+1; j < col_size; j++ ){
            temp = matrix [i][j];
            matrix [i][j] = matrix [j] [i];
            matrix [j] [i] =  temp;
        }
    }

    // swap the column
    int col_left = 0;
    int col_right = col_size -1;
    while( col_right > col_left ){
        for ( int i = 0; i < row_size; i++ ){
            temp = matrix [i][col_left];
            matrix [i][col_left] = matrix [i] [col_right];
            matrix [i] [col_right] =  temp;
        }
        col_left ++;
        col_right--;
    }
    
}
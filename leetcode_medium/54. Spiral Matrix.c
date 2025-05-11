int* spiralOrder(int** matrix, int matrixSize, int* matrixColSize, int* returnSize) {


    // get the matrix size
    int row_num = matrixSize;
    int col_num = * matrixColSize;
    * returnSize = row_num * col_num;
    int* output = (int *)calloc(row_num * col_num, sizeof(int) );
    
    int top = 0;
    int bottom = row_num - 1;
    int left = 0;
    int right = col_num - 1;
    int index = 0;

    while ( bottom >= top && right >= left ){

        // upper left to right
        for ( int j = left; j <= right; j++ ){
            output[index++] = matrix[top][j];
        }
        top ++;

        // right top to bottom
        for ( int i = top; i <= bottom; i++){
            output[index++] = matrix[i][right];
        }
        right--;

        // bottom right to left
        if (top <= bottom) {
            for (int j = right; j >= left; j--){
                output[index++] = matrix[bottom][j];
            }
            bottom--;
        }

        // left bottom to top
        if (left <= right) {
            for (int i = bottom; i >= top; i--){
                output[index++] = matrix[i][left];
            }
            left++;
        }

    }

    return output;
}
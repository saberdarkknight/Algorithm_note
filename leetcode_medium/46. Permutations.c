void backtrack(int* nums, int numsSize, int* path, int pathSize, bool* used, int** output, int* outputIndex) {
    if (pathSize == numsSize) {
        for (int i = 0; i < numsSize; i++) {
            output[*outputIndex][i] = path[i];
        }
        (*outputIndex)++;
        return;
    }

    for (int i = 0; i < numsSize; i++) {
        if (!used[i]) {
            used[i] = true;
            path[pathSize] = nums[i];
            backtrack(nums, numsSize, path, pathSize + 1, used, output, outputIndex);
            used[i] = false;
        }
    }
}

int** permute(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {

    // use factorial to calculate the return size
    int size = 1;
    for ( int i = 1; i <= numsSize; i++){
        size = size * i;
    }
    * returnSize = size;
    
    // use backtrack to get the output
    int row_size = size;
    int col_size = numsSize;
    int ** output = (int **)malloc( sizeof(int*) * row_size );
    *returnColumnSizes = (int*)malloc(sizeof(int) * row_size);
    for (int i = 0; i < row_size; i++) {
        output[i] = (int*)malloc(sizeof(int) * col_size);
        (* returnColumnSizes)[i] = col_size;
    }
    
    int* path = (int*)malloc(sizeof(int) * numsSize);
    bool* used = (bool*)calloc(numsSize, sizeof(bool));
    int outputIndex = 0;

    backtrack(nums, numsSize, path, 0, used, output, &outputIndex);

    free(path);
    free(used);


    return output;
}
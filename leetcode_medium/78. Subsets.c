int size_count( int step){
    int size = 0;
    while(step > 0){
        if ( step & 1 ){
            size ++;
        }
        step = step >> 1; 
    }
    return size;
}

int** subsets(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {
    
    int total_size = 1 << numsSize;  // total size = 2^numsSize
    * returnSize = total_size;
    int** output = (int**)malloc(sizeof(int*)*total_size);
    * returnColumnSizes =  (int*)malloc(sizeof(int)*total_size);

    for ( int i = 0; i < total_size; i ++ ){
        int cur_size = size_count(i);
        (* returnColumnSizes) [i] = cur_size;
        output[i] = (int *)malloc(sizeof(int)*cur_size);
        int pos = 0;
        for (int j = 0; j < numsSize; j++){
            if (i & (1 << j)) {
                output[i][pos++] = nums[j];
            }
        }
    }
    return output;
}
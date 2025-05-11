bool canJump(int* nums, int numsSize) {

    // check some current condition
    if ( numsSize <= 1){
        return true;
    }

    // use DP to record the maximum approachable location
    bool *check = calloc(numsSize, sizeof(bool));
    check[0] = true;
    for (int i = 0; i < numsSize-1; i ++){
        if (!check[i]) {
            continue;  
        }
        for ( int j = 0; j <= nums[i]; j++ ){
            if ( i + j < numsSize )
            check[ i + j ] = true;
        }
    }
    bool result = check[numsSize - 1];
    free(check);
    return result;
}
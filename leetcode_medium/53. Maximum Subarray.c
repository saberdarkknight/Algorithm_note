int maxSubArray(int* nums, int numsSize) {
    int current_sum = 0;
    int current_max = nums[0];

    for ( int i = 0; i < numsSize; i++  ){
        current_sum = current_sum + nums[i];

        if ( current_sum > current_max ){
            current_max = current_sum;
        }

        if ( current_sum < 0 ){
            current_sum = 0;
        }
    }
    return current_max;
}
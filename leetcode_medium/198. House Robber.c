int rob(int* nums, int numsSize) {
    
    if ( numsSize == 0 ){
        return 0;
    }
    if ( numsSize == 1 ){
        return  nums[0] ;
    }

    int prev2 = nums[0];
    int prev1 = fmax(nums[0], nums[1]);

    for ( int i =2; i < numsSize; i++ ){
        int cur = fmax( prev2 + nums[i],  prev1 );

        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
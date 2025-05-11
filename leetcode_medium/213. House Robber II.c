int check(int* nums, int start, int end) {
    if (end - start + 1 == 0) return 0;
    if (end - start + 1 == 1) return nums[start];

    int prev2 = nums[start];
    int prev1 = fmax(nums[start], nums[start+1]);

    for ( int i = start+2; i <= end; i++ ){
        int cur = fmax( prev2 + nums[i],  prev1 );

        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

int rob(int* nums, int numsSize) {
    
    // return the valus if numsSize is less than 1 
    if ( numsSize == 0 ){
        return 0;
    }
    if ( numsSize == 1 ){
        return  nums[0] ;
    }

    // check the 
    int case1 = check (nums, 0, numsSize-2);
    int case2 = check (nums, 1, numsSize-1);

    return case1 > case2? case1:case2;
}


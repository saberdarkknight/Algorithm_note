/*
Time complexity : O(N)
Space complexity : O(1)
*/


void moveZeroes(int* nums, int numsSize) {

    // check early return condition
    if ( numsSize <= 1 ){
        return;
    }
    
    // start replacement
    int left = 0;
    for ( int i = 0; i < numsSize; i++ ){
        if ( nums[i] != 0 ){
            nums[left++] = nums[i];
        }
    }
    for ( int i = left; i < numsSize; i++ ){
        nums[i] = 0;
    }
}
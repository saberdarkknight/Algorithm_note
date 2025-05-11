int removeDuplicates(int* nums, int numsSize) {
    int cur = 0;
    int num = 0;
    for (int i = 0; i<numsSize; i++){  
        if ( i == 0 ){
            nums [num] = nums[i];
            cur = nums[i];
            num += 1 ;
        }
        if (  (nums[i] > cur) && (i>0) ){
            nums[num] = nums[i] ;
            cur = nums[i];
            num += 1 ;
        } 
    }
    return num;
}
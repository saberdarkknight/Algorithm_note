/*
1) Iterate through nums using index i.
2) If nums[i] == val, shift all elements left to overwrite it.
3) Decrease numsSize after shifting to track valid elements.
4) Edge case: The approach has a nested loop, making it less efficient for large inputs.

*/

int removeElement(int* nums, int numsSize, int val) {

    int i = 0;

    while ( i < numsSize ){
        if (nums[i] == val){
            for (int j = i; j < numsSize - 1; j++){
                nums[j] = nums[j + 1];
            }
            numsSize = numsSize - 1;
        }

        if (nums[i] != val){
            i++;
        }
    }
        
   int k = numsSize;

    return k;
}
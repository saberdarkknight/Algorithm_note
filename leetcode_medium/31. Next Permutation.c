/*
input = [1, 3, 5, 4, 2]
temp_l = 1
temp_r = 3
after swap : [1, 4, 5, 3, 2 ]
Then, [5, 3, 2] should change to [2, 3, 5] so that [1, 4, 2, 3, 5] is the number we want
*/

// we need to swap the number from pos_l +1 to numsSize -1 as we ensure remaining numner follow decreasing order
void reverse(int* nums, int start, int end) {
    while (start < end) {
        int temp = nums[start];
        nums[start] = nums[end];
        nums[end] = temp;
        start++;
        end--;
    }
}

void nextPermutation(int* nums, int numsSize) {
    if (numsSize <= 1) return;

    // Find the first decreasing element from right
    int pos_l = numsSize - 2;
    while ( (pos_l >= 0) && (nums[pos_l] >= nums[pos_l + 1]) ) {
        pos_l--;
    }

    if (pos_l >= 0) {
        // Find the element just larger than nums[pos_l]
        int pos_r = numsSize - 1;
        while (nums[pos_r] <= nums[pos_l]) {
            pos_r--;
        }
        // Swap
        int temp = nums[pos_l];
        nums[pos_l] = nums[pos_r];
        nums[pos_r] = temp;
    }

   // Reverse the decreasing sequence
    reverse(nums, pos_l + 1, numsSize - 1); 
}
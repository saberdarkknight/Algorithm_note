
/*
Time complexity : O(N log N)
Space complexity : O(1)

*/

// My attempt
int compare ( const void *a, const void *b ){
    return *(int*)a - *(int*)b;
}

int majorityElement(int* nums, int numsSize) {

    int threshold = (numsSize + 1)/2;
    int cur_index = 0;
    int count = 0;
    qsort( nums, numsSize, sizeof(int), compare);

    for (int i = 0; i < numsSize; i++){
        if ( cur_index == nums[i]) {
            count++;
        } else{
            cur_index = nums[i];
            count = 1;
        }

        if ( count >= threshold ){
            break;
        }
    }
    return cur_index;
}



// option 2
// it has the same complexity as my attempt

int majorityElement(int* nums, int numsSize) {
    qsort(nums, numsSize, sizeof(int), compare);
    // The majority element must be at the middle position
    return nums[numsSize / 2];
}


// option 2

/*
Time complexity : O(N)
Space complexity : O(1)

*/

int majorityElement(int* nums, int numsSize) {
    int candidate = nums[0];
    int count = 1;
    
    // Phase 1: Find potential candidate
    for (int i = 1; i < numsSize; i++) {
        if (count == 0) {
            candidate = nums[i];
            count = 1;
        } else if (nums[i] == candidate) {
            count++;
        } else {
            count--;
        }
    }
    
    // Phase 2: Verify candidate (optional since problem guarantees majority exists)
    count = 0;
    for (int i = 0; i < numsSize; i++) {
        if (nums[i] == candidate) {
            count++;
        }
    }
    
    return candidate;
}


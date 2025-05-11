
// My attempt
bool containsDuplicate(int* nums, int numsSize) {
    int *store  = (int*) malloc( sizeof(int)* numsSize);
    int size = 1;
    store[0] = nums[0];
    bool equal = false;

    for ( int i=1; i<numsSize; i++ ){
        for ( int j=0; j<i; j++ ){
            if ( store[j] == nums[i] ){
                return true;
            } else {
                equal = false;
            }
        }
        store[i] = nums[i];
    }  
    return equal;
}


// If `numsSize` is big, it may exceed the time limit in leetcode.
// use qsort method instead

int compar ( const void* a, const void *b ){
    return ( *(int*)a - *(int*)b );
}

bool containsDuplicate(int* nums, int numsSize) {

qsort (nums, numsSize, sizeof(int), compar);

  for (int i = 0; i < numsSize - 1; i++) {
        if (nums[i] == nums[i + 1]) {
            return true;
        }
    }
    return false;
}
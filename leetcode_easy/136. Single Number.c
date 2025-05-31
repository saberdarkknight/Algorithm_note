
/*

Time complexity : O(N log N)
Space complexity : O(1)

*/

// My attempt

int compare ( const void *a, const void *b ){
    return ( *(int*) a -  *(int*)b );
}

int singleNumber(int* nums, int numsSize) {
    qsort ( nums, numsSize, sizeof(int), compare );
    for ( int i=0; i< numsSize -1; i+=2  ){        
        if ( (nums[i] != nums[i+1]) ){
            return nums[i];
        } 
    }
    return nums[numsSize-1];     
}



// Better solution
/*
Time complexity : O(N)
Space complexity : O(1)


Magic Properties of XOR:

Identity: a ^ 0 = a (XOR with 0 doesn't change the number)
Self-inverse: a ^ a = 0 (any number XOR with itself equals 0)
Commutative: a ^ b = b ^ a (order doesn't matter)
Associative: (a ^ b) ^ c = a ^ (b ^ c) (grouping doesn't matter)

Given array [2, 1, 2, 3, 1]:
1. Step by step:
0 ^ 2 ^ 1 ^ 2 ^ 3 ^ 1

2. Rearrange (due to commutative property):
0 ^ (2 ^ 2) ^ (1 ^ 1) ^ 3

3. Apply self-inverse property:
0 ^ 0 ^ 0 ^ 3

4. Apply identity property:
3
*/


int singleNumber(int* nums, int numsSize) {
    int result = nums[0];
    for (int i = 1; i < numsSize; i++){
        result ^= nums[i];
    }
    return result;
}
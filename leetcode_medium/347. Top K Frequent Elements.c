typedef struct {
    int val;
    int freq;
} Element;

int compare( const void* a, const void *b ){
    return *(int*)a - *(int*)b;
}
int compareFreq(const void* a, const void* b) {
    return ((Element*)b)->freq - ((Element*)a)->freq;
}

int* topKFrequent(int* nums, int numsSize, int k, int* returnSize) {
    
    // sort the array
    qsort(nums, numsSize, sizeof(int), compare);
    int max_val = nums[numsSize -1];
    int min_val = nums[0];
    int num_size = max_val - min_val + 1;

    // create output 
    int * output = (int*)malloc( sizeof(int)*k );

    // create a Element array to store the number and frequency 
    Element* freqArray = malloc(sizeof(Element) * num_size);
    int index;
    for (int i = 0; i < num_size; i ++ ){
        freqArray[i].val = min_val + i;
        freqArray[i].freq = 0;
    }
    for (int i = 0; i < numsSize; i ++ ){
        index = nums[i] - min_val;
        freqArray[index].freq++;
    }

    // sort the array based on freq
    qsort(freqArray, num_size, sizeof(Element), compareFreq);
    for (int i = 0; i < k; i ++ ){
        output[i] = freqArray[i].val;
    }

    *returnSize = k;
    free(freqArray);
    return output;
}
int compare ( const void * a, const void* b ){
    return *(int*)a - *(int*)b;
}

int longestConsecutive(int* nums, int numsSize) {

    if ( numsSize == 0  ){
        return 0;
    }

    qsort (nums, numsSize, sizeof(int), compare );
    int cur_index = nums[0];
    int num_max = 1;
    int temp_max = 1;
  
    for ( int i= 1; i<numsSize; i++ ){
        if ( nums[i] == cur_index ){
            continue;
        }else if ( (nums[i] == cur_index+1) ){
            cur_index = nums[i];
            temp_max +=1;
        } else{
            cur_index = nums[i];
            temp_max = 1;
        }
        if (temp_max > num_max){
            num_max = temp_max;
        }
    }

    return num_max;
}
int searchInsert(int* nums, int numsSize, int target) {

    int left = 0;
    int right = numsSize -1;
    int mid;

    while( right >= left ){
        
        int mid  = (left+ right)/2 ;

        if ( nums[mid] == target ){
            left = mid;
            break;
        } else if ( nums[mid] < target ){
            left = mid + 1;
        } else {
            right = mid -1;
        }   
    }
    return  left;
}
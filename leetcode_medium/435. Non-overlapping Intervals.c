int compare(const void* a, const void* b) {
    int* intervalA = *(int**)a;
    int* intervalB = *(int**)b;
    return intervalA[1] - intervalB[1];
}

int eraseOverlapIntervals(int** intervals, int intervalsSize, int* intervalsColSize) {

    //  use intervals[*][1] to perform sorting
    int rows = intervalsSize;
    qsort( intervals, rows, sizeof(int*), compare);
    int right = intervals[0][1];
    int remove_num = 0;

    // check if the interval is overlapping or not
    for ( int i = 1; i <  rows; i ++){
        if ( intervals[i][0] < right  ){
            remove_num++;
        }else{
            right = intervals[i][1];
        }
    }
    return remove_num;
}
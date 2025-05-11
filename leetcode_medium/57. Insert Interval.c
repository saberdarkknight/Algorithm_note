int** insert(int** intervals, int intervalsSize, int* intervalsColSize, int* newInterval, int newIntervalSize, int* returnSize, int** returnColumnSizes) {

    // create output interval
    int** output = (int**)malloc(sizeof(int*)* (intervalsSize + 1) );
    int out_size=0;
    *returnColumnSizes = malloc(sizeof(int) *  (intervalsSize + 1));
    bool insert = false;
    bool merging = false;
    int cur_start;
    int cur_end;

    for (int i =0; i < intervalsSize; i++){
        cur_start =  intervals[i][0];
        cur_end =  intervals[i][1];

        // check if the new interval inside is before current interval
        if ( ( newInterval[1] < cur_start ) && !insert){
            output[out_size] = (int*)malloc(sizeof(int) * 2);
            output [out_size][0] = newInterval[0];
            output [out_size][1] = newInterval[1];
            (*returnColumnSizes)[out_size++] = 2;
            insert = true;            
        }

        // if newinterval is not inside current interval => add current interval
        // if newinterval is inside current interval => update newinterval
        if ( (cur_end < newInterval[0]) || ( cur_start > newInterval[1]) ) {
            output[out_size] = malloc(sizeof(int) * 2);
            output[out_size][0] = cur_start;
            output[out_size][1] = cur_end;
            (*returnColumnSizes)[out_size++] = 2;
        } else {
            newInterval[0] = fmin(newInterval[0], cur_start);
            newInterval[1] = fmax(newInterval[1], cur_end);
        }
    }
    // if the newinterval is not inserted
    if (!insert) {
        output[out_size] = malloc(sizeof(int) * 2);
        output[out_size][0] = newInterval[0];
        output[out_size][1] = newInterval[1];
        (*returnColumnSizes)[out_size++] = 2;
    }   
    

    * returnSize = out_size;
    return output;
}

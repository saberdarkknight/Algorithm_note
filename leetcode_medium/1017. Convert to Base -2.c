




// Timmy

/*
2 -> 110
3 -> 111
4 -> 100
5 ->

2 -> 10 -> 2%2==0 bit0=0 -> (2/2)%2==1 -> bit1=1
3 -> 11 -> 3%2==1 bit0=1 -> (3/2)%2==1 -> bit1=1

4 & bit0 = 0 --> DC
4 & bit1 = 0 --> DC
4 & bit2 = 1 --> 4-(4)=0 finish

// remain=2
2 & bit0 = 0 --> DC --> remain=2
2 & bit1 = 1 --> 2-(-2) = remian =4 (>0)
4 & bit2 = 1 --> 4-4=0 (finish)

3 & bit0 = 1 --> 3-1 = 2
2 & bit1 = 1 --> 2-(-2) = remain =4
4 .....  = 1



*/


char* baseNeg2(int n) {
    long long int m = n;
    uint32_t bit = 1;
    int count = 0;

    char* out = (char*)malloc(32 * sizeof(char));

    int idx = 31;

    out[idx] = '\0';

    if (m == 0) {
        out[--idx] = '0';
    } else {
        while (m != 0) {
            // printf("m=%d, count=%d\n", m, count);
            if (((m & (bit << count)) >> count) == 1) { // if((m>>count)&1==1)
                if (count % 2 == 1) {
                    m = m + (bit << count);
                } else {
                    m = m - (bit << count);
                }
                out[--idx] = '1';
            } else {
                out[--idx] = '0';
            }
            count += 1;
        }
    }


    // printf("idx=%d, %d, %s\n",idx, strlen(&out[idx]), &out[idx]);

    return &out[idx];
}


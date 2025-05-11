
// My attempt

int compare ( const void * a, const void * b){
    return *(int *)a - *(int *)b;
}

int coinChange(int* coins, int coinsSize, int amount) {

    // check early return condition
    if ( amount == 0 ){
        return 0;
    }
    if ( coinsSize == 1 &&  (amount % coins[0]) != 0 ){
        return -1;
    }

    // sort the coins
    qsort( coins, coinsSize, sizeof(int), compare );

    // start the calculation
    // starting from the maximum number
    // this will nbe wrong if amount can be devided with smaller number
    int remain = amount;
    int num  = 0;
    int temp;
    for ( int i = coinsSize -1; i >= 0; i--){
        if ( ( remain > 0) && ( remain / coins[i] ) >= 1 ){
            num = num + (remain / coins[i]);
            remain = remain % coins[i];
        }
        if ( remain == 0 ){
            break;
        }
    }
    return num;    
}




// The attempt may cause error in some cases. Use DP to solve the problem

int compare ( const void * a, const void * b){
    return *(int *)a - *(int *)b;
}

int coinChange(int* coins, int coinsSize, int amount) {

    // check early return condition
    if ( amount == 0 ){
        return 0;
    }
    if ( coinsSize == 1 &&  (amount % coins[0]) != 0 ){
        return -1;
    }

    // sort the coins
    qsort( coins, coinsSize, sizeof(int), compare );

    // start the calculation
    // set an array to record the number of coins
    int remain = amount;
    int num  = 0;
    int temp;
    int* output = (int*) malloc(sizeof(int)*(amount+1));
    for ( int i =0; i <= amount; i++){
        output[i] = amount+1;
    }

    // checking the combination from 0 to amount
    output[0] = 0;
    for ( int i = 0; i <= amount; i ++){
        for ( int j = 0; j <  coinsSize; j ++){
            if ( i >= coins[j] ){
                if ( output[ i - coins[j] ] + 1 < output[i] ){
                    output[i] = output[ i - coins[j] ] + 1;
                }
            }
        }
    }

    int result = (output[amount] > amount) ? -1 : output[amount];
    free(output);
    return result;
}

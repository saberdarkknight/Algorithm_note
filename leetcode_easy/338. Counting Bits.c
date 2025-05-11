int count_bit ( int num){
    int size = 0;
    while( num > 0 ){
        num = num & (num -1);
        size += 1;
    }
    return size;
}


int* countBits(int n, int* returnSize) {
    int * output = (int*)malloc(sizeof(int)*(n+1));
    for(  int i = 0; i< n+1; i++){
        output[i] = count_bit(i);
    }
    *returnSize = n+1;
    return output;
}
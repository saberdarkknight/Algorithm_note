/*
Time complexity : O(N)
Space complexity : O(1)

Hint :
use stack to calculate the '('
if the depth is even, we assign it to A
if the depth is odd, we assign it to B

*/

int* maxDepthAfterSplit(char* seq, int* returnSize) {
    
    int size = strlen(seq);
    int* output= (int*)malloc(sizeof(int)*size);
    int depth = 0;


    for (int i = 0; i < size; i++){
        if ( seq[i] == '('){
            depth ++;
            if ( (depth%2) == 0){
                output[i] = 0;
            }else{
                output[i] = 1;
            }
        }

        if (seq[i] == ')' ){
            if ( (depth%2) == 0){
                output[i] = 0;
            }else{
                output[i] = 1;
            }
            depth--;
        }
    }

    * returnSize = size;
    return output;
}


// reduce the number of if statement

int* maxDepthAfterSplit(char* seq, int* returnSize) {
    // check the size of seq of create output
    int size = strlen(seq);
    int* output= (int*)malloc(sizeof(int)*size);
    int depth = 0;

    // use depth to assign ( )
    for (int i = 0; i < size; i++){
        if ( seq[i] == '('){
            depth ++;
            output[i] = depth%2;
        }
        if (seq[i] == ')' ){
            output[i] = depth%2;
            depth--;
        }
    }

    * returnSize = size;
    return output;
}


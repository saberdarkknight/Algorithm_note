
/*
Recursion Tree (Example: candidates = [2,3,6,7], target = 7)
Copy
Start (Target = 7)
├── Add 2 (Remaining = 5)
│   ├── Add 2 (Remaining = 3)
│   │   ├── Add 2 (Remaining = 1)
│   │   │   ├── Add 2 (Remaining = -1) → Backtrack
│   │   │   └── Add 3 (Remaining = -2) → Backtrack
│   │   └── Add 3 (Remaining = 0) → Add [2,2,3] to result
│   └── Add 3 (Remaining = 2)
│       ├── Add 3 (Remaining = -1) → Backtrack
│       └── Add 6 (Remaining = -4) → Backtrack
├── Add 3 (Remaining = 4)
│   ├── Add 3 (Remaining = 1)
│   │   ├── Add 3 (Remaining = -2) → Backtrack
│   │   └── Add 6 (Remaining = -5) → Backtrack
│   └── Add 6 (Remaining = -2) → Backtrack
└── Add 7 (Remaining = 0) → Add [7] to result
*/

void backtrack(int* candidates, int candidatesSize, int target,  int** result, int* returnSize, int** returnColumnSizes, int* current, int currentSize , int start){
    if (target == 0){
        result[*returnSize] = (int*)malloc(currentSize * sizeof(int));
        memcpy(result[*returnSize], current, currentSize * sizeof(int));
        (*returnColumnSizes)[*returnSize] = currentSize;
        (*returnSize)++;
        return;
    }
    if (target < 0){
        return;
    }

    for ( int i = start; i< candidatesSize; i++ ){
        current[currentSize] = candidates[i];
        backtrack(candidates, candidatesSize, target - candidates[i], result, returnSize, returnColumnSizes, current, currentSize + 1, i);
    }
}


int** combinationSum(int* candidates, int candidatesSize, int target, int* returnSize, int** returnColumnSizes) {
    *returnSize=0;
    *returnColumnSizes = (int *) malloc(sizeof(int*)*150);
    int ** result = (int **) malloc(sizeof(int*)*150);
    int * current = (int *) malloc(sizeof(int*)*150);
    backtrack(candidates, candidatesSize, target, result, returnSize, returnColumnSizes, current, 0, 0);
    free(current);
    return result;
}

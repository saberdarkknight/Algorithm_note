

int applyOp(int a, int b, char op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    return 0;
}

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*';
}

int* diffWaysToCompute(char* expression, int* returnSize) {

    int n = strlen(expression);
    int capacity = 100;
    int* result = (int*)malloc(sizeof(int) * capacity);
    *returnSize = 0;
    
    for (int i = 0; i < n; i++) {
        if (isOperator(expression[i])) {
            char left[32], right[32];
            strncpy(left, expression, i);
            left[i] = '\0';
            strcpy(right, expression + i + 1);

            int leftSize = 0, rightSize = 0;
            int* leftResults = diffWaysToCompute(left, &leftSize);
            int* rightResults = diffWaysToCompute(right, &rightSize);

            for (int li = 0; li < leftSize; li++) {
                for (int ri = 0; ri < rightSize; ri++) {
                    if (*returnSize >= capacity) {
                        capacity *= 2;
                        result = (int*)realloc(result, sizeof(int) * capacity);
                    }
                    result[*returnSize] = applyOp(leftResults[li], rightResults[ri], expression[i]);
                    (*returnSize)++;
                }
            }
            free(leftResults);
            free(rightResults);
        }
    }


    // if expression contains only digit => return the number
    if (*returnSize == 0) {
        int num = atoi(expression);
        result[0] = num;
        *returnSize = 1;
    }
    
    return result;
}
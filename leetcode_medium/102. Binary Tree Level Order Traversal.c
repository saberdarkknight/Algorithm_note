#define MAX_QUEUE_SIZE 2000

int** levelOrder(struct TreeNode* root, int* returnSize, int** returnColumnSizes) {
    
    // check the early return condition
    if ( root == NULL ){
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }

    // initialize queue
    struct TreeNode* queue[MAX_QUEUE_SIZE];
    int front = 0, back = 0;
    queue[back++] = root;

    // initial the output
    int** result = (int**)malloc(sizeof(int*) * MAX_QUEUE_SIZE);
    *returnColumnSizes = (int*)malloc(sizeof(int) * MAX_QUEUE_SIZE);
    *returnSize = 0;

    // use queue to store the tree  
    while (front < back) {
        int levelSize = back - front;
        result[*returnSize] = (int*)malloc(sizeof(int) * levelSize);
        (*returnColumnSizes)[*returnSize] = levelSize;

        for (int i = 0; i < levelSize; i++) {
            struct TreeNode* node = queue[front++];
            result[*returnSize][i] = node->val;

            if (node->left) {
                queue[back++] = node->left;
            }
            if (node->right) {
                queue[back++] = node->right;
            }
        }
        (*returnSize)++;
    }

    return result;
}
int compare ( const void * a, const void * b ){
    return *(int*)a - *(int*)b;
}

int check_tree(struct TreeNode* root){
    if (root == NULL){
        return 0;
    }
    return check_tree(root->left) + check_tree(root->right) + 1;
}

void put_number (struct TreeNode* root, int * nums, int *pos, int size){
    if ( root == NULL ){
        return;
    }
    put_number (  root->left, nums, pos, size);
    nums[(*pos)++] = root->val;  // share the common pos value
    put_number (  root->right, nums, pos, size);
}

int kthSmallest(struct TreeNode* root, int k) {

    // use array to store the value and sort the value 
    
    // get the tree size
    int size = check_tree (root);
    
    // put the value of node to 
    int* nums = (int*) malloc( sizeof(int) * size);
    int pos = 0;
    put_number ( root, nums, &pos, size );

    //qsort(nums, size, sizeof(int), compare);
    int result = nums[k-1];
    free (nums);
    return result;
}
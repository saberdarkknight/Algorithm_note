struct TreeNode* build(int* preorder, int* inorder, int pre_start, int pre_end, int in_start, int in_end){
    // return if pos over end
    if ( pre_start > pre_end || in_start > in_end ){
        return NULL;
    }
    // create a root
    struct TreeNode* root = (struct TreeNode* ) malloc(sizeof(struct TreeNode));
    root->val = preorder[pre_start];
    root->left = NULL;
    root->right = NULL;

    // get the starting position in inorder
    int inRootIdx;
    for (int i = in_start; i <= in_end; i ++){
        if ( root->val == inorder[i] ){
            inRootIdx = i;
            break;
        }
    }
    int leftTreeSize = inRootIdx - in_start;

    // start the recursive for next level
    root->left = build( preorder, inorder, pre_start + 1, pre_start + leftTreeSize, in_start, inRootIdx-1);
    root->right = build( preorder, inorder, pre_start + leftTreeSize + 1, pre_end, inRootIdx + 1, in_end);

    return root;
}


struct TreeNode* buildTree(int* preorder, int preorderSize, int* inorder, int inorderSize) {
    // return NULL if size is 0
    if ( preorderSize == 0 ){
        return NULL;
    }
    // start creating the tree
    return build(preorder, inorder, 0, preorderSize - 1, 0, inorderSize - 1);
}
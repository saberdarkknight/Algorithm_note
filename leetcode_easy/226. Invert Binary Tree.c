struct TreeNode* invertTree(struct TreeNode* root) {
    
    if (  root == NULL  ){
        return root;
    }

    // swap the node
    struct TreeNode* curr = root->left;
    root->left = root->right;
    root->right = curr;

    // recursive change the node
    invertTree(root->left);
    invertTree(root->right);


    return root;
}
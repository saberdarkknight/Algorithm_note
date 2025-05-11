
int maxDepth(struct TreeNode* root) {
    if (!root) return 0;
    int maxLeft = maxDepth(root->left);
    int maxRight = maxDepth(root->right);
    return fmax(maxLeft, maxRight) + 1;
}
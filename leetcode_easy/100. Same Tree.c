/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
bool isSameTree(struct TreeNode* p, struct TreeNode* q) {
    
    bool check = false;
    if (  !p && !q  ){
        return true;
    }

    if ( !p || !q ){
        return false;
    }

    if (  ( p->val == q->val )  && isSameTree( p->left, q->left ) && isSameTree( p->right, q->right ) ){
        return true;
    } else {
        return false;
    }
}
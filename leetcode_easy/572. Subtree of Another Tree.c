bool equals (struct TreeNode* r, struct TreeNode* t){
    if (r==NULL && t==NULL){
        return true;
    }else if (r==NULL || t== NULL) {
        return false;
    }
    return (r->val == t->val) && equals(r->right,t->right) && equals(r->left,t->left);
}


bool isSubtree(struct TreeNode* root, struct TreeNode* subRoot) {
    if ( (root == NULL) && (subRoot == NULL) ){
        return true;
    }

    return ( (root!=NULL) && ( equals( root, subRoot ) || isSubtree(root->right,subRoot) || isSubtree(root->left, subRoot) ) ) ;  
}
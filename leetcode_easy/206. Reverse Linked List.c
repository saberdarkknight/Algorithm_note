struct ListNode* reverseList(struct ListNode* head) {

    if ( ( head == NULL ) ||  ( head->next == NULL) ){
        return head;
    }
    struct ListNode* output = NULL; 
    
    while( head != NULL  ){
        struct ListNode* new_node = (struct ListNode*) malloc(sizeof(struct ListNode));
        new_node->val = head->val;
        new_node->next=output;
        output = new_node;
        head = head->next;  
    }

    return output;
}
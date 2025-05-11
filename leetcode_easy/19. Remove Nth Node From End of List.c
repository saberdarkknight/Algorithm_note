struct ListNode* removeNthFromEnd(struct ListNode* head, int n) {

    struct ListNode*  output = (struct ListNode* ) malloc(sizeof(struct ListNode));
    struct ListNode* head_new = head;
    head_new = head;
    int size = 0;

    while( head_new->next!=NULL ){
        size ++;
        head_new = head_new->next;
    }
    size++;  // need to add 1 again before we do not calculate the last node

    // consider the first node is deleted
    if ( size == n ){
        return head->next;
    }

    head_new = head;
    output->next = head_new;

    for ( int i =0; i<size; i++ ){
        
        if ( i == size - n -1 ){
            head_new->next=head_new->next->next;
        }else{
            head_new = head_new->next;
        }
    }
    return output->next;   
}
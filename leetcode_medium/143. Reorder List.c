void reorderList(struct ListNode* head) {
    // if size is lower than 2, then return
    if (!head || !head->next || !head->next->next) return;

    // check the size of the list
    int size = 0;
    struct ListNode* output = head;
    while( output != NULL){
        output=output->next;
        size ++;
    }

    // store the list in an array
    struct ListNode** store  = (struct ListNode**)malloc( sizeof(struct ListNode*)*size );  // avoid using struct ListNode* store[size]  to prevent from stack overflow.
    output = head;
    for (int i = 0; i<size; i++){
        store[i] = output;
        output=output->next;
    }

    // start the reorder list
    int left = 0, right = size - 1;
    while (left < right) {
        store[left]->next = store[right];
        left++;
        if (left == right) break;
        store[right]->next = store[left];
        right--;
    }

    store[left]->next = NULL; // terminate the list
    
}
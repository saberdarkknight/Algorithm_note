bool hasCycle(struct ListNode *head) {
    
    // if list has only single node, return false
    if ( (head == NULL) || (head->next == NULL)  ){
        return false;
    }

    struct ListNode* next1 = head;
    struct ListNode* next2 = head;

    while ( ( next1 !=NULL) && ( next2 !=NULL) && ( next2->next!=NULL) ){
        
        next1 = next1->next;
        next2 = next2->next->next;

        if ( (next1 == next2) ){
            return true;
        }
    }
    return false;
}
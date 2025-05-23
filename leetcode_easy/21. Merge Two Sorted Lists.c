struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2) { 
    struct ListNode first;
    first.val = -1;
    first.next = NULL;
    struct ListNode* output = &first ;

    while ( (list1 != NULL) && (list2 != NULL) ){ 
        if ( list1->val <  list2->val ){
            output->next =  list1;
            list1 = list1-> next;
        } else {
            output->next =  list2;
            list2 = list2->next;
        }
        output = output->next;
    }
    output->next = (list1 == NULL) ? list2 : list1;
    return first.next;
}
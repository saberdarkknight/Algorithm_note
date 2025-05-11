
struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    
    struct ListNode* head = malloc(sizeof(struct ListNode));
    head->val=0;
    head->next=NULL;
    struct ListNode* output = head;

    int sum = 0;

    while (  ( l1 != NULL ) || ( l2 != NULL ) || (sum) ){
        if ( l1 != NULL ){
            sum = sum + l1->val;
            l1= l1->next;
        }

        if ( l2 != NULL ){
            sum = sum + l2->val;
            l2= l2->next;
        }
        
        output->next = malloc(sizeof(struct ListNode));
        output->next->val = sum%10;
        output->next->next=NULL;      
        output = output->next;
        sum /= 10; 
    }
    return head->next;
}

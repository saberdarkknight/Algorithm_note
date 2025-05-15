/*

Time complexity : O(N)
Space complexity : O(1)


*/


struct ListNode* rotateRight(struct ListNode* head, int k) {

    // check the early return condition
    if (!head || !head->next || k == 0){
        return head;    
    } 

    // check the size of linklist
    int size = 1;
    struct ListNode* first = head;
    while( first->next != NULL ){
        size ++;
        first = first->next;
    }

    // check the early return condition
    int move = k%size;
    if (  move == 0  ){
        return head;
    }

    // link linklist as cycle
    first->next = head;

    // get the move step and create new link list     
    // move forward and find the starting point
    struct ListNode* end = head;
    for (int i = 0; i < size - move - 1; i++) {
        end = end->next;
    }
    struct ListNode* head_new = end->next;
    end->next = NULL;

    return head_new;
}
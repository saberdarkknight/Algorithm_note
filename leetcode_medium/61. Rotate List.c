/*

Time complexity : O(N)
Space complexity : O(N)


*/


// My attempt
struct ListNode* rotateRight(struct ListNode* head, int k) {

    // check the early return condition
    if (!head || !head->next || k == 0){
        return head;    
    } 

    // check the size of linklist
    struct ListNode* total[501];
    struct ListNode* ptr = head;
    int index = 0;
    while( ptr!=NULL ){
        total[index++] = ptr;
        ptr = ptr->next;
    }

    // check shift number
    int shift = k%index;
    if (shift == 0) {
        return head;
    }

    // start shifting
    total[index - 1]->next = head; 
    struct ListNode* newHead = total[index - shift];
    total[index - shift -1]->next = NULL;

    return newHead;
}




// Better approach
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
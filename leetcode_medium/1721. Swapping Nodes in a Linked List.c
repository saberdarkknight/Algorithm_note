/*

Time complexity : O(N)
Space complexity : O(N)
*/

struct ListNode* swapNodes(struct ListNode* head, int k) {
    // check early return condition
    if (head == NULL) return head;

    // store the node in an array
    struct ListNode* node [100000];
    struct ListNode* temp = head;
    int size = 0;
    while( temp!=NULL ){
        node[size++]=temp;
        temp = temp->next;
    }

    // swap the value  
    int first_idx = k - 1;           // k-th node from beginning (0-based)
    int second_idx = size - k;  
    int temp_val = node[first_idx]->val;
    node[first_idx]->val = node[second_idx]->val;
    node[second_idx]->val = temp_val;

    return head;
}



/*

Time complexity : O(N)
Space complexity : O(1)

Hint :
use two-pointer method to initally get k-th from the back

*/


struct ListNode* swapNodes(struct ListNode* head, int k) {
    if (head == NULL || head->next == NULL) {
        return head;
    }
    
    // Find the length of the list and the k-th node from beginning
    struct ListNode* first = head;
    struct ListNode* second = head;
    int length = 1;
    
    // Move first pointer to k-th position
    for (int i = 1; i < k; i++) {
        first = first->next;
        length++;
    }
    
    // Continue moving first to end while moving second
    struct ListNode* temp = first;
    while (temp->next != NULL) {
        temp = temp->next;
        second = second->next;
        length++;
    }
    
    // Now first points to k-th node from beginning
    // second points to k-th node from end
    
    // Swap the values (not the nodes themselves)
    int tempVal = first->val;
    first->val = second->val;
    second->val = tempVal;
    
    return head;
}


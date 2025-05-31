


/*
Time complexity : O(N)
Space complexity : O(1)
*/

struct ListNode* reverseBetween(struct ListNode* head, int left, int right) {
    if (!head || left == right) return head;

    // Step 1: Create dummy node
    struct ListNode dummy;
    dummy.next = head;
    struct ListNode* prev = &dummy;

    // Step 2: Move prev to the node just before 'left'
    for (int i = 1; i < left; i++) {
        prev = prev->next;
    }

    // Step 3: Reverse sublist between left and right
    struct ListNode* curr = prev->next;
    struct ListNode* next = NULL;

    for (int i = 0; i < right - left; i++) {
        next = curr->next;
        curr->next = next->next;
        next->next = prev->next;
        prev->next = next;
    }

    return dummy.next;
}

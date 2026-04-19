





Attemp 1


typedef struct Item{
    int val;
    int idx;   // which list this value came from
} Item;

int compare(const void *a, const void *b) {
    return ((Item *)a)->val - ((Item *)b)->val;
}

struct ListNode* mergeKLists(struct ListNode** lists, int listsSize) {

    // check the early return condition
    if (lists == NULL || listsSize == 0){
        return NULL;
    }

    // 
    struct ListNode head;
    head.next = NULL;
    struct ListNode *output = &head;
    struct ListNode *head_ptr = &head;
    struct Item *store = malloc(sizeof(Item) * listsSize);
    while(1){
        int count = 0;

        // Collect current heads of non-empty lists
        for(int i = 0; i < listsSize; i++){
            if (lists[i] != NULL){
                store[count].val = lists[i]->val;
                store[count].idx = i;
                count++;
            }
        }

         // All lists exhausted
        if(count == 0){
            break;
        }
        // Sort current head values
        qsort(store, count, sizeof(Item), compare);

        // get the smallest index
        int minIdx = store[0].idx;
        struct ListNode *node = lists[minIdx];

        // Advance that list
        lists[minIdx] = lists[minIdx]->next;

        // Append node to result
        head_ptr->next = node;
        head_ptr = head_ptr->next;
    }
    free(store);
    return output->next;
}




Attemp 2

typedef struct Item{
    int val;
    int idx;   // which list this value came from
} Item;

int compare(const void *a, const void *b) {
    return ((Item *)a)->val - ((Item *)b)->val;
}


void heap_push(Item *heap, int *size, Item item) {
    heap[*size] = item;
    int i = (*size)++;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (compare(&heap[parent], &heap[i]) <= 0) break;
        Item tmp = heap[parent]; heap[parent] = heap[i]; heap[i] = tmp;
        i = parent;
    }
}


Item heap_pop(Item *heap, int *size) {
    Item top = heap[0];           // 1. save the min (root)
    (*size)--;                    // shrinke size
    heap[0] = heap[*size];      // 2. move last element to root
    int i = 0;
    while (1) {                   // 3. sift down
        int l = 2*i+1, r = 2*i+2, smallest = i;
        if (l < *size && compare(&heap[l], &heap[smallest]) < 0) smallest = l;
        if (r < *size && compare(&heap[r], &heap[smallest]) < 0) smallest = r;
        if (smallest == i) break; // already smaller than both children → done
        Item tmp = heap[i]; heap[i] = heap[smallest]; heap[smallest] = tmp;
        i = smallest;
    }
    return top;
}

struct ListNode* mergeKLists(struct ListNode** lists, int listsSize) {

    // check the early return condition
    if (lists == NULL || listsSize == 0){
        return NULL;
    }

    // 
    struct ListNode head;
    head.next = NULL;
    struct ListNode *output = &head;
    struct ListNode *head_ptr = &head;
    struct Item *store = malloc(sizeof(Item) * listsSize);
    int size  = 0;

    // fill the list first
    for (int i = 0; i < listsSize; i++) {
        if (lists[i] != NULL) {
            heap_push(store, &size, (Item){lists[i]->val, i});
        }
    }


    while(size > 0){

        // pop the min value 
        Item min = heap_pop(store, &size);
        int minIdx = min.idx;


        // Advance that list
        struct ListNode *node = lists[minIdx];
        lists[minIdx] = lists[minIdx]->next;

        if (lists[minIdx] != NULL) {
            heap_push(store, &size, (Item){lists[minIdx]->val, minIdx});
        }

        // Append node to result
        head_ptr->next = node;
        head_ptr = head_ptr->next;
    }

    free(store);
    return output->next;
}




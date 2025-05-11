typedef struct Node {
    int key;
    int value;
    struct Node* prev;
    struct Node* next;
    UT_hash_handle hh; /* makes this structure hashable */
} Node;


typedef struct {
    int cap;
    Node* leftMost;     // LRU
    Node* rightMost;    // MRU
} LRUCache;

Node* hashedNodes = NULL;

void removeNode(Node* node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

// Insert node at the right (MRU), just before the rightMost node
void insertNode(Node* node, LRUCache* cache)
{
    Node* prev = cache->rightMost->prev;
    Node* next = cache->rightMost;
    
    prev->next = node;
    next->prev = node;
    
    node->next = next;
    node->prev = prev;
}


LRUCache* lRUCacheCreate(int capacity) {
    
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache)); // Allocate space for LRUCache

    cache->cap = capacity;  // Set the capacity
    
    // Allocate space for the leftMost and rightMost nodes
    cache->leftMost = (Node*)malloc(sizeof(Node));
    cache->rightMost = (Node*)malloc(sizeof(Node));
    
    // Initialize the nodes where we connect the leftMost and rightMost to form the list
    cache->leftMost->prev = NULL;
    cache->leftMost->next = cache->rightMost;
    
    cache->rightMost->prev = cache->leftMost;
    cache->rightMost->next = NULL;
    
    return cache;
}

int lRUCacheGet(LRUCache* obj, int key) {
    Node* node;
    HASH_FIND_INT(hashedNodes, &key, node);
    
    // Check if the key exists
    if(node) {
        // Remove the node from its current position in the list
        removeNode(node);
        // Insert the node at the right (MRU)
        insertNode(node, obj);
        return node->value;
    }
    return -1;
}

void lRUCachePut(LRUCache* obj, int key, int value) {
    Node* node;
    HASH_FIND_INT(hashedNodes, &key, node);
    
    // Check if the key exists
    if(node) {
        // Remove the node from its current position in the list
        removeNode(node);
        
        // Update the node's current value
        node->value = value;
    } else {
        // Allocate space for a new node
        node = (Node*)malloc(sizeof(Node));
        // Initialize the node
        node->key = key;
        node->value = value;
        
        // Add the new node to the hash
        HASH_ADD_INT(hashedNodes, key, node);
    }
    
    // Insert the node at the right (MRU)
    insertNode(node, obj);
    
    // Check if we have exceeded the capacity after adding the new node
    if(HASH_COUNT(hashedNodes) > obj->cap) {
        Node* LRUNode = obj->leftMost->next;
        
        removeNode(LRUNode); // Remove the left node (LRU) from the list
        
        HASH_DEL(hashedNodes, LRUNode); // Remove the node from the hash
        
        free(LRUNode);// Deallocate the node
    }
}

void lRUCacheFree(LRUCache* obj) {
    // Deallocate the nodes first
    free(obj->leftMost);
    free(obj->rightMost);    
    
    free(obj); // Deallocate the LRUCache
    
    Node *currentNode, *tmp; // Delete and deallocate all the nodes from the hash

    HASH_ITER(hh, hashedNodes, currentNode, tmp) {
        HASH_DEL(hashedNodes, currentNode);  /* delete; nodes advances to next */
        free(currentNode);
   }
}


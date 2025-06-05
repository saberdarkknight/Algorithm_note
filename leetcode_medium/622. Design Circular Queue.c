

typedef struct {
    int* queue;     // store quese element
    int capacity;   // size of the queue
    int count;      // current size of the queue
    int head;       // current position to perforn enqueue
    int tail;       // position to perform dequeue
} MyCircularQueue;


MyCircularQueue* myCircularQueueCreate(int k) {
    MyCircularQueue*  obj = (MyCircularQueue*) malloc (sizeof(MyCircularQueue));
    obj->count = 0;
    obj->tail = 0;
    obj->head = 0;
    obj->capacity = k;
    obj->queue = (int*) malloc(sizeof(int)*k);
    return obj;
    
}

bool myCircularQueueEnQueue(MyCircularQueue* obj, int value) {
    // return false if queue is full
    if (obj->count == obj->capacity){
        return false;
    }
    obj->queue[obj->tail] = value;
    obj->tail = (obj->tail + 1) % obj->capacity;
    obj->count++;
    return true;
}

bool myCircularQueueDeQueue(MyCircularQueue* obj) {
    // return false if queue is empty
    if (obj->count == 0){
        return false;
    }
    obj->head = (obj->head + 1) % obj->capacity;
    obj->count--;
    return true;
}

int myCircularQueueFront(MyCircularQueue* obj) {
    // return -1 if front is empty
    if (obj->count == 0) {
        return -1;
    }
    return obj->queue[obj->head];
}

int myCircularQueueRear(MyCircularQueue* obj) {
    // return -1 if front is empty
    if (obj->count == 0) {
        return -1;
    }
    int rearIndex = (obj->tail - 1 + obj->capacity) % obj->capacity;
    return obj->queue[rearIndex];
}

bool myCircularQueueIsEmpty(MyCircularQueue* obj) {
    return (obj->count == 0);
}

bool myCircularQueueIsFull(MyCircularQueue* obj) {
    return (obj->count == obj->capacity);    
}

void myCircularQueueFree(MyCircularQueue* obj) {
    if (obj->queue) {
        free(obj->queue);
    }
    
    free(obj);
}


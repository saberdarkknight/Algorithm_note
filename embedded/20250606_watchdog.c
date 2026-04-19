
/*
Embedded Systems Interview Question: Watchdog Timer Implementation
Problem Statement
You're designing a safety-critical embedded system that requires a software watchdog timer to prevent system hangs. 
The system has multiple tasks running, and you need to implement a watchdog mechanism that:

Monitors multiple tasks - Each task must "kick" the watchdog within its deadline
Handles different timeout periods - Different tasks may have different criticality levels
Provides system recovery - When a timeout occurs, the system should attempt recovery before a full reset
Minimizes resource usage - The implementation should be efficient for a resource-constrained embedded system

Requirements
Implement a WatchdogController class/module with the following functionality:
typedef enum {
    WATCHDOG_OK,
    WATCHDOG_TIMEOUT,
    WATCHDOG_INVALID_ID,
    WATCHDOG_MAX_TASKS_REACHED, 
    WATCHDOG_TIMEOUT_INVALID
} watchdog_status_t;

typedef enum {
    RECOVERY_RESTART_TASK,
    RECOVERY_SYSTEM_RESET
} recovery_action_t;



// Function prototypes to implement:
void watchdog_init(void);  // initialize the wachdog
watchdog_status_t watchdog_register_task(uint8_t task_id, uint32_t timeout_ms, recovery_action_t recovery); // register the task based on task ID
watchdog_status_t watchdog_unregister_task(uint8_t task_id);   // remove the task based on task ID
watchdog_status_t watchdog_kick(uint8_t task_id);   // Kick (reset) the watchdog for a specific task
void watchdog_tick(void); // Called by system timer (e.g., every 10ms)
watchdog_status_t watchdog_get_status(uint8_t task_id, uint32_t* remaining_time_ms); // get the remaining_time_ms from watchdog based on task ID



Constraints

Maximum 8 concurrent tasks can be monitored
System timer resolution is 10ms
Memory usage should be minimized (consider using bit fields, efficient data structures)
The watchdog_tick() function will be called from an interrupt context
When a timeout occurs, call system_recovery_handler(task_id, recovery_action)

Example Usage

// Register critical tasks
watchdog_register_task(1, 1000, RECOVERY_SYSTEM_RESET);    // Task 1: 1 second timeout
watchdog_register_task(2, 500, RECOVERY_RESTART_TASK);     // Task 2: 500ms timeout

// In task loops:
// Task 1
while(1) {
    do_critical_work();
    watchdog_kick(1);  // Reset watchdog for task 1
    delay(800);        // Simulate work
}

// Task 2  
while(1) {
    do_normal_work();
    watchdog_kick(2);  // Reset watchdog for task 2
    delay(400);        // Simulate work
}

Follow-up Questions

How would you handle the case where watchdog_tick() itself gets interrupted or delayed?
What happens if a task tries to kick the watchdog too frequently? Should there be a minimum interval?
How would you extend this to support hierarchical watchdogs (e.g., a master watchdog monitoring the watchdog controller itself)?
In a multi-core system, how would you modify this design to work across different cores?
What additional features would you add for debugging watchdog timeouts in development vs. production?

Evaluation Criteria

Correctness: Does the implementation handle all edge cases?
Efficiency: Minimal memory usage and fast execution in interrupt context
Safety: Proper handling of race conditions and interrupt safety
Code Quality: Clean, readable, and maintainable code structure
System Design: Understanding of embedded systems constraints and real-world considerations
*/





// My attempt

#define max_task 8
#define time_resolution 10
#define INVALID_TASK_ID 0

typedef enum {
    WATCHDOG_OK,
    WATCHDOG_TIMEOUT,
    WATCHDOG_INVALID_ID,
    WATCHDOG_MAX_TASKS_REACHED, 
    WATCHDOG_TIMEOUT_INVALID
} watchdog_status_t;

typedef enum {
    RECOVERY_RESTART_TASK,
    RECOVERY_SYSTEM_RESET
} recovery_action_t;


typedef struct task {
    uint8_t task_id;
    uint32_t timeout;
    uint32_t time_remain;
    recovery_action_t recovery_action;
    bool is_active;
} task;


typedef struct watchdog_registry_t {
    task task_list[max_task];
    uint8_t count;
} watchdog_registry_t;

static watchdog_registry_t watchdog_registry = {0};



// Function prototypes to implement:

void watchdog_init(void) {
    for (uint8_t i = 0; i < MAX_TASKS; i++) {
        watchdog_registry.tasks[i].is_active = false;
    }
    watchdog_registry.active_count = 0;
}


static int8_t find_task_index(uint8_t task_id) {
    for (uint8_t i = 0; i < MAX_TASKS; i++) {
        if (watchdog_registry.tasks[i].is_active && 
            watchdog_registry.tasks[i].task_id == task_id) {
            return i;
        }
    }
    return -1;  // Not found
}

static int8_t find_empty_slot(void) {
    for (uint8_t i = 0; i < MAX_TASKS; i++) {
        if (!watchdog_registry.tasks[i].is_active) {
            return i;
        }
    }
    return -1;  // No empty slot
}

watchdog_status_t watchdog_register_task(uint8_t task_id, uint32_t timeout_ms, recovery_action_t recovery){
    // check if timeout value is invalid
    if( timeout_ms == 0){
        return WATCHDOG_TIMEOUT_INVALID;
    }
    // check if the new task_id is invalid
    if( task_id == INVALID_TASK_ID){
        return WATCHDOG_INVALID_ID;
    }
    // check if max number of task reached
    if( tasks->count >= max_task ){
        return WATCHDOG_MAX_TASKS_REACHED;
    }
    // check if the id existed
    if (find_task_index(task_id) >= 0) {
        return WATCHDOG_INVALID_ID; 
    }

    // get the index of task is not active
    int8_t slot = find_empty_slot();

    watchdog_registry.tasks[slot].task_id = task_id;
    watchdog_registry.tasks[slot].timeout_ms = timeout_ms;
    watchdog_registry.tasks[slot].remaining_ms = timeout_ms;
    watchdog_registry.tasks[slot].recovery_action = recovery;
    watchdog_registry.tasks[slot].is_active = true;
    watchdog_registry.count++;

    return WATCHDOG_OK;
}

// remove task based on task ID
watchdog_status_t watchdog_unregister_task(uint8_t task_id) {
    if (task_id == INVALID_TASK_ID) {
        return WATCHDOG_INVALID_ID;
    }
    
    int8_t index = find_task_index(task_id);
    if (index < 0) {
        return WATCHDOG_INVALID_ID;
    }
    
    // Clear task slot
    watchdog_registry.tasks[index].is_active = false;
    watchdog_registry.active_count--;
    
    return WATCHDOG_OK;
}


// Kick (reset) the watchdog for a specific task
watchdog_status_t watchdog_kick(uint8_t task_id){
    // check if the task_id is invalid
    if( task_id == INVALID_TASK_ID){
        return WATCHDOG_INVALID_ID;
    }

    // get index of given task_id
    int8_t index = find_task_index(task_id);
    if (index < 0) {
        enable_interrupts();
        return WATCHDOG_INVALID_ID;
    }

    watchdog_registry.tasks[index].remaining_ms = watchdog_registry.tasks[index].timeout_ms;
    return WATCHDOG_OK;
}

// Called by system timer (e.g., every 10ms)
void watchdog_tick(void){

    for( uint8_t i = 0; i < max_task; i++ ){
        if (!watchdog_registry.tasks[i].is_active) {
            continue;
        }

        if( watchdog_registry.tasks[i].remaining_ms <= TIMER_RESOLUTION_MS ){
            // get the task ID and recovery action. Reset timeout value and perform system_recovery_handler
            uint8_t task_id = watchdog_registry.tasks[i].task_id;
            recovery_action_t action = watchdog_registry.tasks[i].recovery_action;
            watchdog_registry.tasks[i].remaining_ms = watchdog_registry.tasks[i].timeout_ms;

            system_recovery_handler(task_id, recovery_action);
        } else {
            watchdog_registry.tasks[i].remaining_ms -= TIMER_RESOLUTION_MS;
        }
    }
}


// Get current status and remaining time for a task
watchdog_status_t watchdog_get_status(uint8_t task_id, uint32_t* remaining_time_ms){
    // check task ID and remaining_time_ms
    if( task_id == INVALID_TASK_ID ) {
        return WATCHDOG_INVALID_ID;
    }

    if ( remaining_time_ms == NULL) {
        return WATCHDOG_TIMEOUT_INVALID;
    }

    // get the index of given task ID
    int8_t index = find_task_index(task_id);
    if (index < 0) {
        return WATCHDOG_INVALID_ID;
    }

    *remaining_time_ms = watchdog_registry.tasks[index].remaining_ms;
    return WATCHDOG_OK;
}



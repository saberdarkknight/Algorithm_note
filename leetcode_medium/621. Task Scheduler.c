#define alphabet_num 26

int leastInterval(char* tasks, int tasksSize, int n) {

    // calculate the total number of alphabet
    int alphabet[alphabet_num];
    memset(alphabet, 0, alphabet_num);
    for ( int i = 0; i < tasksSize; i++ ){
        alphabet[ tasks[i] - 'A' ] ++;
    }

    // find which char has the maximum showing 
    int max_show = 0;
    for ( int i = 0; i < alphabet_num; i++ ){
        if (alphabet[i] > max_show){
            max_show = alphabet[i];
        }
    }

    // find how many char showing time equal max_show
    int countMax = 0;
    for (int i = 0; i < alphabet_num; i++) {
        if (alphabet[i] == max_show) {
            countMax++;
        }
    }


    // we need at least require =(max_show - 1)*(n+1) + countMax to allocate the task
    // if tasksize > require, then tasksize is enough to execute the task
    int partCount = max_show - 1;
    int partLength = n + 1;
    int emptySlots = partCount * partLength + countMax;

    return (emptySlots > tasksSize) ? emptySlots : tasksSize;
}
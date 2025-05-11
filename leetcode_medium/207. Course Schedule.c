bool dfs( int course, bool * visited, bool *visiting, bool ** link, int numCourses){
    if (visiting[course]) return true;  // Detected a cycle
    if (visited[course]) return false;  // Already checked and safe

    visiting[course] = true;

    for (int i = 0; i < numCourses; i++) {
        if (link[course][i]) {
            if (dfs(i, visited, visiting, link, numCourses)) {
                return true;
            }
        }
    }
    
    visiting[course] = false;
    visited[course] = true;

    return false;
}


bool canFinish(int numCourses, int** prerequisites, int prerequisitesSize, int* prerequisitesColSize) {
    
    // if prerequisites is empty, return false
    if ( prerequisitesSize == 0) {
        return true; 
    }

    // create a matrix with number of courses
    // if there is a link between a and b, we give true, then we check the lower matrix to check if there is value
    bool **link = (bool **)malloc(sizeof(bool*) * numCourses);
    for (int i = 0; i < numCourses; i++) {
        link[i] = (bool *)calloc(numCourses, sizeof(bool));
    }

    // give the value to the matrix
    for ( int i = 0; i < prerequisitesSize; i++){
        link [ prerequisites[i][1] ][ prerequisites[i][0] ] = true;
        if ( prerequisites[i][1] == prerequisites[i][0] ){
            return false;
        }
    }

    // check the matrix
    bool* visited = (bool*)calloc(numCourses, sizeof(bool));
    bool* visiting = (bool*)calloc(numCourses, sizeof(bool));

    for ( int i =0; i < numCourses; i++){
        if (!visited[i]) {
            if (dfs(i, visited, visiting, link, numCourses)){
                return false;
            }
        }
    }

    free(link);
    free(visited);
    free(visiting);
    return true;
}

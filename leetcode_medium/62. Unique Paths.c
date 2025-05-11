
// my attempt of using recursive method

int check_path ( int m, int n, int start_row, int start_col ){
    // over the boundry
    if (start_row >= m || start_col >= n) {
        return 0; 
    }
    // reach the goal
    if ( ( start_col == n-1) && ( start_row == m -1 ) ){
        return 1;
    }
    return check_path (m, n, start_row + 1, start_col ) + check_path (m, n, start_row, start_col + 1) ;
}

int uniquePaths(int m, int n) {
    return check_path( m, n, 0, 0);
}


//Use only recursive method will be over time limit, it is better to use DP + recursive method

int dp[101][101]; 

int check_path ( int m, int n, int start_row, int start_col ){
    // over the boundry
    if (start_row >= m || start_col >= n) {
        return 0; 
    }
    // reach the goal
    if ( ( start_col == n-1) && ( start_row == m -1 ) ){
        return 1;
    }
    if (dp[start_row][start_col] != -1) {
        return dp[start_row][start_col];
    }
    dp[start_row][start_col] = check_path(m, n, start_row + 1, start_col) + check_path(m, n, start_row, start_col + 1);
    return dp[start_row][start_col];
}

int uniquePaths(int m, int n) {
    // initialize the matrix
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            dp[i][j] = -1;
        }
    }

    return check_path( m, n, 0, 0);
}


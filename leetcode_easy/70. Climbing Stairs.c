
//  My attempt

int climbStairs(int n) {
    if (n <= 2) {
        return n;
    }
    return climbStairs(n - 1) + climbStairs(n - 2);
}



// Better solution
// If `n` is big, it may exceed the time limit in leetcode.

int climbStairs(int n) {
if ( n <=2){
    return n;
} else {
    int dp [ n ];
    dp [0] = 1;
    dp [1] = 2;

    for ( int i=2 ; i< n; i++){
        dp[i] = dp[i-1] + dp[i-2];
    }
    return dp[n-1];
}

}

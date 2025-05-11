
// Be aware of using long long to represent the number to prevent from overflow.


int divide(int dividend, int divisor) {
    // check the early return condition
    if ( divisor  == 0 ){
        return INT_MAX;
    }
    if ( divisor == 1 ){
        return dividend;
    }
    if ( divisor == -1 ){
        if (dividend == INT_MIN) {
            return INT_MAX;
        }
        return -dividend;
    }

    // we need to use a variable to store the sign of the result
    // use long long to store the variable to prevent from overflow
    long long a = llabs((long long)dividend);
    long long b = llabs((long long)divisor);
    int sign = ((dividend < 0) ^ (divisor < 0)) ? -1 : 1;
    int result = 0;

    while (a >= b) {
        long long temp = b, multiple = 1;
        while (a >= (temp << 1)) {
            temp <<= 1;
            multiple <<= 1;
        }
        a -= temp;
        result += multiple;
    }

    // Apply the sign
    if ((dividend < 0) ^ (divisor < 0)) {
        result = -result;
    }
    return result;
}

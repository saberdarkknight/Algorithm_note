int reverse(int x){

    bool negative = x<0? true:false;
    int output = 0;
    int remain;
    bool start = false;
    while (x != 0){
        remain = x %10;

        // check whether it will overflow in positive number
        // INT_MAX = 2147483647, if current output 214748364, we should add less than 7
        if (output > INT_MAX / 10 || (output == INT_MAX / 10 && remain > 7)) {
            return 0;
        }
        // check whether it will overflow in negative number
        // INT_MAX = -2147483648, if current output -214748364, we should add less than 8
        if (output < INT_MIN / 10 || (output == INT_MIN / 10 && remain < -8)) {
            return 0;
        }

        // we do not need to check if remain is 0 
        output = output * 10 + remain;
        x /= 10;
    }

    return output;
}
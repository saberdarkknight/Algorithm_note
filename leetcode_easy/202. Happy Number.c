/*
This should determine the cycle.
During the iteration, it should eventually becomes a single digit number.
Therefore, we can use recursive method to deal with sigle digit case.
*/

bool isHappy(int n) {
    if ( (n==1)  || (n==7) ){
        return true;
    }else if (n < 10){
        return false;
    }else {
        int temp = 0;
        while( n > 0){
            temp = temp + (n%10) * (n%10);
            n = n/10;
        }
        return isHappy(temp);
    }  
}
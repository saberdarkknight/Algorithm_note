
// My attempt

int maxArea(int* height, int heightSize) {
    
    // find the max height
    int max_height = 0;
    for ( int i=0; i< heightSize; i++){
        if ( height[i] > max_height ){
            max_height = height[i];
        }
    }

    // if max height = 0, return 0
    int contain = 0;
    if ( max_height == 0 ){
        return contain;
    }

    // search for max container
    int left = 0;
    int right = heightSize -1;
    bool left_check = false;
    bool right_check = false;
    int temp_contain = 0;
    while ( max_height > 0 ){
        if ( (height[left] >= max_height) && (!left_check)  ){
            left_check = true;
        } 


        if ( (height[right] >= max_height) && (!right_check)  ){
            right_check = true;
        } 

        if ( !right_check ){
            right --;
        }
        if ( !left_check ){
            left ++;
        }

        if ( left_check && right_check ){
            temp_contain = max_height * ( right - left );
            if ( temp_contain > contain ){
                contain = temp_contain;
            }
            left = 0;
            right = heightSize -1;
            max_height --;
            left_check = false;
            right_check = false;
        }
    }
    return contain;
}


/*
Better answer 
We do not need to find the maximum value.
We just need to find maximum value between left and right index.
Then, the side with lower height will move forward.
*/

int maxArea(int* height, int heightSize) {
    int left = 0;
    int right = heightSize - 1;
    int max_area = 0;

    while (left < right) {
        int h = (height[left] < height[right]) ? height[left] : height[right];
        int width = right - left;
        int area = h * width;

        if (area > max_area) {
            max_area = area;
        }

        // move the shorter size so that we have chance to find the higher value
        if (height[left] < height[right]) {
            left++;
        } else {
            right--;
        }
    }

    return max_area;
}
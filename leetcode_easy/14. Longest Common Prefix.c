/*

Remember to give output with `static char` and use `memset`

`output` 如果定義成 `char output[201]` , 這個陣列會被分配在 stack（堆疊）上, 則 function 結束之後會刪除該變量因為


使用 malloc 的版本

```C=
char *output = (char*) malloc(201);
memset(output, 0, 201);
```
優點：
- 記憶體空間是動態分配的，函數結束後仍然有效，不會導致未定義行為（Undefined Behavior）。
- 支援多次呼叫此函數，互不干擾（因為每次都分配新的空間）。
- 更接近「正確且通用」的 C 語言習慣。

缺點：
- 需要手動釋放記憶體（free()），否則會導致記憶體洩漏（memory leak）。

*/



char* longestCommonPrefix(char** strs, int strsSize) {

    if ( strsSize == 0 ){
        return "";
    }

    char *output = (char*) malloc(201); // or use static char output [201];
    memset(output, 0, sizeof(output));


    for ( int i =0; strs[0][i] != '\0'; i++ ){
        char  current = strs[0][i];

        for (int j = 1; j < strsSize; j++) {
            if (strs[j][i] != current || strs[j][i] == '\0') {
                return output; 
            }
        }
        output [i] = current;
    }
    return output;   
}



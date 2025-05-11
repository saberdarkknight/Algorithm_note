bool check ( char** board, int row_size, int col_size, char* word, int row_start, int col_start, int word_pos ){
    if (word_pos == strlen(word)){
        return true;
    }

    if (row_start < 0 || row_start >= row_size || col_start < 0 || col_start >= col_size || board[row_start][col_start] != word[word_pos]) {
        return false;
    }

    char temp = board[row_start][col_start];
    board[row_start][col_start] = '#';
    bool found =check(board, row_size, col_size, word, row_start + 1, col_start, word_pos + 1) ||
                check(board, row_size, col_size, word, row_start - 1, col_start, word_pos + 1) ||
                check(board, row_size, col_size, word, row_start, col_start + 1, word_pos + 1) ||
                check(board, row_size, col_size, word, row_start, col_start - 1, word_pos + 1);
    board[row_start][col_start]  = temp;

    return found;
}

bool exist(char** board, int boardSize, int* boardColSize, char* word) {
    
    int size = strlen (word);
    int row_size = boardSize;
    int col_size = * boardColSize;
    bool found = false;
    for ( int i = 0; i < row_size; i ++ ){
        for ( int j = 0; j < col_size; j++){
            if ( word[0] == board[i][j] ){
                found = found || check ( board, row_size, col_size,  word, i, j, 0);
            }
        }
    }
    return found;
}
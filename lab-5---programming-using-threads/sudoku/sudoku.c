#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9


// Sudoku board. Modify this to test your checker!
// 0 == empty cell; 1-9 is the filled in digit.
int board[SIZE][SIZE] = {
    {1,0,0,0,0,0,0,0,0},
    {0,2,0,0,0,0,0,0,0},
    {0,0,3,0,0,0,0,0,0},
    {0,0,0,4,0,0,0,0,0},
    {0,0,0,0,5,0,0,0,0},
    {0,0,0,0,0,6,0,0,0},
    {0,0,0,0,0,0,7,0,0},
    {0,0,0,0,0,0,0,8,0},
    {0,0,0,0,0,0,0,0,9},
};

bool row_check[SIZE];
bool col_check[SIZE];
bool box_check[SIZE];

void printBoard(int board[SIZE][SIZE]) {
    for(int i = 0; i<SIZE; i++) {
        for(int j = 0; j<SIZE; j++) {
            printf("%5d", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Checks the given row for duplicate numbers, and updates the row_check
// value for that row appropriately. If no number is repeated in that row,
// row_check[row] will be set to true; otherwise, it will be false.
void* checkRow(void* args) {
  int frequency[SIZE];
  int i;
  
  for(i=0; i<SIZE; i++){
    frequency[i] = 0;
  }
  int row = *((int *)args);
  
  for(i=0; i<SIZE; i++){
    frequency[board[i][row] - 1] += 1;
  }
  
  row_check[row] = true;
  for(i=0; i<SIZE; i++){
    row_check[row] = false;
  }
}

// Checks the given col for duplicate numbers, and updates the col_check
// value for that col appropriately. If no number is repeated in that col,
// col_check[col] will be set to true; otherwise, it will be false.
void* checkCol(void* args) {
  int frequency[SIZE];
  int i;
  
  for(i=0, i<SIZE; i++){
    frequency[i] = 0;
  }
  int col = *((int *)args);
  
  for(i=0; i<SIZE; i++){
    frequency[board[col][i] - 1] += 1;
  }
  
  col_check[col] = true;
  
  for(i=0; i<SIZE; i++){
    if(frequency[i] != 1){
      col_check[col] = false;
    }
  }
}

// Checks the given 3x3 box for duplicate numbers, and updates the box_check
// value for that box appropriately. If no number is repeated in that box,
// box_check[box] will be set to true; otherwise, it will be false.
void* checkBox(void* args) {
  int frequency[SIZE];
  int i;
  
  for(i=0; i<SIZE; i++){
    frequency[i] = 0;
  }
  
  int box = *((int *)args);
  int j;
  int x = (box/3) * 3;
  int y = (box%3) * 3;
  
  for(int i=0; i<3; i++){
    for(j=0; j<3; j++){
      frequency[board[i+x][j+y] - 1] += 1;
    }
  }
  
  box_check[box] = true;
  
  for(i=0; i<SIZE; i++){
    if(frequency[i] != 1){
      box_check[box] = false;
    }
  }
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() { 
    // 1. Print the board.
    printf("Board:\n");
    printBoard(board);
  
    pthread_t row_threads[SIZE];
    pthread_t col_threads[SIZE];
    pthread_t box_threads[SIZE];
    int i;
  
    for(i=0; i<SIZE; i++){
      int *x = malloc(sizeof(int));
      int *y = malloc(sizeof(int));
      int *z = malloc(sizeof(int));
      
      *x = i;
      *y = i;
      *z = i;
      
      pthread_create(&row_threads[i], NULL, checkRow, x);
      pthread_create(&col_threads[i], NULL, checkCol, y);
      pthread_create(&box_threads[i], NULL, checkBox, z);
    }
    
    for(i=0; i<SIZE; i++){
      pthread_join(row_threads[i], NULL);
      pthread_join(col_threads[i], NULL);
      pthread_join(box_threads[i], NULL);
    }
    
    // 2. Create pthread_t objects for our threads.
    
    // 3. Create a thread for each cell of each matrix operation.
    
    // 4. Wait for all threads to finish.
    
    // 5. Print the results.
    printf("Results:\n");
    bool all_rows_passed = true;
    printf("Rows:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!row_check[i]) {
            printf("Row %i did not pass\n", i);
            all_rows_passed = false;
        }
    }
    if (all_rows_passed) {
        printf("All rows passed!\n");
    }
    
    bool all_cols_passed = true;
    printf("Cols:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!col_check[i]) {
            printf("Col %i did not pass\n", i);
            all_cols_passed = false;
        }
    }
    if (all_cols_passed) {
        printf("All cols passed!\n");
    }
    
    bool all_boxes_passed = true;
    printf("Boxes:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!box_check[i]) {
            printf("Box %i did not pass\n", i);
            all_boxes_passed = false;
        }
    }
    if (all_boxes_passed) {
        printf("All boxes passed!\n");
    }
    return 0;
}


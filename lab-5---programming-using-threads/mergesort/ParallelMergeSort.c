#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 100

//Collaborators: Nubia Johnson

int array[SIZE];

void fillArrayWithRandomNumbers(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) array[i] = rand()%100;
}

void printArray(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) printf("%5d", arr[i]);
    printf("\n");
}

typedef struct StartEndIndexes {
    int start;
    int end;
} StartEndIndexes;

// Runs mergesort on the array segment described in the
// argument. Spawns two threads to mergesort each half
// of the array segment, and then merges the results.

struct divider{
  int part_num;
  int part_low;
  int part_high;
}

//func merges two parts
void merge(int low, int mid, int high){
  int i, j;
  int leftP = (mid - low + 1);
  int rightP = (high-mid);
  int *left = malloc(leftP * sizeof(int));
  int *right = malloc(rightP * sizeof(int));
  
  for(i=0; i<leftP; i++){
    left[i] = array[i + low];
  }
  
  for(i = 0; i<rightP; i++){
    right[i] = array[i + mid + 1];
  }
  
  int l = low;
  
  i = 0;
  j = 0;
  
  while(i < leftP && j < rightP){
    if(left[i] <= right[j]){
      array[l++] = left[i++];
    }
    else{
      array[l++] = right[j++];
    }
  }
  
  while(i < leftP){
    array[l++] = left[i++];
  }
  
  while(j < rightP){
    array[l++] = right[j++];
  }
  
  free(left);
  free(right);
}

void merge_sort(int low, int high){
  int mid = low + (high - low)/2; //midpoint
  
  if(low < high){
    merge_sort(low, mid);
    merge_sort(mid + 1, high);
    merge(low, mid, high);
  }
}

void* mergeSort(void* args) {
  struct divider *divider - arg;
  int low;
  int high;
  int num;
  
  //setting low and high
  high = divider->part_high;
  low = divider->part_low;
  
  int mid = low + (high-low)/2; //midpoint
  
  if(low < high){
    merge_sort(low, mid);
    merge_sort(mid+1, high);
    merge(low, mid, high);
  }
  
  return 0;
}

int main() {
    srand(time(0));
    StartEndIndexes sei;
    sei.start = 0;
    sei.end = SIZE - 1;
    
    // 1. Fill array with random numbers.
    fillArrayWithRandomNumbers(array);
    
    // 2. Print the array.
    printf("Unsorted array: ");
    printArray(array);
    struct divider *divider;
  
    pthread_t threads[2];
    struct divider list[2];
  
    int length = SIZE/2;
    int low = 0;
    
    for(int i = 0; i<2; i++, low+=length){
      divider = &list[i];
      divider->part_num = i;
      
      divider->part_low = i * (SIZE/2);
      divider->part_high = (i+1) * (SIZE/2)-1;
    }
    
    // 3. Create a 2 threads for merge sort.
    for(int i = 0; i<2; i++){
      pthread_join(threads[i], NULL);
      merge(0, (SIZE/2-1) / 2, SIZE/2-1);
      merge(SIZE/2, SIZE/2 + (SIZE - 1 - SIZE/2)/2, SIZE-1);
      merge(0,(SIZE-1)/2, SIZE-1);
    }
    // 4. Wait for mergesort to finish.
    
    // 5. Print the sorted array.
    printf("Sorted array:   ");
    printArray(array);
    
    return 0;
}

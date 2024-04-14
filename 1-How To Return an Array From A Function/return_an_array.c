#include <stdio.h>
#include <stdlib.h>

int *set_array(int value){
    int array_local[5];
    for (int i = 0; i < 5; i++){
        array_local[i] = value;
    }
    
    return array_local;
}

void set_array1(int value, int *array){
    for (int i = 0; i < 5; i++){
        array[i] = value;
    }
}

int *set_array2(int value){

    int *array = malloc(sizeof(int) * 5); // Dynamically allocates space to store 5 int values on the heap.
    for (int i = 0; i < 5; i++){
        array[i] = value;
    }

    return array; // Returns the memory address of the first element in the heap allocated array.
}

int *set_array3(int value)
{
  static int array[5]; // 'array' is now a static variable. It lives as long as the program is running.

  for (int i = 0; i < 5; i++)
    array[i] = value;

  return array;
}

int main(){
    int array[] = {1, 2, 3, 4, 5};

    int *result = set_array3(4);

    for(int i = 0; i < 5; i++){
        printf("result[%d] = %d\n", i, result[i]);
    } 
    
    return 0;
}
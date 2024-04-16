#include <stdio.h>

int main() {
  int array[5];
  array[2] = 2;

  printf("array: %zu \n", array);
  printf("array + 1: %zu \n", array + 1); // + 1 adds 4 bytes to the value to 'array'


  printf("&array: %zu \n", &array);
  printf("&array + 1: %zu \n", &array + 1);


  int matrix [3][5] =
  {
      {0, 1, 2, 3, 4},
      {5, 6, 7, 8, 9},
      {10, 11, 12, 13, 14}
  };

  printf("&matrix[1]: %zu\n", &matrix[1]);
  printf("&matrix[1] + 1: %zu\n", &matrix[1] + 1);
  printf("*(&matrix[1] + 1): %zu\n\n", *(&matrix[1] + 1));

  // This line also works: "int *pointer = (&matrix[1] + 1)" because the compiler
  // figured what we wanted to do. But we get WARNING!
  int *pointer = (int*) (&matrix[1] + 1);

  printf("pointer: %zu\n", pointer);
  printf("*pointer: %d\n", *pointer);


  // Dereferencing twice would give us 10
  printf("*(*(&matrix[1] + 1)): %d\n\n", *(*(&matrix[1] + 1)));
}

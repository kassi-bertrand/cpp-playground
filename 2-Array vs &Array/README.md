# What is the difference between `array` and `&array`

In `C` we can declare an array of 5 `int`s like so:

```c
#include <stdio.h>

int main(){
    int array[5];
    return 0;
}
```

Like I learned in the previous note, using the `array` by itself will decay to a pointer. Meaning `array` can be treated like a pointer because it stores the address of the memory location containing the first element in the array `array`. And as I saw previously, one can print the address stored in the pointer like this:

```c
printf("array: %zu\n", array);
```

_NOTE: The `%zu` placeholder is used to display potentially large integer values. We could have also used the `%p` placeholder as well_.

> But what happens when we apply the `&` to array like so `&array`?

To answer the above question I added few lines to the program I have been writing:

```c
#include <stdio.h>

int main() {
  int array[5];
  array[2] = 2;

  printf("array: %zu \n", array);
  printf("&array: %zu \n", &array);
}
```

When compiled and ran, the above code returns the following result, on my machine:

```txt
array: 6127612612
&array: 6127612612
```

> The same result? Why? 🤔

This led me to beleive that `&array` is the _same as_ `array`. But there is a difference. `&array` holds the address of the **whole array itself** but since the array is stored as contiguous block of memory, the address of the entire array is the _same_ as the address of its first element.

> Hum...

Okay, let's try again. `array` tells me that I hold the address to a single `int` element which also happens to be the first element of an array we created. `&array` on the other hand, tells me explicitely that I hold the address to _consecutive blocks of memory_, more specifically that I am holding the _starting address of 5 consecutive blocks of `int`s in memory_. The distinction is subtle, but yet very important in my opinion.

> Why does it matter? Pointer arithmetic.

Let's try to do pointer arithmetic with both pointers and see how they behave. Let's use the following snippet to see the behavior in action:

```c

int main() {
  int array[5];
  array[2] = 2;

  printf("array: %zu \n", array);
  printf("array + 1: %zu \n", array + 1); // + 1 adds 4 bytes to 'array'

  printf("&array: %zu \n", &array);
  printf("&array + 1: %zu \n", &array + 1);
}
```

Compiling and running the program outputs the following result, on my machine:

```txt
    array:  6135263940
array + 1:  6135263944

    &array: 6135263940
&array + 1: 6135263960
```

Like the comment said, adding `+1` to `array` will add `4` bytes to the value that `array` is currently holding, and doing that addition causes `array + 1` to refer to the address of the SECOND element in the array.

> Hum... Why `4` bytes? Adding `+1` to the pointer increments it by _one time_ the byte size of what the pointer is pointing to. On my machine, an `int` occupies `4` bytes of memory. If you look the output of `array` and `array + 1` you should see a difference of `4` between both values.

Now, let's look at what happened with `&array`. It looks like the increment operation on `&array` added not `1`, but **`20` bytes**! The reason is because `&array` holds the address of the starting point of 5 consecutive `int` values. We know that an `int` value occupies 4 bytes in memory. So, when incrementing the pointer means adding 20 bytes!

And that's the big difference between `array` and `&array`. Another interesting situation where this matters is when working with 2D arrays. Let's see this in action with the following snippet:

```c

int main() {

  int matrix [3][5] =
  {
      {0, 1, 2, 3, 4},
      {5, 6, 7, 8, 9},
      {10, 11, 12, 13, 14}
  };

  printf("matrix[1]: %zu\n", matrix[1]); // address the first element in the SECOND array in the matrix
  printf("&matrix[1]: %zu\n", &matrix[1]); // pointer to the SECOND row (i.e. array) in the matrix
}
```

In the above code snippet, we are creating a 2D array, with 3 rows and 5 columns. Compiling and running the code gives us the following result, on my machine:

```txt
matrix[1]: 6165525148
&matrix[1]: 6165525148
```

The result why we get the same result is familiar at this point. `matrix[1]` holds the address of the first element in the SECOND row (i.e array) of the matrix.`&matrix[1]` holds the _starting_ address of a 5 consecutives blocks of `int`s in memory. In other words, `&matrix[1]` holds the address of the second array in the in the matrix.

> Let's see what happens if we increment `&matrix[1]`...

`&matrix[1]` is pointing to an array of `5` `int` values, which totals `20` bytes. So, incrementing `&matrix[1]` will add `20` bytes, but if we dereference the pointer after this operation we should see that the pointer points to the first element of the third array, which `10`. Let's see this action:

```c
int main() {

  int matrix [3][5] =
  {
      {0, 1, 2, 3, 4},
      {5, 6, 7, 8, 9},
      {10, 11, 12, 13, 14}
  };

  printf("&matrix[1]: %zu\n", &matrix[1]);
  printf("&matrix[1] + 1: %zu\n", &matrix[1] + 1);
  printf("*(&matrix[1] + 1): %zu\n\n", *(&matrix[1] + 1));
}
```

The above snippet, increments `&matrix[1]`, displays the new address, then dereferences the pointer to see what it's pointing to. Compiling and running this program returns the following output, on my machine:

```txt
&matrix[1]: 6162903708
&matrix[1] + 1: 6162903728
*(&matrix[1] + 1): 6162903728
```

> Huh?! I thought `*(&matrix[1] + 1)` would output  `10`...

I thought so too. But then I remembered... After incrementing ``&matrix[1]`` which type of pointer do we have right now? One that holds the address of the first element in the array? OR one that holds the starting address of a _block_ of memory? We are holding the starting address of a block of memory. As a matter of fact, the pointer is of type `int (*)[5]`, not `int *`. The `int (*) [5]` means the variable contains the starting address of a block of `5` consecutive `int`s in memory.

> So, how do we get `10` then?

Two ways...

(1) By 'casting' our `int(*)[5]` pointer to a `int*` pointer, _before_ dereferencing it. It looks like this:

```c
int main() {

  int matrix [3][5] =
  {
      {0, 1, 2, 3, 4},
      {5, 6, 7, 8, 9},
      {10, 11, 12, 13, 14}
  };

  printf("&matrix[1]: %zu\n", &matrix[1]);
  printf("&matrix[1] + 1: %zu\n", &matrix[1] + 1);
  printf("*(&matrix[1] + 1): %zu\n\n", *(&matrix[1] + 1));

  int *pointer = (int*) (&matrix[1] + 1); // (int*) casting happens here

  printf("pointer: %zu\n", pointer);
  printf("*pointer: %zu\n", *pointer);
}
```

Compiling and running the program gives the following output on my machine, which is in line with our expectations:

```txt
&matrix[1]: 6102954652
&matrix[1] + 1: 6102954672
*(&matrix[1] + 1): 6102954672

pointer: 6102954672
*pointer: 10
```

The reason we got the desired result is because by casting, we instructed the C compiler to interpret the address stored in `(&matrix[1] + 1)` as an `int*` instead of an `int (*)[5]`.

(2) The second way is a little less obvious. At least was to me...

When I ran this code:

```c
int main() {

  int matrix [3][5] =
  {
      {0, 1, 2, 3, 4},
      {5, 6, 7, 8, 9},
      {10, 11, 12, 13, 14}
  };

  printf("&matrix[1]: %zu\n", &matrix[1]);
  printf("&matrix[1] + 1: %zu\n", &matrix[1] + 1);
  printf("*(&matrix[1] + 1): %zu\n\n", *(&matrix[1] + 1));
}
```

I got the following output on my machine:

```txt
&matrix[1]: 6162903708
&matrix[1] + 1: 6162903728
*(&matrix[1] + 1): 6162903728
```
Dereferencing `(&matrix[1] + 1)` did not return the `10` immediately, instead it did return a `int*` which means that if we did dereference that a _second_ time, we'd be getting the `10` we were looking for. It would look like this:

```c
printf("*(*(&matrix[1] + 1)): %zu\n\n", *(*(&matrix[1] + 1)));
```

and the result on my machine is this:

```txt
*(*(&matrix[1] + 1)): 10
```

Initially `&matrix[1] + 1` is holding the starting address of a block of consecutive memory (i.e. the array). Dereferencing it once gives the actual array, but remember _arrays decay to pointers_. So, what we ended up getting was not the entire array, but rather the address of its first element! That's why dereferencing the first time returned a `int*` and that's why dereferencing the _second_ time returned the expected result: `10`.

# References 📚

1. [array vs &array Pointers Difference Explained | C Programming Tutorial](https://youtu.be/WL1P6xiA_KY?si=P-Z00q1F5LxIJOCr)

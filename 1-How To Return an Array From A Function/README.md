# How To Return an Array from a function in C

Let's start by creating a function called `my_function` to return an array of `int` values like so:

```c
int[] my_function(){

}

int main(){
    
    return 0;
}
```

Compiling this code using gcc returned the following error message for me:

```txt
main.c:3:20: error: brackets are not allowed here; to declare an array, place the brackets after the identifier
int[] my_function()
   ~~              ^
                   []
main.c:3:18: error: function cannot return array type 'int[]'
int[] my_function()
                 ^
2 errors generated.
```

Those errors are fairly clear! In C, a function _cannot_ an array all at once to me. 

> What can it return then?

Let's first remind ourselves the relationship between pointers and arrays with the following code:

```cpp
int main(){
    int array[] = {1, 2, 3, 4, 5};
    return 0;
}
```

The above creates an array of `int`s. The values are stored consequently in the memory of the computer. And I am already familiar with how to access an element in the array, and I am also aware of "pointer arithmetic".

Thus using the array identifier like this:

```c
array
```

We say `array` **decayed to a pointer**. In the above code snippet, `array` contains the memory address of _the first element_ of the array, `array`. Since the `array` variable stores a memory address, it _can_ be treated as a pointer to an `int`, and we can store a copy of its value (i.e. the address) in another pointer to an `int` like so:

```c
int main(){
    int array[] = {1, 2, 3, 4, 5};
    
    int *result = array; // 'array' is an 'int *'

    return 0;
}
```

Now, we _can_ use the `result` pointer 'points' to (that is contains) the memory location of the element in the array, `array`. So, we could use the `result` pointer to traverse the array just like we would using the `array` variable.

```c
int main(){
    int array[] = {1, 2, 3, 4, 5};

    int *result = array;

    int value = *result; // Dereferencing the 'result' pointer

    printf("Dereferencing the value pointer: %d\n", value);
    printf("Dereferencing the result pointer: %d\n", *array); // 'array' is tread like pointer

    return 0;
}
```

Dereferencing Both the `result` and the `array` pointers produce the same values, because they 'point' to the same location in memory, the element in the array `array`, which is `1`.

To prove the point made earlier, we can further a print the memory addresses that the `array` and `result` pointers are storing using the following snippet:

```c
int main(){
    int array[] = {1, 2, 3, 4, 5};

    int *result = array;

    printf("Address stored in 'array' pointer: %p\n", array);
    printf("Address stored in 'result' pointer: %p\n", result);

    return 0;
}
```

The above snippet prints the _same_ values. That is the `array` and `result` pointers store the address of the _same_ location in the computer's memory.

This is where the insight from returning arrays from functions in `C` lies: **Returning a pointer to the array**, and once returned, we can store the returned value into a variable, and access the array. Let's see it in action.

Let's consider the following scenario: Create the function `set_array` whose job is to set all the elements in the array to a particular value. `set_array` will return a pointer to an `int`, which in our case is the first element of the array. In code, the function will look like this:

```c
int *set_array(int value){
}
```
Let's to create a local array of `5` elements _inside_ the function called `array_local`, set all its values to `value` and return the 'decayed' pointer to this local array. 

```c
int *set_array(int value){
    int array_local[5];
    for (int i = 0; i < 5; i++){
        array_local[i] = value;
    }
    
    return array_local;
}
```
Based on what we learned so far, it looks like the above should work; it's returning a pointer to the first element like we learned. But let's see, let's call use function in `main`:

```c
int main(){
    int array[] = {1, 2, 3, 4, 5};

    int *result = set_array(4); // 'set_array' should return a pointer the first element of an array of 4s

    for(int i = 0; i < 5; i++){
        printf("result[%d] = %d\n", i, result[i]);
    } 

    return 0;
}
```
Compiling the program yields the following error message on my computer:
```c
main.c:9:12: warning: address of stack memory associated with local variable 'array_local' returned [-Wreturn-stack-address]
    return array_local;
           ^~~~~~~~~~~
1 warning generated.
```

And running the code results in the following output:
```
result[0] = 4
result[1] = -1630826680
result[2] = 173932545
result[3] = 4
result[4] = 4
```
This result was _not_ expected. It has incorrect values. The reason why we get this unexpected result is given away by the warning we got earlier. `array_local` as the name suggested is **local** to the `set_array` function. In other words, it has the scope of the `set_array` function. So, the '**lifetime**' of the variable is limited, scoped, tied, to the `set_array` function. So, when the `set_array` function returns the memory sections occupied by `local_array` on the stack (an area of memory for local/function variables) are released, meaning the array no longer exist! It implies that other programs can _claim_ those memory locations and set their own values there! Effectively overriding what was there before. The fact that we see some `4`s in the array is pure luck, and rerunning the program yields different results! 🥴

> How do you address that?

What people do, is returning a pointer to an array **passed to the function**. In other words, the function manipulates an array it _does not own_ and returns a pointer. Let's see it in action:

```c
void set_array1(int value, int *array){
    for (int i = 0; i < 5; i++){
        array[i] = value;
    }
}

int main(){
    int array[] = {1, 2, 3, 4, 5};

    set_array1(4, array); // 'array' when passed to the function decays (i.e. becomes) to a pointer

    for(int i = 0; i < 5; i++){
        printf("array[%d] = %d\n", i, array[i]);
    } 

    return 0;
}
```

It's the same `set_array` function, with a new name. This time it accepts an `int*` argument, in addition of the `int` argument. Also, the function returns `void` because it will manipulate the array passed directly. Notice that we no longer have a `array_local` inside the function, instead the array to be manipulated is defined in the `main` function. Running the above code gives the following result:

```
array[0] = 4
array[1] = 4
array[2] = 4
array[3] = 4
array[4] = 4
```

Excellent! 🚀

Now, technically it's possible to have variables declared _inside_ a function, it can be achieved by **dynamically allocate memory on the heap, and returns a pointer to that memory**. The only thing is, if this memory is properly released, or freed it remains occupied for the entire duration of the program. That's a memory leak! In `C`, memory is allocated on the heap using the `malloc` function. It is short for **m**emory **alloc**ation. Let's see this in action:

```c
int *set_array2(int value){

    int *array = malloc(sizeof(int) * 5); // Dynamically allocates space to store 5 int values on the heap.
    for (int i = 0; i < 5; i++){
        array[i] = value;
    }

    return array; // Returns the memory address of the first element in the heap allocated array.
}

int main(){
    int array[] = {1, 2, 3, 4, 5};

    int *result = set_array2(4);

    for(int i = 0; i < 5; i++){
        printf("result[%d] = %d\n", i, result[i]);
    } 

    free(result); // DO NOT FORGET - Free the blocks of memory 'result' is pointing to

    return 0;
}
```
The result:
```txt
result[0] = 4
result[1] = 4
result[2] = 4
result[3] = 4
result[4] = 4
```
I reverted back to our old function definition. `set_array2` accepts an `int` argument, and returns a `int*` just like `set_array`. `malloc` allocates memory on the heap, a different part of memory. The heap lets us allocate/deallocate blocks of memory at any time. It's very powerful, but with great power also comes great responsibility. Because we can allocate blocks of memory when we need it, it's just as much, if not more important to deallocate, or release (i.e. cleanup) the memory when done using it.

Another way to return a pointer to something created inside a function, is to create variables that remain accessible for the the entire program's execution. We call those variable `static`. So, using `static` variables let us to return a pointer to an array that has been declared _in a function_. Let's see it in action:

```c
int *set_array3(int value)
{
  static int array[5]; // 'array' is now a STATIC variable. It lives as long as the program is running.

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
```
Result:
```txt
result[0] = 4
result[1] = 4
result[2] = 4
result[3] = 4
result[4] = 4
```

Not only compiling, and running the program gives the expected result, but notice that we did not use the `free` function. Why? Because the array is destroyed when the program ends. Trying into 'free' `result`, the code will compile fine, but running it will get you this:

```
main.out(79712,0x206b03ac0) malloc: *** error for object 0x102d54000: pointer being freed was not allocated
main.out(79712,0x206b03ac0) malloc: *** set a breakpoint in malloc_error_break to debug
[1]    79712 abort      ./main.out
```

# References 📚

1. [How To Return An Array From A Function | C Programming Tutorial](https://youtu.be/uI1J8o1rY10)
2. [What and where are the stack and heap?](https://stackoverflow.com/questions/79923/what-and-where-are-the-stack-and-heap)
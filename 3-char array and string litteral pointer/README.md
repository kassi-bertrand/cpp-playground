# What is the difference between a char array and a pointer to a string literal

In C, a **string literal** refers to a pair of double quotes enclosing a series of one or multiple characters. The following is an example of a string literal:

```txt
"literal"
```
A string literals like the above, exist in some place of memory. Where exactly? We don't know, it's entirely dependent on the implementation. The reason why it's like this is because location of is not explicitely defined by the C standard.

In C, string literals can be used to initialize characters arrays like so:

```c
#include <stdio.h>

int main(){
    char s1[] = "literal";
    return 0;
}
```

The above code snippet creates a character array `s1` containing 8 `char`s (7 letters + null terminator). The array `s1` is stored on the stack, and because of its position in this region of memory, we can modify the slots in the array.

For instance, we can change the first character in `s1` like so:

```c
#include <stdio.h>

int main(){
    char s1[] = "literal";

    // Change the first character is s1
    s1[0] = 'X';

    // Print modified s1
    printf("s1: %s\n", s1);
    return 0;
}
```

Compiling and running the above snippet gives me the following result on my machine:

```c
s1: Xiteral
```

But things start changing when we declare `s2`:

```c
#include <stdio.h>

int main(){
    // ... old code ...

    char *s2 = "another litteral";
}
```
`s2` is not a character array. It's a pointer to the first character of the specified string literal. Unlike `s1`, `s2` does not live on the stack but rather a location not defined by the C standard which makes it entirely dependent on whoever is implementing the compiler I am currently using. Since `s2` does not live on the stack, but in a "secret" part of memory, attempting any form of modification results in a segmention fault. Another way to put this is that `s2` points to read-only part of memory. So, pretty much of all we can is print what `s2` is pointing to:

```c
#include <stdio.h>

int main(){
    // ... old code ...

    // s2 points to a read-only location of memory.
    // Trying to change the content this location results in segmentation fault.
    char *s2 = "another litteral";
    printf("s2: %s\n", s2);

}
```
We know what to expect from compiling and running the above snippet. But trying we the following:

```c
#include <stdio.h>

int main(){
    // ... old code ...

    // s2 points to a read-only location of memory.
    // Trying to change the content this location results in segmentation fault.
    char *s2 = "litteral";
    printf("s2: %s\n", s2);

    // Attempting to modifying a restricted region results in a seg. fault
    s2[0] = 'X';
    return 0;
}
```
gives the following as result on my M2 Mac:
```txt
s1: Xiteral
[1]    86332 bus error  ./main.out
```

We know `s2` points to the location of the first character in `literal` which itself lives in read-only part of the memory. But since `s2` is not a constant pointer, we can change its value by doing pointer arithmetic:

```c
#include <stdio.h>

int main(){
    // ... old code ...

    s2++;
    printf("s2: %s\n", s2);
    return 0;
}
```
and compiling and running the program on my machine gives me this:

```txt
s1: Xiteral
s2: iteral
```
When printing `s2`, the first character is skipped because we passed the a pointer to the second character to `printf` which starts printing from there until it meets a null terminated character.

> Okay, things are going pretty smooth right now. But you said `s2` is a pointer. `s1` is an array, which when passed to a function decays to pointer. Does it mean we can do pointer arithmetic with `s1` just like we did with `s2`?

Well, let's find out...

```c
#include <stdio.h>

int main(){
    // ... old code ...

    // Can you do pointer arithmetic with s1?
    s1++;
    return 0;
}
```
Compiling and running the above on my computer gives the following error:

```c
char_array_vs_string_literal_pointer.c:21:7: error: cannot increment value of type 'char[8]'
    s1++;
    ~~^
1 error generated.
```

The reason was hinted a little bit earlier. `s1` _is_ a character array to begin with. Not a pointer. It decays to a pointer when it's passed around; as an argument to a function for instance. Another way of looking at it is that `s2` was referred to as a non-constant pointer, and that's why we were able to change its value (by incrementing it), but `s1` however is a **constant pointer**, meaning we can't change what it's pointing to, we can't move `s1` around like we did with `s2`. That's the first difference between `s1` and `s2`.

The second difference between `s1` and `s2` is that we can't just re-assign `s1` to a new value, but can for `s2`. Let's it in action with the following snippet:

```c
#include <stdio.h>

int main(){
    // ... old code ...

    // s1 CANNOT just be re-assigned to a new value like this below
    // Instead, we'd need a function like strcopy() from string.h to change the chars in s1.
    s1 = "new string";

    // s2 however CAN be re-assigned a new value like this:
    s2 = "new string";
    return 0;
}
```
Compiling and running the snippet above gives the following output on my machine:

```txt
char_array_vs_string_literal_pointer.c:25:8: error: array type 'char[8]' is not assignable
    s1 = "new string";
    ~~ ^
1 error generated.
```
The reason why can't just re-assign `s1` like did with `s2` has to do with the nature (i.e. type) of both variables.

**NOTE**: Notice that I use the word "re-assign". I say that we can't re-assign `s1` because I assume `s1` has been initialized before. It implies that the following code is valid:

```c
char s[];
s = "string";
```

Another interesting difference between `s1` and `s2` is their size in memory. Let's see it in action with the following snippet:

```c

#include <stdio.h>

int main(){
    // ... old code ...

    // s1 and s2 have difference sizes in memory
    printf("size of s1: %lu\n", sizeof(s1));
    printf("size of s2: %lu\n", sizeof(s2));
    return 0;
}
```
Compiling and running the above of my machine gives me the following output:

```txt
s1: Xiteral
s2: iteral
s2: new literal
size of s1: 8
size of s2: 8
```
> You said they had different sizes right? But the print statement returned `8` for both `s1` and `s2`.

Short answer: It's a coincidence.

We got `8` bytes for `s1` because `s1` stores 7 characters plus a null character. Since `char`s just one byte of memory, we get a total of `8` bytes for the entire array `s1`. The same cannot be said for `s2`, though. `s2` is just a pointer variable living on the stack. It occupies `8` bytes of memory because it contains the address of the memory location containing the first `char` in `new literal`.

**NOTE**: Since I am running on a 64-bit system, memory addresses are represented using 64 bits (i.e. 8 bytes). If I was running on a 32-bit machine, addresses would be represented using 4 bytes.

# References
- [String In Char Array VS. Pointer To String Literal | C Programming Tutorial](https://youtu.be/Qp3WatLL_Hc?si=0AgPODktqP1zX1md)

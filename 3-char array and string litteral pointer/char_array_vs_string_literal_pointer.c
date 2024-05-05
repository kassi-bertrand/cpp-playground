#include <stdio.h>

int main(){
    char s1[] = "literal";

    // Change the first character is s1
    s1[0] = 'X';

    // Print modified s1
    printf("s1: %s\n", s1);

    // s2 points to a read-only location of memory.
    // Trying to change the content this location results in segmentation fault.
    char *s2 = "literal";
    // s2[0] = 'X';

    s2++;
    printf("s2: %s\n", s2);

    // Can you do pointer arithmetic with s1? NO
    // s1++;

    // s1 CANNOT just be re-assigned to a new value like this below
    // Instead, we'd need a function like strcopy() from string.h to change the chars in s1.
    // s1 = "new string";

    // s2 however CAN be re-assigned a new value like this:
    s2 = "new literal";
    printf("s2: %s\n", s2);

    // s1 and s2 have difference sizes in memory
    printf("size of s1: %lu\n", sizeof(s1));
    printf("size of s2: %lu\n", sizeof(s2));
    return 0;
}

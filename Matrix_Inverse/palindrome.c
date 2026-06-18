//Check the give word is palindrome or not
#include <stdio.h>
#include <string.h>
int main() {
    char str[100], rev[100];
    int i, j, len = 0;
    printf("Enter a string: ");
    scanf("%s", str);       
    // Calculate the length of the string
    for (i = 0; str[i] != '\0'; i++) {
        len++;
    }
    // Reverse the string
    for (i = len - 1, j = 0; i >= 0; i--, j++) {
        rev[j] = str[i];
    }
    rev[j] = '\0';
    // Check if the original string and reversed string are the same
    if (strcmp(str, rev) == 0) {
        printf("The string is a palindrome.\n");
    } else {
        printf("The string is not a palindrome.\n");
    }
    return 0;
}
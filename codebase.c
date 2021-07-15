#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_DIG 32

// fast INput-OUTput
void ingore_garbage_int();
void ingore_garbage_char();
int  expect_pos_int();
char expect_char();

int main() {

    return 0;
}

// fast INput-OUTput

void ingore_garbage_int() {
    char ch;

    while(!isdigit(ch = getchar())) {}

    ungetc(ch, stdin);
}

void ingore_garbage_char() {
    char ch;

    while(!isalpha(ch = getchar())) {}

    ungetc(ch, stdin);
}

int expect_pos_int() {
    char digits[MAX_DIG], ch;
    int i, num, pow10;

    ingore_garbage_int();
    
    i = 0;
    while(isdigit(ch = getchar()) && i < MAX_DIG) {
        digits[i] = ch;
        i++;
    }
    ungetc(ch, stdin);

    if(i == 0) {
        return -1;
    }

    num = 0;
    pow10 = 1;
    i--;

    for(; i >= 0 ; i-- , pow10 *= 10) {
        num += ((digits[i] - '0') * pow10);
    }

    return num;
}

char expect_char() {
    ingore_garbage_char();

    return getchar();
}
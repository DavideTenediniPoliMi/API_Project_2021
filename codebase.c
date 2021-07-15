#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#ifdef _WIN32
# define gc _getchar_nolock
# define pc _putchar_nolock
#endif

#ifdef linux
# define gc getchar_unlocked
# define pc putchar_unlocked
#endif

// fast Input-Output
void ingore_garbage_int();
void ingore_garbage_char();
int  expect_pos_int();
char expect_char();

int main() {
    int a,b;

    a = expect_pos_int();
    b = expect_pos_int();
    
    printf("%d", a+b);
        
    return 0;
}

// fast INput-OUTput

void ingore_garbage_int() {
    char ch;

    while(!isdigit(ch = gc())) {}

    ungetc(ch, stdin);
}

void ingore_garbage_char() {
    char ch;

    while(!isalpha(ch = gc())) {}

    ungetc(ch, stdin);
}

int expect_pos_int() {
    char ch;
    int num;

    ingore_garbage_int();

    num = 0;
    while(isdigit(ch = gc())) {
        num = num * 10 + (ch - '0');
    }
    ungetc(ch, stdin);

    return num;
}

char expect_char() {
    ingore_garbage_char();

    return gc();
}
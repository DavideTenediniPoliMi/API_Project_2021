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
char ingore_garbage_int();
char ingore_garbage_char();
int  expect_pos_int();
int expect_char();

int main() {
    int a,b;

    a = expect_pos_int();
    b = expect_pos_int();
    
    printf("%d", a+b);
        
    return 0;
}

// fast INput-OUTput

char ingore_garbage_int() {
    char ch;

    do {
        ch = gc();
    } while (ch != EOF && !isdigit(ch));
    
    return ch;
}

char ingore_garbage_char() {
    char ch;

    do {
        ch = gc();
    } while(ch != EOF && !isalpha(ch));

    return ch;
}

int expect_pos_int() {
    char ch;
    int num;

    ch = ingore_garbage_int();

    num = 0;
    do {
        num = num * 10 + (ch - '0');
        ch = gc();
    } while(isdigit(ch));
    ungetc(ch, stdin);

    return num;
}

int expect_char() {
    char ch;

    ch = ingore_garbage_char();

    return (ch == EOF) ? -1 : ch;
}
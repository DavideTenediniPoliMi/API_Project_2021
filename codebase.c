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

// fast IO
int expect_pos_int() {
    char ch;
    int num;

    do {
        ch = gc();
    } while (ch != EOF && !isdigit(ch));

    if(ch == EOF) {
        return -1;
    }

    num = 0;
    do {
        num = num * 10 + (ch - '0');
        ch = gc();
    } while(isdigit(ch));

    return num;
}

int expect_char() {
    char ch;

    do {
        ch = gc();
    } while(ch != EOF && !isalpha(ch));

    return (ch == EOF) ? -1 : ch;
}

int main() {
    int a,b;

    a = expect_pos_int();
    b = expect_pos_int();
    
    printf("%d", a+b);
        
    return 0;
}
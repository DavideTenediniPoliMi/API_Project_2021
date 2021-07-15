#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

#ifdef _WIN32
# define gc _getchar_nolock
# define pc _putchar_nolock
#endif

#ifdef linux
# define gc getchar_unlocked
# define pc putchar_unlocked
#endif

// Fast IO
int expect_int() {
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

void print_pos_int(int num) {
    int trail0, reversed;

    if(num == 0) {
        pc('0');
        return;
    }

    trail0 = 0;
    while(num % 10 == 0) {
        trail0 ++;
        num /= 10;
    }

    reversed = 0;
    while(num >= 10) {
        reversed = (reversed * 10) + (num % 10);
        num /= 10;
    }

    if(num != 0) {
        pc(num + '0');
    }
        
    while(reversed != 0) {
        pc((reversed % 10) + '0');
        reversed /= 10;
    }

    while(trail0 > 0) {
        pc('0');
        trail0 --;
    }
}

// Input Parsing
void expect_graph(int N);

// Program Flow
void add_graph(int N) {
    //dynamic allocation of matrix NxN

    expect_graph(N);

    //calculate score

    //update topk if necessary
}

// Output Formatting
void print_topK();

int main() {
    int N, K;
    char ch;

    N = expect_int();
    K = expect_int();

    while((ch = expect_char()) != -1) { 
        if(ch == 'A') {
            add_graph(N);
        } else {
            print_topK();

            while(ch != -1 || ch != '\n') {
                ch = expect_char();
            }
        }
    }

    return 0;
}
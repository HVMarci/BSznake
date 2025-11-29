#include "helper.h"
#include <stdlib.h>
#include <time.h>

#include "debugmalloc.h"

int stoi(char const *str, int def) {
    int val = 0;
    bool neg = false;

    if (str[0] == '-') {
        neg = true;
    }

    for (char const *ptr = neg ? str+1 : str; *ptr != '\0'; ptr++) {
        if (*ptr < '0' || *ptr > '9') return def; // invalid string

        val *= 10;
        val += *ptr - '0';
    }

    return neg ? -val : val;
}

// [min, max] inclusive
int randint(int min, int max) {
    long double r = (long double) rand() / (long double) RAND_MAX;
    int range = max - min + 1;
    r = (r * range) + min;

    return (int) r;
}

long long randll(long long min, long long max) {
    long double r = (long double) rand() / (long double) RAND_MAX;
    long long range = max - min + 1;
    r = (r * range) + min;

    return (long long) r;
}

bool double_eq(double a, double b) {
    if (a > b) return (a - b) < 1e-6;
    else return (b - a) < 1e-6;
}

int int_cmp(const void *pa, const void *pb) {
    int a = *(const int *) pa;
    int b = *(const int *) pb;
    if (a < b) return -1;
    if (a == b) return 0;
    return 1;
}

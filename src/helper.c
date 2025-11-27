#include "helper.h"
#include <stdlib.h>
#include <time.h>

#include "debugmalloc.h"

int stoi(char const *str, int def) {
    int val = 0;

    for (char const *ptr = str; *ptr != '\0'; ptr++) {
        if (*ptr < '0' || *ptr > '9') return def; // invalid string

        val *= 10;
        val += *ptr - '0';
    }

    return val;
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

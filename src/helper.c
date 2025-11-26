#include "helper.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

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
    double r = (double) rand() / (double) RAND_MAX;
    int range = max - min + 1;
    r = (r * range) + min;

    return (int) r;
}

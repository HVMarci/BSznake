#include "helper.h"

#include "debugmalloc.h"

int stoi(char const *str, int def) {
    int val = 0;

    for (char *ptr = str; *ptr != '\0'; ptr++) {
        if (*ptr < '0' || *ptr > '9') return def; // invalid string

        val *= 10;
        val += *ptr - '0';
    }

    return val;
}

#include "bsz.h"

/**
 * @brief Megmondja `a` és `b` legnagyobb közös osztóját.
 * 
 * @return `(a,b)`
 */
int lnko(int a, int b) {
    int r = a % b;
    while (r > 0) {
        a = b;
        b = r;
        r = a % b;
    }

    return b;
}

/**
 * @brief Megadja az `ax === b (mod m)` kongruenciarendszer legkisebb megoldását.
 * 
 * @return `x` értéke (`0 <= x < m/(a,m)`)
 */
int kongruencia(int a, int b, int m) {
    int d = lnko(a, m);
    if (b % d != 0) return -1; // nincs megoldás

    m /= d;
    b /= d;
    a /= d;
    int a0 = m, b0 = 0;

    while (a > 0) {
        int hanyszor = a0 / a;
        int uja = a0 - hanyszor * a;
        int ujb = (b0 - hanyszor * b) % m;
        if (ujb < 0) ujb += m;
        
        //printf("%dx === %d (%d)\n", a, b, m);

        a0 = a, b0 = b;
        a = uja, b = ujb;
    }

    return b0;
}

/**
 * @brief Megmondja `p`-ről, hogy prím-e
 */
bool prime(int p) {

}

/**
 * @brief Kiszámolja egy nxn-es mátrix determinánsát.
 * 
 * @param mx A mátrix
 * @param n A mátrix mérete
 */
int det(int **mx, int n) {

}

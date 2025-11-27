#include "bsz.h"

#include "helper.h"
#include "debugmalloc.h"

/**
 * @brief Megmondja `a` és `b` legnagyobb közös osztóját.
 * 
 * @return `(a,b)`
 */
long long lnko(long long a, long long b) {
    long long r = a % b;
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
long long kongruencia(long long a, long long b, long long m) {
    long long d = lnko(a, m);
    if (b % d != 0) return -1; // nincs megoldás

    m /= d;
    b /= d;
    a /= d;
    long long a0 = m, b0 = 0;

    while (a > 0) {
        long long hanyszor = a0 / a;
        long long uja = a0 - hanyszor * a;
        long long ujb = (b0 - hanyszor * b) % m;
        if (ujb < 0) ujb += m;
        
        //printf("%lldx === %lld (%lld)\n", a, b, m);

        a0 = a, b0 = b;
        a = uja, b = ujb;
    }

    return b0;
}

/**
 * @brief Kiszámítja `a^n (mod m)` értékét.
 */
long long modexp(long long a, long long n, long long m) {
    if (n == 0) return 1;
    long long v = modexp(a, n/2, m);
    return (((v * v) % m) * (n%2 == 0 ? 1 : a)) % m;
}

/**
 * @brief Megmondja `p`-ről, hogy prím-e
 * 
 * Miller-Rabin teszt, forrás: https://cs.bme.hu/bsz1/jegyzet/bsz1_jegyzet.pdf
 */
bool prime(long long p) {
    // Miller-Rabin teszt
    if (p <= 1 || p % 2 == 0) return false;

    long long c = p - 1;
    int t = 0;
    while (c % 2 == 0) c /= 2, t++;

    //printf("p = %lld\n", p);
    //printf("c = %lld; t = %d\n", c, t);
    for (int k = 0; k < 100; k++) {
        long long a = randll(1, p - 1);
        long long d = lnko(a, p);
        //printf("a = %lld, (a,p) = %lld\n", a, d);

        if (d != 1) return false;

        //if (modexp(a, p - 1) != 1) return false;
        else {
            long long a_c = modexp(a, c, p);

            bool nemprim = a_c != p-1 && a_c != 1;
            //printf("a^c mod p = %lld\n", a_c);
            for (int i = 1; nemprim && i < t; i++) {
                a_c = (a_c * a_c) % p;
                //printf("a^(2^%d)c = %lld\n", i, a_c);
                if (a_c == p - 1) {
                    nemprim = false;
                }
            }

            if (nemprim) return false;
        }
    }

    return true;
}

bool double_eq(double a, double b) {
    if (a > b) return (a - b) < 1e-6;
    else return (b - a) < 1e-6;
}

/**
 * @brief Kiszámolja egy nxn-es mátrix determinánsát.
 * 
 * Gauss-elimináció, forrás: https://cs.bme.hu/bsz1/jegyzet/bsz1_jegyzet.pdf, 106. o.
 * 
 * @param mx A mátrix (ez futás közben módosul)
 * @param n A mátrix mérete
 */
double det(double **mx, int n) {
    double D = 1;
    for (int i = 0; i < n; i++) {
        if (!double_eq(mx[i][i], 0.)) {
            D *= mx[i][i];
            for (int k = i+1; k < n; k++) mx[i][k] = mx[i][k] / mx[i][i];
            mx[i][i] = 1.;

            for (int t = i + 1; t < n; t++) {
                if (double_eq(mx[t][i], 0.)) mx[t][i] = 0.;
                for (int k = i+1; k < n; k++) mx[t][k] -= mx[t][i] * mx[i][k];
                mx[t][i] = 0.;
            }
        } else {
            int cserei = -1;
            for (int t = i + 1; t < n; t++) {
                if (!double_eq(mx[t][i], 0.)) cserei = t;
            }
            if (cserei == -1) return 0.;
            for (int k = i; k < n; k++) {
                double tmp = mx[i][k];
                mx[i][k] = mx[cserei][k];
                mx[cserei][k] = tmp;
            }
            i--;
            D *= -1.;
        }
    }
    return D;
}

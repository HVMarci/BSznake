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
            for (int k = i + 1; k < n; k++) mx[i][k] = mx[i][k] / mx[i][i];
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

int randomprim(bool legyenprim, int min, int max) {
    int p = randint(min, max);
    while (prime(p) != legyenprim) p = randint(min, max);
    return p;
}

BSzFeladat feladat_generalas() {
    BSzFeladat feladat;
    feladat.type = randint(0, 3);
    feladat.mx = NULL;

    switch (feladat.type) {
        case LNKO: {
            int lnko = randint(1, 30);
            feladat.a = randint(1, 30) * lnko;
            feladat.b = randint(1, 30) * lnko;
            break;
        }
        case KONGRUENCIA: {
            feladat.c = randint(20, 500);
            feladat.a = randint(1, feladat.c);
            feladat.b = randint(1, feladat.c);
            break;
        }
        case PRIME: {
            bool legyenprim = randint(0, 1);
            feladat.a = randomprim(legyenprim, 1000, 10000);
            // a páros és az öttel osztható számok túl könnyűek
            while (feladat.a % 2 == 0 || feladat.a % 5 == 0) feladat.a += 1;
            break;
        }
        case DETERMINANS: {
            // a determináns értéke mindig egész lesz
            int n = randint(3, 4);
            int det = 1;
            feladat.a = n;
            feladat.mx = (double **) malloc(n * sizeof(double*));
            for (int i = 0; i < n; i++) {
                feladat.mx[i] = (double *) malloc(n * sizeof(double));
                for (int j = 0; j < i; j++) feladat.mx[i][j] = 0.;
                feladat.mx[i][i] = 1.;
                for (int j = i + 1; j < n; j++) feladat.mx[i][j] = (double) (randint(0, 10) - 5);
            }

            for (int i = 0; i < n-1; i++) {
                for (int k = i + 1; k < n; k++) {
                    double hanyszor = (double) (randint(0, 10) - 5);
                    for (int j = 0; j < n; j++) {
                        feladat.mx[k][j] += feladat.mx[i][j] * hanyszor;
                    }
                }
            }
            for (int i = 0; i < n; i++) {
                double hanyszor = (double) randint(1, 5);
                for (int j = 0; j < n; j++) feladat.mx[i][j] *= hanyszor;
                det *= hanyszor;
            }
            break;
        }
    }

    return feladat;
}

int get_answer(BSzFeladat feladat) {
    switch (feladat.type) {
        case LNKO:
            return lnko(feladat.a, feladat.b);
        case KONGRUENCIA:
            return kongruencia(feladat.a, feladat.b, feladat.c);
        case PRIME:
            return prime(feladat.a);
        case DETERMINANS:
            return det(feladat.mx, feladat.a);
    }

    return -1;
}

void feladat_free(BSzFeladat feladat) {
    if (feladat.mx == NULL) return;

    for (int i = 0; i < feladat.a; i++) free(feladat.mx[i]);
    free(feladat.mx);
    feladat.mx = NULL;
}

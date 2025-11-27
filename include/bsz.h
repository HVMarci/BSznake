/** @file bsz.h
 * @brief Ebben a fájlban vannak a függvények a matekfeladatokhoz.
 */

#ifndef _BSZ_H
#define _BSZ_H

#include <stdbool.h>

/**
 * @brief Megmondja `a` és `b` legnagyobb közös osztóját.
 * 
 * @return `(a,b)`
 */
long long lnko(long long a, long long b);

/**
 * @brief Megadja az `ax === b (mod m)` kongruenciarendszer legkisebb megoldását.
 * 
 * @return `x` értéke (`0 <= x < m`)
 */
long long kongruencia(long long a, long long b, long long m);

/**
 * @brief Kiszámítja `a^n (mod m)` értékét.
 */
long long modexp(long long a, long long n, long long m);

/**
 * @brief Megmondja `p`-ről, hogy prím-e.
 */
bool prime(long long p);

/**
 * @brief Kiszámolja egy nxn-es mátrix determinánsát.
 * 
 * @param mx A mátrix
 * @param n A mátrix mérete
 */
double det(double **mx, int n);

#endif /* _BSZ_H */

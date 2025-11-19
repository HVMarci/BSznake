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
int lnko(int a, int b);

/**
 * @brief Megadja az `ax === b (mod m)` kongruenciarendszer legkisebb megoldását.
 * 
 * @return `x` értéke (`0 <= x < m`)
 */
int kongruencia(int a, int b, int m);

/**
 * @brief Megmondja `p`-ről, hogy prím-e.
 */
bool prime(int p);

/**
 * @brief Kiszámolja egy nxn-es mátrix determinánsát.
 * 
 * @param mx A mátrix
 * @param n A mátrix mérete
 */
int det(int **mx, int n);

#endif /* _BSZ_H */

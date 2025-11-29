/** @file bsz.h
 * @brief Ebben a fájlban vannak a függvények a matekfeladatokhoz.
 */

#ifndef _BSZ_H
#define _BSZ_H

#include <stdbool.h>

/**
 * @brief Ilyen feladatokat lehet feladni játék után a játékosnak pluszpontért.
 */
typedef enum BSzFeladatTipus {
    LNKO = 0, KONGRUENCIA = 1, PRIME = 2, DETERMINANS = 3
} BSzFeladatTipus;

/**
 * @brief Egy feladatot lehet így eltárolni.
 * 
 * `a`, `b`, `c` - a feladat paraméterei olyan sorrendben, amilyenben a megoldó függvény is kéri.
 * `mx` - determináns számításánál a mátrixra mutat.
 */
typedef struct BSzFeladat {
    BSzFeladatTipus type;
    int a, b, c;
    double **mx;
} BSzFeladat;

/**
 * @brief Megmondja `a` és `b` legnagyobb közös osztóját.
 * 
 * @return `(a,b)`
 */
long long lnko(long long a, long long b);

/**
 * @brief Megadja az `ax === b (mod m)` kongruenciarendszer legkisebb megoldását.
 * 
 * @return `x` legkisebb pozitív értéke (`0 <= x < m`)
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
 * 
 * @return A mátrix determinánsa
 */
double det(double **mx, int n);

/**
 * @brief Prím/összetett szám generálása.
 * 
 * @param legyenprim `true` - prímet, `false` - összetett számot generál
 * @param min A lehető legkisebb érték
 * @param max A lehető legnagyobb érték
 * 
 * @return A generált szám
 */
int randomprim(bool legyenprim, int min, int max);

/**
 * @brief Generál egy random feladványt.
 * 
 * @return A generált feladvány. Fel kell `feladat_free`-vel szabadítani a benne dinamikusan foglalt mátrix miatt!
 */
BSzFeladat feladat_generalas();

/**
 * @brief Megold egy feladatot.
 */
int get_answer(BSzFeladat feladat);

/**
 * @brief Felszabadítja a megadott `BSzFeladat` struct `mx` paraméterét
 * 
 * @param feladat A felszabadítandó struct
 */
void feladat_free(BSzFeladat feladat);

#endif // _BSZ_H

/** @file helper.h
 * @brief Bárhol használható segédfüggvények.
 */

#ifndef _HELPER_H
#define _HELPER_H

#include <stdbool.h>

/**
 * @brief char const * -> int
 * 
 * Ha a sztring tartalmaz `[^0-9]` karaktert, akkor hibára fut, és `def` értékét adja vissza.
 * 
 * @param str A bemeneti sztring
 * @param def Alapértelmezett érték
 * 
 * @return A sztring számmá alakítva, vagy hiba esetén `def` értéke
 */
int stoi(char const *str, int def);

/**
 * @brief Random `int` generálása.
 * 
 * Random számot generál [min,max] tartományban. A `rand()` függvényt használja, így `srand()`-ot be kell állítani.
 * Egyenlő eséllyel generálja bármelyik számot a tartományban.
 * 
 * @param min Az intervallum kezdete
 * @param max Az intervallum vége
 * 
 * @return Random szám
 */
int randint(int min, int max);

/**
 * @brief Random `long long` generálása.
 * 
 * Random számot generál [min,max] tartományban. A `rand()` függvényt használja, így `srand()`-ot be kell állítani.
 * Egyenlő eséllyel generálja bármelyik számot a tartományban. (Igaz, nem teljesen tökéletes...)
 * 
 * @param min Az intervallum kezdete
 * @param max Az intervallum vége
 * 
 * @return Random szám
 */
long long randll(long long min, long long max);

/**
 * @brief Double értékeket hasonlít össze.
 * 
 * @return `|a - b| < 1e-6`
 */
bool double_eq(double a, double b);

/**
 * @brief `qsort`-hoz összehasonlító függvény `int`-ekre
 */
int int_cmp(const void *pa, const void *pb);

#endif // _HELPER_H

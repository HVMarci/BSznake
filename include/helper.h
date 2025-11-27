/** @file helper.h
 * @brief Bárhol használható segédfüggvények.
 */

#ifndef _HELPER_H
#define _HELPER_H

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
 * @brief Random szám generálása.
 * 
 * Random számot generál [min,max] tartományban. A `rand()` függvényt használja, így `srand()`-ot be kell állítani.
 * Egyenlő eséllyel generálja bármelyik számot a tartományban.
 * 
 * @param min Az intervallum kezdete
 * @param max Az intervallum vége
 * 
 * @return Random szám
 */
// random number [min,max] inclusive, srand() must be set first
int randint(int min, int max);
long long randll(long long min, long long max);
#endif // _HELPER_H

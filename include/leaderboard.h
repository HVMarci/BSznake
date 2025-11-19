/** @file leaderboard.h
 * @brief A ranglistát kezelő függvények.
 * 
 * A ranglistát tartamazó fájl formátuma a következő:
 * ```
 * BSZNAKE_RANGLISTAv1
 * név1
 * pontszám1
 * név2
 * pontszám2
 * név3
 * pontszám3
 * ...
 * ```
 * 
 * Minden név maximum 30 karakter hosszú lehet. A pontszámoknak csökkenő sorrendben kell szerepelni.
 */

#ifndef _LEADERBOARD_H
#define _LEADERBOARD_H

/**
 * @brief A ranglista elemeit tároló struct.
 */
typedef struct Result {
    char name[30+1]; /**< A játékos neve (max 50 karakter) */
    int score; /**< A játékos által elért pontszám */
    struct Result *next; /**< A láncolt lista következő eleme */
} Result;

/**
 * @brief A ranglistát tároló struct.
 */
typedef struct Leaderboard {
    char filename[50+1]; /**< Az eredményeket tároló fájl / a létrehozandó fájl neve (max 50 karakter) */
    Result *results; /**< Az eredmények láncolt listában, növekvő sorrendben */
} Leaderboard;

/**
 * @brief Megnyitja a ranglistát tartalmazó fájlt és betölti egy dinamikusan foglalt structba. Ha a fájl nem létezik, üres a struct.
 * 
 * A visszatérési értéket `close_leaderboard`-dal kell felszabadítani!
 * 
 * @param filename A fájlnév
 */
Leaderboard *open_leaderboard(const char *filename);

/**
 * @brief Hozzáfűz egy új eredményt a már meglévőekhez, meghagyva a növekvő sorendet.
 * 
 * @param lb Mutató a `Leaderboard` structra
 * @param name A játékos neve
 * @param score A játékos pontszáma
 */
void add_score(Leaderboard *lb, const char *name, int score);

/**
 * @brief Kimenti a ranglistát az `open_leaderboard`-ban meghatározott fájlba. Ha nem létezik, létrehozza.
 * 
 * @param lb Mutató a `Leaderboard` structra
 */
void save_leaderboard(Leaderboard *lb);

/**
 * @brief Megszüntet a `Leaderboard` structhoz kapcsolódó minden memóriafoglalást.
 * 
 * @param lb Mutató a `Leaderboard` structra
 */
void close_leaderboard(Leaderboard *lb);

#endif /* _LEADERBOARD_H */

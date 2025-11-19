#include "leaderboard.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debugmalloc.h"

Leaderboard *open_leaderboard(const char *filename) {
    Leaderboard *lb = (Leaderboard *) malloc(sizeof(Leaderboard));

    strcpy(lb->filename, filename);
    lb->results = NULL;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return lb;
    } else {
        char name[30+1];
        int score;

        // BSZNAKE_RANGLISTAv1 ellenőrzése
        fscanf(fp, "%30s", name);
        if (strcmp(name, "BSZNAKE_RANGLISTAv1") != 0) {
            // rossz a fájl
            fclose(fp);
            return lb;
        }

        Result **curr = &lb->results;
        while (fscanf(fp, " %30[^\r\n] %d", name, &score) == 2) {
            Result *uj = (Result *) malloc(sizeof(Result));
            strcpy(uj->name, name);
            uj->score = score;
            uj->next = NULL;

            *curr = uj;
            curr = &uj->next;
        }

        fclose(fp);
        return lb;
    }
}

void add_score(Leaderboard *lb, const char *name, int score) {
    Result *uj = (Result *) malloc(sizeof(Result));
    strcpy(uj->name, name);
    uj->score = score;

    Result *mozgo = lb->results, **lemarado = &lb->results;
    while (mozgo != NULL && mozgo->score >= uj->score) {
        lemarado = &mozgo->next;
        mozgo = mozgo->next;
    }

    *lemarado = uj;
    uj->next = mozgo;
}

void save_leaderboard(Leaderboard *lb) {
    FILE *fp = fopen(lb->filename, "w");
    if (fp == NULL) {
        return; // error
    }

    fprintf(fp, "BSZNAKE_RANGLISTAv1\n");
    for (Result *mozgo = lb->results; mozgo != NULL; mozgo = mozgo->next) {
        fprintf(fp, "%s\n%d\n", mozgo->name, mozgo->score);
    }
    fclose(fp);
}

void close_leaderboard(Leaderboard *lb) {
    Result *mozgo = lb->results;

    while (mozgo != NULL) {
        Result *temp = mozgo;
        mozgo = mozgo->next;
        free(temp);
    }

    free(lb);
}

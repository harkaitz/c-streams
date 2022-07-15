#ifndef IO_FREPLACE_H
#define IO_FREPLACE_H

#include <stdio.h>
#include <string.h>

static __attribute__((unused))
int freplace_a(FILE *_i, FILE *_o, const char *_map[]) {

    /* Get the size of the map and the biggest. */
    size_t bmax  = 0;
    size_t mapsz = 0;
    while (_map[mapsz*2]) {
        size_t l = strlen(_map[mapsz*2]);
        if (l>bmax) bmax = l;
        mapsz++;
    }
    
    /* Allocate matcher and buffer. */
    char   b[bmax+1];
    size_t bsz = 0;
    int    match[mapsz];
    memset(match, 0, sizeof(match));
   
    /* While loop. */
    int chr;
    while ((chr = fgetc(_i))!=EOF) {
        int         match_count = 0;
        const char *match_found = NULL;
        /* Search on mapping. */
        for (int m=0; m<mapsz; m++) {
            int        *matched   = match+m;
            const char *candidate = _map[m*2];
            if (chr == candidate[*matched]) {
                (*matched)++;
                if (candidate[*matched]=='\0') {
                    match_found = _map[m*2+1];
                }
                match_count++;
            } else {
                (*matched)=0;
            }
        }
        /* A match found, write and reset. */
        if (match_found) {
            memset(match, 0, sizeof(match));
            bsz = 0;
            if (fputs(match_found, _o)==EOF) return -1;
            continue;
        }
        /* There are matches, save. */
        if (match_count) {
            b[bsz++] = chr;
            continue;
        }
        /* No matches, write. */
        if (bsz) {
            if (fwrite(b, 1, bsz, _o)!=bsz) return -1;
            bsz = 0;
        }
        if (fputc(chr, _o)==EOF) return -1;
    }
    return (!ferror(_i))?0:-1;
}

#endif

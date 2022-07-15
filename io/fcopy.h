#ifndef IO_FCOPY_H
#define IO_FCOPY_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static __attribute__((unused))
ssize_t fcopy_ff(FILE *_to, FILE *_fr, size_t _bsize, FILE *_opt_dots) {
    size_t  bsize = (_bsize>0)?_bsize:1024;
    char   *b     = malloc(bsize);
    ssize_t ret  = 0;
    if (!b/*err*/) return -1; /* errno */
    while (1) {
        size_t r = fread(b, 1, bsize, _fr);
        if (ferror(_fr)) break;
        if (r) {
            size_t w = fwrite(b, 1, r, _to);
            ret += w;
            if (_opt_dots) fputc('.', _opt_dots); 
            if (ferror(_to)) break;
        }
        if (r < bsize) break;
    }
    if (_opt_dots) fputc('\n', _opt_dots); 
    free(b);
    if (ferror(_fr) || ferror(_to)) {
        return -ret;
    } else {
        return ret;
    }
}

static __attribute__((unused))
ssize_t fcopy_fd(FILE *_to, int _fr, size_t _bsize, FILE *_opt_dots) {
    size_t  bsize = (_bsize>0)?_bsize:1024;
    char   *b     = malloc(bsize);
    ssize_t ret  = 0;
    ssize_t r    = 0; 
    if (!b/*err*/) return -1; /* errno */
    while (1) {
        r = read(_fr, b, bsize);
        if (r == -1/*err*/) break;
        if (r) {
            size_t w = fwrite(b, 1, r, _to);
            ret += w;
            if (_opt_dots) fputc('.', _opt_dots); 
            if (ferror(_to)) break;
        }
        if (!r) break;
    }
    if (_opt_dots) fputc('\n', _opt_dots); 
    free(b);
    if (r == -1 || ferror(_to)) {
        return -ret;
    } else {
        return ret;
    }
}

static __attribute__((unused))
ssize_t fcopy_dd(int _to, int _fr, size_t _bsize, FILE *_opt_dots) {
    size_t  bsize = (_bsize>0)?_bsize:1024;
    char   *b     = malloc(bsize);
    ssize_t ret  = 0;
    ssize_t r    = 0;
    ssize_t w    = 0;
    if (!b/*err*/) return -1;
    while (1) {
        r = read(_fr, b, bsize);
        if (r == -1/*err*/) break;
        if (r) {
            w = write(_to, b, r);
            if (w==-1/*err*/) break;
            ret += w;
            if (_opt_dots) fputc('.', _opt_dots); 
        }
        if (!r) break;
    }
    if (_opt_dots) fputc('\n', _opt_dots); 
    free(b);
    if (r == -1 || w == -1) {
        return -ret;
    } else {
        return ret;
    }
}

#endif

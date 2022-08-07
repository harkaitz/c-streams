#ifndef STDIO_REC_H
#define STDIO_REC_H
#ifdef FLYCHECK
#  define _POSIX_C_SOURCE 202200L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

static inline int
rec_fputs(const char *_s, FILE *_fp, bool _escape)
{
    if (!_escape) return fputs(_s, _fp);
    for (char const *s = _s; *s; s++) {
        if (*s=='\n') {
            fputs("\n+ ", _fp);
        } else {
            fputc(*s, _fp);
        }
    }
}

static inline int
rec_fputm(const char *_s, size_t _sz, FILE *_fp, bool _escape)
{
    if (!_escape) return fputs(_s, _fp);
    for (size_t i=0; i<_sz; i++) {
        if (_s[i]=='\n') {
            fputs("\n+ ", _fp);
        } else {
            fputc(_s[i], _fp);
        }
    }
}

static inline int
rec_vfprintf(FILE *_fp, bool _escape, const char *_fmt, va_list _va)
{
    char          *b               = NULL;
    size_t         bsz             = 0;
    FILE          *fp              = NULL;
    int            e;

    fp = open_memstream(&b, &bsz);
    if (!fp/*err*/) { return -1; }

    e = vfprintf(fp, _fmt, _va);
    if (e==-1/*err*/) { fclose(fp); free(b); return -1; }
    fflush(fp);

    e = rec_fputm(b, ftell(fp), _fp, _escape);
    if (e==-1/*err*/) { fclose(fp); free(b); return -1; }

    fclose(fp);
    free(b);
    
    return 0;
}

static inline __attribute__ ((__format__ (__printf__, 3, 4))) int
rec_fprintf(FILE *_fp, bool _escape, const char *_fmt, ...)
{
    va_list va;
    va_start(va, _fmt);
    int r = rec_vfprintf(_fp, _escape, _fmt, va);
    va_end(va);
    return r;
}

#endif

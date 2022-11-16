#ifndef INCBIN_FP_H
#define INCBIN_FP_H

#ifdef INCBIN_HDR
#  error Include <incbin-fp.h> before <incbin.h>
#endif
#ifdef INCBIN_PREFIX
#  error The INCBIN_PREFIX with <incbin-fp.h> is INCBIN_.
#endif
#ifdef INBIN_STYLE
#  error The INCBIN_STYLE with <incbin-fp.h> is INCBIN_STYLE_SNAKE.
#endif

#define INCBIN_PREFIX INCBIN_
#define INCBIN_STYLE  INCBIN_STYLE_SNAKE
#include <incbin.h>
#include <stdio.h>
#include <stdbool.h>

__attribute__((weak)) bool INCBIN_FP2_TEXTMODE = false;

/* ------------------------------------------------------------------ */

#define INCBIN_FP2(A,B,C) \
    INCBIN(A ## _HTML,B); \
    INCBIN(A ## _TEXT,C); \
    void fmem_ ## A (FILE **_out) { \
        if (*_out) fclose(*_out);                        \
        if (INCBIN_FP2_TEXTMODE) {                        \
            void  *d   = (void*) INCBIN_ ## A ## _TEXT_data;  \
            size_t dsz =         INCBIN_ ## A ## _TEXT_size;  \
            *_out = fmemopen(d, dsz, "r");               \
        } else {                                         \
            void  *d   = (void*) INCBIN_ ## A ## _HTML_data;  \
            size_t dsz =         INCBIN_ ## A ## _HTML_size;  \
            *_out = fmemopen(d, dsz, "r");               \
        }                                                \
    }                                                    \
    void fmem_ ## A (FILE **_out)

#define incbin_fp2_set_textmode() INCBIN_FP2_TEXTMODE = true

/* ------------------------------------------------------------------ */

#define INCBIN_FP(A,B)                              \
    INCBIN(A ## _HTML, B);                          \
    void fmem_ ## A (FILE **_out) {                 \
        if (*_out) fclose(*_out);                   \
        void  *d   = (void*) INCBIN_ ## A ## _HTML_data; \
        size_t dsz =         INCBIN_ ## A ## _HTML_size; \
        *_out = fmemopen(d, dsz, "r");              \
    }                                               \
    void fmem_ ## A (FILE **_out)

#define INCBIN_BUF(A, B) \
    void fmem_ ## A (FILE **_out) {                   \
        static const char b[] = (B);                  \
        if (*_out) fclose(*_out);                     \
        *_out = fmemopen((char*)b, sizeof(b)-1, "r"); \
    }

#endif

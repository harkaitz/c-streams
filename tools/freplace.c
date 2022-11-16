#define _POSIX_C_SOURCE 201000L
#include "../io/freplace.h"
#include <errno.h>
#include <libgen.h>
#include <stdlib.h>
#include <getopt.h>

#define COPYRIGHT_LINE \
    "Bug reports, feature requests to gemini|https://harkadev.com/oss" "\n" \
    "Copyright (c) 2022 Harkaitz Agirre, harkaitz.aguirre@gmail.com" "\n" \
    ""

static char const help[] =
    "Usage: %s"                                       "\n"
    ""                                                "\n"
    "    -d DELIMS   : Delimeters for map files."     "\n"
    "    -t MAP-FILE : Transform col1->col2"          "\n"
    "    -r MAP-FILE : Revert col2->col1"             "\n"
    "    -T MAP      : Transform from the arguments." "\n"
    "    -R MAP      : Rever from the arguments."     "\n"
    "    -p          : Print mapping."                "\n"
    ""                                                "\n"
    COPYRIGHT_LINE;

static int fp2map(FILE *_fp, int flip);

char       *map[1000];
size_t      mapsz  = 0;
char const *delims = " \r\t";
int main (int _argc, char *_argv[]) {

    int            retval          = 1;
    FILE          *fp;
    int            opt,e;
    int            opt_p = 0;

    if (_argc == 1 || !strcmp(_argv[1], "-h") || !strcmp(_argv[1], "--help")) {
        fprintf(stdout, help, basename(_argv[0]));
        return 0;
    }

    while((opt = getopt (_argc, _argv, "d:t:r:T:R:p")) != -1) {
        switch (opt) {
        case 'd':
            delims = optarg;
            break;
        case 't':
        case 'r':
            fp = fopen(optarg, "r");
            if (!fp/*err*/) goto fail_optarg_errno;
            e = fp2map(fp, opt=='r');
            fclose(fp);
            if (e==-1/*err*/) goto cleanup;
            break;
        case 'T':
        case 'R':
            fp = fmemopen(optarg, strlen(optarg), "r");
            if (!fp/*err*/) goto fail_errno;
            e = fp2map(fp, opt=='R');
            fclose(fp);
            if (e==-1/*err*/) goto cleanup;
            break;
        case 'p':
            opt_p = 1;
            break;
        case '?':
        default:
            return 1;
        }
    }
    map[mapsz] = NULL;

    if (opt_p) {
        for (size_t i=0; i<mapsz; i+=2) {
            fprintf(stdout, "[%s][%s]\n", map[i], map[i+1]);
        }
    } else {
        e = freplace_a(stdin, stdout, (const char **)map);
        if (e==-1/*err*/) goto fail_errno;
    }
    retval = 0;
 cleanup:
    for (size_t i=0; i<mapsz; i++) {
        free(map[mapsz]);
    }
    return retval;
 fail_optarg_errno:
    fprintf(stderr, "%s: %s: %s\n", basename(_argv[0]), optarg, strerror(errno));
    goto cleanup;
 fail_errno:
    fprintf(stderr, "%s: %s\n", basename(_argv[0]), strerror(errno));
    goto cleanup;
}

static int fp2map(FILE *_fp, int flip) {
    char *b, *key1,*key2, *nl, buffer[1024];

    while ((b = fgets(buffer, sizeof(buffer)-1, _fp)) && mapsz < 998) {
        buffer[sizeof(buffer)-1] = '\0';
        key1 = key2 = NULL;
        /* Remove newline. */
        if ((nl = strchr(b, '\n'))) *nl = '\0';
        /* Remove spaces before the key. */
        while (*b && strchr(" \t\r", *b)) b++;
        if (*b) key1 = b; else continue;
        /* Search the key ending. */
        while (*b && !strchr(delims, *b)) b++;
        if (*b) *(b++) = '\0'; else continue;
        /* Traverse the separation. */
        while (*b && strchr(delims, *b)) b++;
        key2 = b;
        /* Add mapping. */
        if (!flip) {
            map[mapsz++] = strdup(key1);
            map[mapsz++] = strdup(key2);
        } else if (*key2 && flip) {
            map[mapsz++] = strdup(key2);
            map[mapsz++] = strdup(key1);
        }
    }

    if (mapsz >= 998) {
        fprintf(stderr, "Maximun number of mappings reached.\n");
        return -1;
    }
    return 0;
}

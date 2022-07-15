#include "../include/io/freplace.h"
#include "../include/str/strarray.h"
#include <errno.h>

int main (int _argc, char *_argv[]) {
    char *map[_argc*2+1];
    streq2map(_argv+1, _argc*2+1, map);
    int res = freplace_a(stdin, stdout, (const char **)map);
    if (res==-1) {
        fprintf(stderr, "%s", strerror(errno));
        return 1;
    }
    fflush(stdout);
    return 0;
}

#include "../include/io/fcopy.h"
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <syslog.h>
#include <errno.h>
#include <sys/wait.h>

#define COPYRIGHT_LINE \
    "Bug reports, feature requests to gemini|https://harkadev.com/oss" "\n" \
    "Copyright (c) 2022 Harkaitz Agirre, harkaitz.aguirre@gmail.com" "\n" \
    ""

int main (int _argc, char *_argv[]) {

    int         retval = 1;
    int         opt    = 0;
    FILE       *m_fp   = NULL;
    char       *m      = NULL;
    size_t      m_sz   = 0;
    FILE       *o_fp   = stdout;
    int         input  = -1;
    ssize_t     count  = 0;
    ssize_t     shift  = 0;
    size_t      bsize  = 0;
    int         p[2]   = {-1,-1};
    int         child  = -1;
    const char *cmd    = NULL;
    FILE       *dots   = NULL;
    int         res    = 0;
    int         status = 0;
    bool        print_count = false;
    const char *ifile       = NULL;
    const char *ofile       = "stdout";
    
    if (_argc > 1 && (!strcasecmp(_argv[1], "-h") || !strcasecmp(_argv[1], "--help"))) {
        printf("Usage: fcopy [-v][-b BLEN][-c][-o FILE][-p CMD] FILES..."             "\n"
               ""                                                                     "\n"
               "Read from files (or standard input) to memory. Then if everything is" "\n"
               "correct write to output."                                             "\n"
               ""                                                                     "\n"
               COPYRIGHT_LINE
               );
        return 0;
    }

    while ((opt = getopt (_argc, _argv, "vb:co:p:")) != -1) {
        switch (opt) {
        case 'v': dots = stderr; break;
        case 'b': bsize = strtol(optarg, NULL, 10); break;
        case 'c': print_count = true; break;
        case 'o':
            ofile = optarg;
            o_fp = fopen(optarg, "w");
            if (!o_fp/*err*/) goto cleanup_errno;
            break;
        case 'p': cmd = optarg; break;
        case '?':
        default:
            return 1;
        }
    }

    m_fp = open_memstream(&m, &m_sz);
    if (!m_fp/*err*/) goto cleanup_errno;

    if (optind == _argc) {
        ifile = "stdin";
        input = 0;
    }
    
    do {
        if (input == -1) {
            ifile = _argv[optind];
            input = open(_argv[optind], O_RDONLY);
            if (input==-1/*err*/) goto cleanup_errno;
            optind++;
        }

        if (cmd) {
            res = pipe(p);
            if (res==-1/*err*/) goto cleanup_errno;
            child = fork();
            if (child==-1/*err*/) goto cleanup_errno;
            if (child==0) {
                dup2(p[1], 1);
                if (input != 0) {
                    dup2(input, 0);
                    close(input);
                }
                close(p[0]);
                close(p[1]);
                execl("/bin/sh", "/bin/sh", "-e", "-c", cmd, NULL);
                syslog(LOG_ERR, "Can't exec /bin/sh: %s", strerror(errno));
                exit(1);
            }
            close(input);
            close(p[1]);
            input = p[0];
            p[0] = -1;
            p[1] = -1;
        }
        if (dots) {
            fprintf(dots, "[i] %s: ", ifile);
        }
        
        shift = fcopy_fd(m_fp, input, bsize, dots);
        if (shift<0/*err*/) goto cleanup_fcopy_errno;
        count += shift;

        if (child != -1) {
            res = waitpid(child, &status, 0);
            if (res==-1/*err*/) goto cleanup_errno;
            if (WIFEXITED(status) && WEXITSTATUS(status)/*err*/) goto cleanup_cmdfail;
        }
            
        close(input);
        input = -1;

    } while (optind < _argc);

    res = fflush(m_fp);
    if (res==EOF/*err*/) goto cleanup_fcopy_errno;
    res = fseek(m_fp, 0, SEEK_SET);
    if (res==EOF/*err*/) goto cleanup_fcopy_errno;

    if (print_count) {
        printf("%li\n", count);
    }

    if (dots) {
        fprintf(dots, "[o] %s: ", ofile);
    }
    shift = fcopy_ff(o_fp, m_fp, bsize, dots);
    if (shift<0/*err*/) goto cleanup_fcopy_errno;
    
    retval = 0;
    goto cleanup;
 cleanup_errno:
    syslog(LOG_ERR, "%s", strerror(errno));
    goto cleanup;
 cleanup_fcopy_errno:
    syslog(LOG_ERR, "%s", strerror(errno));
    goto cleanup;
 cleanup_cmdfail:
    syslog(LOG_ERR, "Child returned error.");
    goto cleanup;
 cleanup:
    if (m_fp)      fclose(m_fp);
    if (m)         free(m);
    if (o_fp)      fclose(o_fp);
    if (input!=-1) close(input);
    if (p[0]!=-1)  close(p[0]);
    if (p[1]!=-1)  close(p[1]);
    if (child!=-1) {
        kill(child, SIGINT);
        waitpid(child, NULL, 0);
    }
    return retval;
}

# FCOPY

fcopy_ff(), fcopy_fd()

# SYNOPSIS

    #include <io/fcopy.h>
    
    ssize_t fcopy_ff(FILE *_to, FILE *_fr, size_t _bsize, FILE *_opt_dots);
    ssize_t fcopy_fd(FILE *_to, int _fr, size_t _bsize, FILE *_opt_dots)

# DESCRIPTION

This functions read from one file descriptor or file pointer and write to
another. If you specify *_opt_dots* then dots are printed to *_opt_dots* in
the process for each write.

# RETURN VALUE

In case of error they return the written bytes in negative. In case of success
then it prints a positive value (or zero).

# COLLABORATING

For making bug reports, feature requests and donations visit
one of the following links:

1. [gemini://harkadev.com/oss/](gemini://harkadev.com/oss/)
2. [https://harkadev.com/oss/](https://harkadev.com/oss/)

# SEE ALSO

**open(3)**, **fopen(3)**, **open_memstream(3)**

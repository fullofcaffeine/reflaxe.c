#ifndef CAXECRAFT_POSIX_OPENAT_H
#define CAXECRAFT_POSIX_OPENAT_H

#include <fcntl.h>
#include <sys/types.h>

/*
 * Fixed-arity ABI shim for the optional mode argument of POSIX openat.
 *
 * haxe.c deliberately admits only the fixed prefix of variadic C functions.
 * The Haxe caller supplies every policy flag and retains descriptor ownership;
 * this header only makes the reviewed four-argument ABI shape explicit.
 */
static inline int caxecraft_posix_openat_with_mode(
    int directory_descriptor,
    const char *path,
    int flags,
    mode_t mode)
{
    return openat(directory_descriptor, path, flags, mode);
}

#endif

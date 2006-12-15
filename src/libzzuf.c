/*
 *  zzuf - general purpose fuzzer
 *  Copyright (c) 2006 Sam Hocevar <sam@zoy.org>
 *                All Rights Reserved
 *
 *  $Id$
 *
 *  This program is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What The Fuck You Want
 *  To Public License, Version 2, as published by Sam Hocevar. See
 *  http://sam.zoy.org/wtfpl/COPYING for more details.
 */

/*
 *  libzzuf.c: preloaded wrapper library
 */

#include "config.h"
#define _GNU_SOURCE

#if defined HAVE_STDINT_H
#   include <stdint.h>
#elif defined HAVE_INTTYPES_H
#   include <inttypes.h>
#endif
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <regex.h>

#include <stdarg.h>
#include <dlfcn.h>

#include "libzzuf.h"
#include "debug.h"
#include "preload.h"

/* Global variables */
int   _zzuf_ready   = 0;
int   _zzuf_debug   = 0;
int   _zzuf_seed    = 0;
float _zzuf_percent = 0.04f;
regex_t * _zzuf_include = NULL;
regex_t * _zzuf_exclude = NULL;

#define MAXFD 1024
struct zzuf files[MAXFD];

/* Library initialisation shit */
void zzuf_init(void)
{
    char *tmp;
    int i;

    if(zzuf_preload())
        abort();

    tmp = getenv("ZZUF_DEBUG");
    if(tmp && *tmp)
        _zzuf_debug = 1;

    tmp = getenv("ZZUF_SEED");
    if(tmp && *tmp)
        _zzuf_seed = atol(tmp);

    tmp = getenv("ZZUF_PERCENT");
    if(tmp && *tmp)
        _zzuf_percent = atof(tmp);
    if(_zzuf_percent < 0.0f)
        _zzuf_percent = 0.0f;
    else if(_zzuf_percent > 1.0f)
        _zzuf_percent = 1.0f;

    tmp = getenv("ZZUF_INCLUDE");
    if(tmp && *tmp)
    {
        _zzuf_include = malloc(sizeof(*_zzuf_include));
        regcomp(_zzuf_include, tmp, 0);
    }

    tmp = getenv("ZZUF_EXCLUDE");
    if(tmp && *tmp)
    {
        _zzuf_exclude = malloc(sizeof(*_zzuf_exclude));
        regcomp(_zzuf_exclude, tmp, 0);
    }

    for(i = 0; i < MAXFD; i++)
        files[i].managed = 0;

    _zzuf_ready = 1;
}

/* Deinitialisation */
void zzuf_fini(void)
{
    int i;

    for(i = 0; i < MAXFD; i++)
    {
        if(!files[i].managed)
            continue;

        /* TODO */
    }
}


/* infcodes.h -- header to use infcodes.c
 * Copyright (C) 1995 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h 
 */

/* WARNING: this file should *not* be used by applications. It is
   part of the implementation of the compression library and is
   subject to change. Applications should only use zlib.h.
 */

#ifndef __INFCODES_H__
#define __INFCODES_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

struct inflate_codes_state;
typedef struct inflate_codes_state FAR inflate_codes_statef;

extern inflate_codes_statef *inflate_codes_new OF((
    uInt, uInt,
    inflate_huft *, inflate_huft *,
    z_stream *));

extern int inflate_codes OF((
    inflate_blocks_statef *,
    z_stream *,
    int));

extern void inflate_codes_free OF((
    inflate_codes_statef *,
    z_stream *));


#endif // __INFCODES_H__
/*
 * jversion.h
 *
 * Copyright (C) 1991, 1992, 1993, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains software version identification.
 */

#ifndef __SFL_JVERSION_H__
#define __SFL_JVERSION_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

namespace stingray {
namespace foundation {

#define JVERSION	_T("4A 18-Feb-93")

#define JCOPYRIGHT	_T("Copyright (C) 1993, Thomas G. Lane")

};  // namespace stingray::foundation
};  // namespace stingray

#endif /* __SFL_JVERSION_H__ */
/* $Header: /4CReleased/V2.20.00/Target/4CWin/Control/inc/osFile.h 2     3.08.07 15:01 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: osFile.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/Control/inc/osFile.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				RunTime
 *
 * =COMPONENT			osKernel
 *
 * =CURRENT 	 $Date: 3.08.07 15:01 $
 *			 $Revision: 2 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _OSFILE_H_
#define _OSFILE_H_

/* Operating system dependent include files
 * ----------------------------------------------------------------------------
 */
#include <sys\stat.h>
#include <direct.h>


/* File IO Definitions
 * ----------------------------------------------------------------------------
 */
#define VMF_INVALID_HANDLE			NULL			/* Invalid handle value */

#define VMF_FILE					FILE *			/* File pointer 		*/

#define VMF_MODE_READ				"r" 			/* File open mode:		*/
#define VMF_MODE_WRITE				"w+"			/* -> Access mode		*/
#define VMF_MODE_APPEND 			"a+"
#define VMF_MODE_RDWR				"r+"

#define VMF_MODE_TEXT				"t" 			/* File open mode:		*/
#define VMF_MODE_BIN				"b" 			/* -> Text/binary file	*/

#define VMF_DIR_READ				0				/* Dir. creation mode	*/
#define VMF_DIR_WRITE				0				/* -> Read/Write		*/

#define VMF_SEEK_CUR				SEEK_CUR		/* Seek origins 		*/
#define VMF_SEEK_END				SEEK_END
#define VMF_SEEK_SET				SEEK_SET


/* Return Values
 * ----------------------------------------------------------------------------
 */
#define VMF_RET_OK					0				/* Positive return val. */


/* API functions
 * ----------------------------------------------------------------------------
 */
#define osfclose(str)				fclose(str)
#define osfopen(nam,mod)			fopen(nam,mod)
#define osfseek(str,off,org)		fseek(str,off,org)
#define osfread(buf,siz,cnt,str)	fread(buf,siz,cnt,str)
#define osfeof(str) 				feof(str)
#define osfgets(buf,n,str)			fgets(buf,n,str)
#define osfwrite(buf,siz,cnt,str)	fwrite(buf,siz,cnt,str)
#define osfputs(buf,str)			fputs(buf,str)
#define osfflush(str)				fflush(str)
#define osfsync(str)				
#define ossync()

#define osstrerror					strerror(errno)

#define osremove(nam)				remove(nam)
#define osrename(from,to)			rename(from,to)

#define oschdir(dn) 				_chdir(dn)
#define osmkdir(dn,mod) 			_mkdir(dn)
#define oschdrive(dn)				_chdrive(dn)

#define osstat(fn,buf)				_stat(fn,buf)
#define osstructstat				_stat

#endif /* _OSFILE_H_ */

/* ---------------------------------------------------------------------------- */

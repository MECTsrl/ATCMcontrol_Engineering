/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Mar 24 16:29:22 2015
 */
/* Compiler settings for AddOnHandler.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID LIBID_ADDONHANDLERLib = {0xE07E1A27,0x8A91,0x46c1,{0xA4,0xCD,0xF4,0x6D,0x1A,0xC4,0xCA,0xE0}};


const CLSID CLSID_HW119AddonObject = {0xBEAEB7F7,0x3B03,0x477c,{0x98,0x9A,0xA3,0x1A,0x04,0xE0,0x04,0x94}};


#ifdef __cplusplus
}
#endif


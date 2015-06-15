/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Mar 24 16:29:21 2015
 */
/* Compiler settings for CSC_OnlSrv.idl:
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

const IID LIBID_CSC_Srv_Lib = {0xD7508B45,0x4384,0x400c,{0x8F,0x7E,0x7E,0xE3,0xF9,0x97,0x36,0x16}};


const CLSID CLSID_CustomConnect = {0xC31E31A7,0x567C,0x49a2,{0xB3,0x93,0xCE,0xC8,0xF2,0xE1,0x7F,0x99}};


#ifdef __cplusplus
}
#endif


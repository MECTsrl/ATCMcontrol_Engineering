/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Util.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_Util.hpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Util.hpp $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CBar
 *
 * =COMPONENT           bar
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *           $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  02.07.01  KT      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/

#ifndef _BAR_UTIL_HPP
#define _BAR_UTIL_HPP

#include <windows.h>
#include <string>
#include <sstream>
using namespace ::std;
#include "CSC_OnlineServer.h"

string  HRToString(HRESULT hr);
wstring HRToWString(HRESULT hr);

string  TrgToString(CSC_TRANSFER_TARGET trg);

string  GetUpper(const string& s);

wstring ToWS(const string& s);

bool ExistsFile(const string& sFileName);
bool ExistsDir(const string& sDirName);

string GUIDToString(const GUID& guid);

// encryption
typedef enum E_EncryptErr
{
    eEE_Ok,             //  0 = Ok
    eEE_OpenFile,       //  1 = file could not be opened
    eEE_ReadFile,       //  2 = error while read file
    eEE_CreateFile,     //  3 = file creation failed
    eEE_WriteFile,      //  4 = error while writing file
    eEE_WrongPassword,  //  5 = wrong encryption password
    eEE_NoMemory = 99,  // 99 = memory allocation error
};

// DES algorithm functions
int  desinit(int mode);
void desdone();
int  setkey(char (*kn)[8], const char* key);
int  endes(char (*kn)[8], char* block);
int  dedes(char (*kn)[8], char* block);

// encryption / decryption of single blocks
BOOL EncryptBlock(char* lpszBuffer, const char* lpszKey);
BOOL DecryptBlock(char* lpszBuffer, const char* lpszKey);

// encryption / decryption of files
E_EncryptErr Pack(const char* lpszUnpackedFile, const char* lpszPackedFile, const char* lpszKey);
E_EncryptErr Unpack(const char* lpszPackedFile, const char* lpszUnpackedFile, const char* lpszKey);

#endif _BAR_UTIL_HPP

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_Util.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 21.04.04   Time: 13:20
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * added encrypt
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 10.07.01   Time: 16:39
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

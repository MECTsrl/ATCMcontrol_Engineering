/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/utilif.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: utilif.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/utilif.h $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           FC_UTIL
 *
 * =CURRENT             $Date: 28.02.07 19:01 $
 *                      $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==ATCMControl core system utillity functions, Note: this header should only
 * contain 4C specific functions. Real generall purpose code/defines should 
 * be in fc_tools.lib.
 * CAUTION: header file under construction (moving from V1.3) some functions will
 * be removed/moved to fc_tools.lib.
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  23.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#ifndef __UTILIF_H_
#define __UTILIF_H_


#if	defined	(UTIL_IMP)
#define	UTILIF __declspec(dllexport)
#else
#define	UTILIF __declspec(dllimport)
#endif

#ifdef	__cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////
// Attention: This header file shall be used in a project wide context
// SWKE local definitions shall be kept away.
////////////////////////////////////////////////////////////////////////////
#include "IECKeywords.h"



//$CONTROL Core system defines:
#define UTIL_INSTALL_KEY	        _T("SOFTWARE\\Softing\\4ControlV2\\2.0")
#define UTIL_INSTALL_PATH	        _T("Path") 
#define UTIL_INSTALL_LIBPATH        _T("LibPath") 
#define UTIL_STD_PRJ_PATH           _T("Projects")


#define UTIL_TEMPFILE_PREFIX        _T("4C")
#define UTIL_ADDR_START_DELIMITER   _T('{')
#define UTIL_ADDR_END_DELIMITER     _T('}')

/////////////////////////////////////////////////////////////////////////////
// communication and control string definitions
/////////////////////////////////////////////////////////////////////////////
#define UTIL_BRACKET_OPEN           _T('(')
#define UTIL_BRACKET_CLOSE          _T(')')
#define UTIL_VERSION_STRING         _T("VERSION")

#define UTIL_MAX_OPC_SERVER_LEN         64
#define UTIL_MAX_GUID_LEN               37
#define UTIL_MAX_OPC_INST_NAME_LEN      64
#define UTIL_MAX_COMM_CHANNEL_LEN       8
#define UTIL_MAX_CONNECT_PAR_LEN        64
#define UTIL_MAX_PROTOCOL_LEN           8
#define UTIL_MAX_PROTOCOL_PAR_LEN       64
#define UTIL_MAX_FIRMWARE_VERSION_LEN   6
#define UTIL_MAX_CONTROL_STRING_LEN     319     // sum of max lengths above + 4 commas

// encryption / decryption
#define UTIL_CRYPTKEYSIZE               16
#define UTIL_FINGERPRINTSIZE            16

/////////////////////////////////////////////////////////////////////////////


//V2.0:   #define ARRAY_LEN(var)      use fc_tools\fc_defines.h FC_ARRAY_LEN
//V2.0:   #ifndef UTIL_FREESYSTRING   use fc_tools\fc_defines.h FC_FREESYSTRING



// HRESULTS of UTIL functions:
//
// MessageId: E_UTIL_ID_SYNTAX
//
// MessageText:
//
//  Identifier syntax: '%1' invalid syntax, corret is: (letter | ('_' (letter|digit))) {['_'] (letter|digit)}
//
#define E_UTIL_ID_SYNTAX                 0xC2F20001L

//---------------
//
// MessageId: E_UTIL_ID_IS_KEYWORD
//
// MessageText:
//
//  Identifier syntax: '%1' is a reserved keyword
//
#define E_UTIL_ID_IS_KEYWORD             0xC2F20002L

//---------------
//
// MessageId: E_UTIL_ID_TOO_LONG
//
// MessageText:
//
//  Identifier syntax: '%1' is too long, maximum is 32 characters 
//
#define E_UTIL_ID_TOO_LONG               0xC2F20003L




// character encoding to use when encoding/decoding strings for XML files
// V1.1: we currently have one (default) representation: UTF-8
typedef enum {
    XML_ENCODE_UTF_8,
    XML_ENCODE_DEFAULT = XML_ENCODE_UTF_8
} XML_CHAR_ENCODING;


// OS enumerators
typedef enum {
    unknown,
    // Win16 only !!!
    Windows2,
    Windows30,
    WfW311,
    Windows31,
    // Win32
    Win32s,                     // WfW 3.11 with Win32s
    Windows95,                  // Win32; Win16 under Win95
    WindowsNT,                  // NT w/o common controls or Win16 under NT!
    WindowsNT351,
    WindowsNT40
} OS_ENV;

typedef enum {
    UTIL_FULL_VERSION,          //Version PROD_VER_MAJOR.PROD_VER_MINOR(PRODUCT_BUILD)
    UTIL_SHORT_VERSION          //VPROD_VER_MAJOR.PROD_VER_MINOR
}UTIL_VERSION_SPEC;

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>retrieves ATCMControl install path from the registry</B>.
*
*	Description = UTIL_GetInstallPath retrieves ATCMControl install path from the registry
*	The function stores the path in the passed @param pszPath.
*	<p>
*	Specification = 
*	<p>
*	@param	pszPath - string pointer where the install path will be
*			copied to
*			pSizeChars - pointer to size of the buffer in characters
*	<p>
*	@return hr = E_INVALIDARG if the param is Null
*	@return hr = E_OUTOFMEMORY a memory allocation operation fails
*	@return hr = S_FALSE registry key or value not found
*	@return hr = E_FAIL	- memory size to small *pSizeChars contains 
*				 necessary size.
*	@return hr = S_OK - Success
*
*	@exception No
*	<p>
*	@see
*	<p>
*/
UTILIF HRESULT UTIL_GetInstallPath(IN LPTSTR pszPath,IN OUT size_t *pSizeChars);

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>UTIL_GetFileTitle</B>.
*
*	Description = Retrieves the file title (filename without extension) from a file path.
*	<p>
*	Specification = 
*	<p>
*	@param	pszPath - 
*	<p>
*	@param	ppszTitle -
*	<p>
*	@param	SizeChars -
*	<p>
*
*	@return
*	<p>
*	@return
*	<p>
*
*
*	@exception
*	<p>
*	@see
*	<p>
*/
//V2.0 use inc\fc_tools\FC_StringGetFileTitle
UTILIF HRESULT UTIL_GetFileTitle(IN LPCTSTR pszPath, LPTSTR pszTitle, size_t SizeChars);


//V2.0 use inc\fc_tools\fc_string.h FC_StringIsAbsPath()
UTILIF HRESULT UTIL_IsAbsolutePath(LPCTSTR pszPath);

UTILIF HRESULT UTIL_GetFileExt(IN LPCTSTR pszPath, LPTSTR pszExt, size_t SizeChars);
UTILIF HRESULT UTIL_GetFileDates(LPCTSTR pszFilePath, VARIANT & vtCreate, VARIANT & vtModify);

//class id to keyword and language dependent description
//V2.0 use inc\fc_tools\fc_registry FC_* reg functions
UTILIF BOOL UTIL_SetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName);

//V2.0 ????
UTILIF HRESULT UTIL_GetModulePath(HINSTANCE hInstance,LPTSTR pszPath,size_t SizeChars);

// SIS, 22.10.01: copied from V1.3
// needed for open console
UTILIF LPCTSTR UTIL_GetMasterPassword();

// SIS, 15.01.02: set file changed time to current time
UTILIF BOOL UTIL_TouchFile(LPCTSTR lpszFilePath);
/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>UTIL_MakeMachineName</B>.
*
*	Description = returns a string constant containing a decorated machine name ('['MachineName']') 
*	<p>
*	Specification = 
*	<p>
*	@param pszMachineName - input, output, machine name (may be decorated)
*	<p>
*	@param SizeChars        input, length of output buffer in chars.
*   <p>
*	@return - string constant
*	<p>
*/
UTILIF HRESULT UTIL_MakeMachineName(LPTSTR pszMachineName, size_t SizeChars); 

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>UTIL_MakeMachineName</B>.
*
*	Description = returns a string constant containing a undecorated machine name (removes '[' and']'), 
*	<p>
*	Specification = 
*	<p>
*	@param pszMachineName - input, output, machine name (may be decorated)
*	<p>
*	@param SizeChars        input, length of output buffer in chars.
*   <p>
*	@return - string constant
*	<p>
*/
UTILIF HRESULT UTIL_StripMachineName(LPTSTR pszMachineName, size_t SizeChars); 

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>UTIL_GetLocalMachineName</B>.
*
*	Description = returns a string constant containing the local machine name,
*                   Windows NT: local machine name
*                   Windows 95: empty string 
*	<p>
*	Specification = 
*	<p>
*	@param pszMachineName - output, machine name
*	<p>
*	@param SizeChars        input, length of output buffer in chars.
*   <p>
*	@return - string constant
*	<p>
*/
UTILIF HRESULT UTIL_GetLocalMachineName(LPTSTR pszMachineName, size_t SizeChars);

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>UTIL_DisplaySysError</B>.
*
*	Description = Displays an error description for a system error. The
*	error description will be obtained by a call to system function
*	
*	<p>
*	@param dwError - System error coded (for example obtained by GetLastError)
*	<p>
*	@param pszTitle - Optional title. If NULL a system default title will be displayed.
*	@param pszOpt - Optional string which is appendend to the system message separated
*	by a '\n'. lpszOpt can be a NULL-pointer.
*	<p>
*
*	@return - S_OK
*	<p>
*	@exception - No
*	<p>
*/
UTILIF HRESULT UTIL_DisplaySysError(HRESULT hrError,LPCTSTR pszTitle,LPCTSTR pszOpt);

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*   <B>UTIL_GetTempPath</B>.
*
*   Description = Returns the system temporary path in pszPath. If the
*   allocated buffer in @param pszPath is too short, the path will be 
*   truncated and the return value is S_FALSE
*   <p>
*   @param pszPath - client allocated buffer, where the temp path will
*   be copieed to. 
*   <p>
*   @param SizeChars - Number of characters of the allocated buffer
*   <p>
*   @return - hr : S_OK : success, 
*             hr : S_FALSE - buffer too short or no temp path. 
*             hr : HR of system call ::GetTempPath.
*             hr : E_INVALIDARG - pszPath = NULL
*             hr : E_OUTOFMEMORY - memory allocation failed.
*   <p>
*   @exception - No
*   <p>
*   @see 
*   <p>
*/
UTILIF HRESULT UTIL_GetTempPath(OUT LPTSTR pszPath,size_t SizeChars);

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*   <B>UTIL_GetTempFileName</B>.
*
*   Description = Allocates a temporary file name. Returns the file path
*   in pszPath. If the allocated buffer in @param pszPath is too short, the path will be 
*   truncated and the return value is S_FALSE. @param pszPrefix identifies
*   the prefix of the temp file name (like 'CE' the generated temp files will
*   look like 'CE0815.tmp'). If @param pszPrefix is NULL a default prefix
*   '4C' is choosed.
*   <p>
*   @param pszPrefix - prefix for the temp file name - see description.
*   <p>
*   @param pszPath - client allocated buffer, where the temp file path will
*   be copieed to. 
*   <p>
*   @param SizeChars - Number of characters of the allocated buffer
*   <p>
*
*   @return - hr : S_OK : success, 
*             hr : S_FALSE - buffer too short or no temp path. 
*             hr : HR of system call ::GetTempFileName.
*             hr : E_INVALIDARG - pszPath = NULL
*             hr : E_OUTOFMEMORY - memory allocation failed.
*   <p>
*   @exception - No
*   <p>
*   @see 
*   <p>
*/
UTILIF HRESULT UTIL_GetTempFileName(IN LPCTSTR pszPrefix, OUT LPTSTR pszPath,IN size_t SizeChars);


//------------------------------------------------------------------*
/**
 * UTIL_XMLEncodeString
 *  encode special characters in string to XML format
 *
 * @param           sIn             string to encode
 * @param           psEncoded       encoded string
 * @param           encoding        character representationto use
 *                                  use BL_ENCODE_DEFAULT (= BL_ENCODE_UTF_8)
 * @return          
 * @exception       -
 * @see             
*/
UTILIF HRESULT
    UTIL_XMLEncodeString(BSTR sIn, BSTR *psEncoded, XML_CHAR_ENCODING encoding);


//------------------------------------------------------------------*
/**
 * UTIL_XMLDecodeString
 *  decode escaped special characters in string from XML format to binary format
 *
 * @param           sIn             string to decode
 * @param           psDecoded       decoded string
 * @param           encoding        character representationto use
 *                                  use BL_ENCODE_DEFAULT (= BL_ENCODE_UTF_8)
 * @return          
 * @exception       -
 * @see             
*/
UTILIF HRESULT 
    UTIL_XMLDecodeString(BSTR sIn, BSTR *psDecoded, XML_CHAR_ENCODING encoding);



#define UTIL_MAX_ID_LENGTH 32
//------------------------------------------------------------------*
/**
 * UTIL_CheckIdentifier
 * Checks for a valid IEC1131-3 Identifier, this means 
 * a) Syntax  identifier := (letter | ('_' (letter|digit))) {['_'] (letter|digit)}
 * b) identifier must not be one of the Keywords (ignoring the case). This function 
 *    checks also for all ATCMControl specific Keywords, e.g. NATIVE,SYNCHRONIZED,... .
 * c) the maximum length is checked, see the define UTIL_MAX_ID_LENGTH.
 *
 * @param           pszId   string to be checked for valid IEC1131-3 Identifier
 * @return                  One of the following:           meaning
 *                          S_OK                        valid identifier
 *
 *                          E_UTIL_ID_SYNTAX            the pure syntax is incorrect
 *                          E_UTIL_ID_IS_KEYWORD        syntax is ok, but pszId is a keyword
 *                          E_UTIL_ID_TOO_LONG          syntax is ok, but pszId is too long
 * @see 
 */
UTILIF HRESULT UTIL_CheckIdentifier(LPCTSTR pszId);


//------------------------------------------------------------------*
/**
 * UTIL_CheckIdentifierEx
 * Checks for a valid IEC1131-3 Identifier, this means 
 * a) Syntax  identifier := (letter | ('_' (letter|digit))) {['_'] (letter|digit)}
 * b) identifier must not be one of the Keywords (ignoring the case). This function 
 *    checks also for all ATCMControl specific Keywords, e.g. NATIVE,SYNCHRONIZED,... .
 * c) the maximum lenght is checked, see the define UTIL_MAX_ID_LENGTH.
 *
 * @param           pszId   string to be checked for valid IEC1131-3 Identifier
 * @param         pKwEnum   output: if pszId is a keyword *pKwEnum is set to 
 *                          the correponding enum, if pszId is no keyword 
 *                          *pKwEnum is set to KW_NO_KEYWORD. 
 *                          
 * @return                  One of the following:           meaning
 *                          S_OK                        valid identifier or keyword
 *
 *                          E_UTIL_ID_SYNTAX            the pure syntax is incorrect
 *                                                      this means that the underline '_'
 *                                                      was misused.
 *                          E_UTIL_ID_TOO_LONG          syntax is ok, but pszId is too long
 * @see 
 */
UTILIF HRESULT UTIL_CheckIdentifierEx(LPCTSTR pszId, UTIL_KW_ENUM* pKwEnum);


//------------------------------------------------------------------*
/**
 * UTIL_IsIECKeyword
 * Checks for IEC1131-3 Keyword including ATCMControl extensions
 * This function is case insensitive.
 * Note: If this function return UTIL_KW_NO_KEYWORD this does not 
 * imply that psz is a valid identifier.
 *
 * @param           psz     string to be checked
 * @return                  one enum of UTIL_KW_ENUM, especially 
 *                          UTIL_KW_NO_KEYWORD if no keyword.
 */
UTILIF UTIL_KW_ENUM UTIL_IsIECKeyword(LPCTSTR psz);

//------------------------------------------------------------------*
/**
 * UTIL_EnumToKeyword
 * Returns a const static pointer to the keyword for enum kw.
 * The keyword returned is uppercase.
 *
 * @param           kw     string to be checked
 * @return                 "<KEYWORD>" or the empty string if 
 *                         kw is UTIL_KW_NO_KEYWORD 
 *                         
 */
UTILIF LPCTSTR UTIL_EnumToKeyword(UTIL_KW_ENUM kw);



/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*   <B>UTIL_GetInstallLanguage</B>.
*
*   Description = Returns the id of the installed language.
*   <p>
*   @param - No
*   <p>
*
*   @return - LANGID - The language id of the installed language
*   <p>
*
*   @exception - No
*   <p>
*/
UTILIF LANGID UTIL_GetInstallLanguage(void);


//------------------------------------------------------------------*
/**
 * UTIL_FatalError
 *  display message box for unrecoverable application errors
 *
 * @param           pszDescription  description of circumstances
 * @param           hr              error code
 * @return          
 * @exception       -
 * @see             
*/
UTILIF void UTIL_FatalError (LPCTSTR pszDescription, HRESULT hr);


/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*   <B>retrieves the absolute standard (default) library path</B>.
*
*   Description = UTIL_GetStdLibPath retrieves the absolute 
*   standard (default) library path. The function stores the path in 
*   the passed @param pszPath.
*   <p>
*   @param pszPath - string pointer where the install path will be
*   <p>
*   @param SizeChars - size of the buffer receiving the path
*   <p>
*
*	@return hr = E_INVALIDARG if the param is Null
*	@return hr = E_OUTOFMEMORY a memory allocation operation fails
*	@return hr = S_FALSE registry key or value not found
*	@return hr = E_FAIL	- memory size to small (SizeChars)
*	@return hr = S_OK - Success
*
*   @return - 
*   <p>
*   @return - 
*   <p>
*
*
*   @exception - No
*   <p>
*   @see - UTIL_GetInstallPath
*   <p>
*/
UTILIF HRESULT UTIL_GetStdLibPath(IN LPTSTR pszPath,IN size_t SizeChars);

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*   <B>retrieves the absolute standard (default) project path</B>.
*
*   Description = UTIL_GetStdProjectPath retrieves the absolute 
*   standard (default) project path. The function stores the path in 
*   the passed @param pszPath.
*   <p>
*   @param pszPath - string pointer where the install path will be
*   <p>
*   @param SizeChars - size of the buffer receiving the path
*   <p>
*
*	@return hr = E_INVALIDARG if the param is Null
*	@return hr = E_OUTOFMEMORY a memory allocation operation fails
*	@return hr = S_FALSE registry key or value not found
*	@return hr = E_FAIL	- memory size to small (SizeChars)
*	@return hr = S_OK - Success
*
*   @return - 
*   <p>
*   @return - 
*   <p>
*
*
*   @exception - No
*   <p>
*   @see - UTIL_GetInstallPath
*   <p>
*/
UTILIF HRESULT UTIL_GetStdProjectPath(IN LPTSTR pszPath,IN size_t SizeChars);

//------------------------------------------------------------------*
/**
 * UTIL_GetOS_Version
 *  retrieves a enumerator that describes to Windows OS version currently running
 *
 * @param           void
 * @return          enumerator describing present OS
 * @exception       -
 * @see             
*/
UTILIF OS_ENV
    UTIL_GetOS_Version(void);

UTILIF HRESULT UTIL_GetVersionString(UTIL_VERSION_SPEC spec, LPTSTR pszVersion,size_t SizeChars);



//------------------------------------------------------------------*
/**
 * %function.
 * @param			HINSTANCE hResHandle
 * @param			LPTSTR pszOut
 * @param			size_t sizeBuf
 * @param			HRESULT hr
 * @param			...
 * @return			HRESULT 
 * @exception			
 * @see			
*/
UTILIF HRESULT UTIL_FormatMessage(HINSTANCE hResHandle,LPTSTR pszOut,size_t sizeBuf,HRESULT hr,...);


//------------------------------------------------------------------*
/**
 * UTIL_BuildControlString()
 *
 *  Build a control string used for communication purposes
 *  control string is built up like this:
 *  "<OPCServer>,<CustomConnectGuid>,<CommChannel>(<ConnectParameters>)[,VERSION(<FirmwareVersion>)]"
 *      where:
 *      <OPCServer>: OPC-Server address
 *      <CustomConnectGuid>: GUID of custom connect interface
 *      <CommChannel>: communication channel "TCP" or "RS232" ...
 *      <ConnectParameters>: connect parameter string depending on communication channel
 *      <FirmwareVersion>: internal firmware version string (optional)
 *
 *  ****************** caution *******************
 *  * minimum buffer sizes are noted in          *
 *  * square brackets after each parameter !!    *
 *  **********************************************
 *  
 * @param           pszOPCServer: OPC-Server address
 * @param           pszCustomConnectGuid: custom connect guid
 * @param           pszOPCInstName: OPC-Server instance name used for OPC server identification
 * @param           pszCommChannel: communication channel
 * @param           pszConnectPar: connect parameter string
 * @param           pszProtocol: protocol string
 * @param           pszProtocolPar: protocol parameter string
 * @param           pszFirmwareVersion: firmware version string (may be NULL)
 * @param           pszControlString: builded control string (output) [UTIL_MAX_CONTROL_STRING_LEN]
 * @return          S_OK: ok
 *                  E_INVALIDARG: required argument was invalid (NULL)
 *                  E_FAIL: buffer too small
 * @exception       -
 * @see             -
*/
UTILIF HRESULT UTIL_BuildControlString(
    IN LPCTSTR pszOPCServer, 
    IN LPCTSTR pszCustomConnectGuid,
    IN LPCTSTR pszOPCInstName,
    IN LPCTSTR pszCommChannel, 
    IN LPCTSTR pszConnectPar,
    IN LPCTSTR pszProtocol,
    IN LPCTSTR pszProtocolPar,
    IN LPCTSTR pszFirmwareVersion,
    LPTSTR pszControlString);


//------------------------------------------------------------------*
/**
 * UTIL_ParseControlString()
 *
 *  Parse a control string used for communication purposes
 *  Uses UTIL_ParseTargetControlString()
 *  Any parameter may be NULL if not required
 *  ****************** caution *******************
 *  * minimum buffer sizes are noted in          *
 *  * square brackets after each parameter !!    *
 *  **********************************************
 *
 * @param           pszControlString: control string to be parsed (input)
 * @param           pszOPCServer: OPC-Server address (output) [UTIL_MAX_OPC_SERVER_LEN]
 * @param           pszCustomConnectGuid: custom connect guid [UTIL_MAX_GUID_LEN]
 * @param           pszOPCInstName: OPC-Server instance name [UTIL_MAX_OPC_INST_NAME_LEN]
 * @param           pszCommChannel: communication channel [UTIL_MAX_COMM_CHANNEL_LEN]
 * @param           pszConnectPar: connect parameter string [UTIL_MAX_CONNECT_PAR_LEN]
 * @param           pszProtocol: protocol string [UTIL_MAX_PROTOCOL_LEN]
 * @param           pszProtocolPar: protocol parameter string [UTIL_MAX_PROTOCOL_PAR_LEN]
 * @param           pszFirmwareVersion: firmware version string [UTIL_MAX_FIRMWARE_VERSION_LEN]
 * @return          S_OK: ok
 *                  E_FAIL: syntax error or missing fields
 *                  E_INVALIDARG: required argument was invalid (NULL)
 * @exception       -
 * @see             UTIL_BuildControlString()
*/
UTILIF HRESULT UTIL_ParseControlString(
    IN LPCTSTR pszControlString,
    LPTSTR pszOPCServer, 
    LPTSTR pszCustomConnectGuid, 
    LPTSTR pszOPCInstName,
    LPTSTR pszCommChannel, 
    LPTSTR pszConnectPar,
    LPTSTR pszProtocol,
    LPTSTR pszProtocolPar,
    LPTSTR pszFirmwareVersion);

//------------------------------------------------------------------*
/**
 * UTIL_ParseTargetControlString()
 *
 *  Parse target portion of a control string used for communication purposes
 *  see also UTIL_ParseControlString() and UTIL_BuildControlString()
 *  Any parameter may be NULL if not required
 *  ****************** caution *******************
 *  * minimum buffer sizes are noted in          *
 *  * square brackets after each parameter !!    *
 *  **********************************************
 *
 * @param           pszTargetControlString: target control string to be parsed (input)
 * @param           pszOPCInstName: OPC-Server instance name [UTIL_MAX_OPC_INST_NAME_LEN]
 * @param           pszCommChannel: communication channel [UTIL_MAX_COMM_CHANNEL_LEN]
 * @param           pszConnectPar: connect parameter string [UTIL_MAX_CONNECT_PAR_LEN]
 * @param           pszProtocol: protocol string [UTIL_MAX_PROTOCOL_LEN]
 * @param           pszProtocolPar: protocol parameter string [UTIL_MAX_PROTOCOL_PAR_LEN]
 * @param           pszFirmwareVersion: firmware version string [UTIL_MAX_FIRMWARE_VERSION_LEN]
 * @return          S_OK: ok
 *                  E_FAIL: syntax error or missing fields
 *                  E_INVALIDARG: required argument was invalid (NULL)
 * @exception       -
 * @see             UTIL_ParseControlString(), UTIL_BuildControlString()
*/
UTILIF HRESULT UTIL_ParseTargetControlString(
    IN LPCTSTR pszTargetControlString,
    LPTSTR pszOPCInstName,
    LPTSTR pszCommChannel, 
    LPTSTR pszConnectPar,
    LPTSTR pszProtocol,
    LPTSTR pszProtocolPar,
    LPTSTR pszFirmwareVersion);


//------------------------------------------------------------------*
/**
 * encrypt string
 *
 *  ensure that buffer size of pszEncrypted is at least 2*(strlen(pszToEncrypt) + 8)!
 *
 * @param           pszToEncrypt: string to be encrypted (must be null terminated!)
 * @param           pszEncrypted: encrypted version of string (buffer size see above)
 * @param           pszKey: 16 character encryption key (if NULL the UTIL standard key is used)
 * @return          HRESULT
 * @exception       -
 * @see             UTIL_DecryptString()
*/
UTILIF HRESULT UTIL_EncryptString(IN LPCTSTR pszToEncrypt, OUT LPTSTR pszEncrypted, IN LPCTSTR pszKey);

//------------------------------------------------------------------*
/**
 * decrypt string
 *
 *  ensure that pszDecrypted is at least as big as strlen(pszToDecrypt)/2 + 1
 *
 * @param           pszToDecrypt: string to be decrypted (must be null terminated!)
 * @param           pszDecrypted: decrypted string (buffer size big enough, see above!)
 * @param           pszKey: 16 character encryption key (if NULL the UTIL standard key is used)
 * @return          HRESULT
 * @exception       -
 * @see             UTIL_EncryptString()
*/
UTILIF HRESULT UTIL_DecryptString(IN LPCTSTR pszToDecrypt, OUT LPTSTR pszDecrypted, IN LPCTSTR pszKey);


//------------------------------------------------------------------*
/**
 * calculate fingerprint for string
 *
 *  ensure that buffer size of pszOutput is at least 33
 *
 * @param           pszInput: string for which fingerprint is to be calculated
 * @param           pszOutput: 32 character fingerprint string + trailing 0
 * @return          S_OK
 * @exception       -
 * @see             -
*/
UTILIF HRESULT UTIL_GetStringFingerprint(IN LPCTSTR pszInput, OUT LPTSTR pszOutput);

//------------------------------------------------------------------*
/**
 * convert byte array to hexadecimal string.
 *
 *  ensure that buffer size of pszHexString is at least 2*lSize
 *
 * @param           lpByte: byte array
 * @param           lSize: size of byte array
 * @param           pszHexString: hexadecimal string (buffer size see above)
 * @return          S_OK
 * @exception       -
 * @see             UTIL_HexStringToByteArray()
*/
UTILIF HRESULT UTIL_ByteArrayToHexString(IN LPBYTE lpByte, IN LONG lSize, OUT LPTSTR pszHexString);

//------------------------------------------------------------------*
/**
 * convert hexadecimal string to byte array.
 *
 *  ensure that size of lpByte is at least strlen(pszHexString) / 2 + 1
 *
 * @param           pszHexString: hexadecimal string
 * @param           lpByte: byte array (size see above)
 * @return          S_OK: ok
 *                  E_FAIL: strlen(pszHexString) is odd or non hex digit encountered
 * @exception       -
 * @see             UTIL_HexStringToByteArray()
*/
UTILIF HRESULT UTIL_HexStringToByteArray(IN LPCTSTR pszHexString, OUT LPBYTE lpByte);

//------------------------------------------------------------------*
/**
 * UTIL_XML_EscapeSpecialCharacters
 *
 *  replace special XML characters by their escape sequence
 *  special characters are '&', '<', '>', ''', '"', TAB, NEWLINE and RETURN
 *
 * @param           lpszString: input string
 * @param           lpszOut: string with escaped characters
 * @param           SizeChars: size of lpszOut
 *                             to be shure that buffer is big enough
 *                             SizeChars has to be 6*strlen(lpszOut) + 1
 * @return          successful?
 * @exception       -
 * @see             -
*/
UTILIF BOOL UTIL_XML_EscapeSpecialCharacters(LPCTSTR lpszString, LPTSTR lpszOut, size_t SizeChars);

//------------------------------------------------------------------*
/**
 * UTIL_String2UTF8
 *
 *  transforms an ASCII string into UTF-8 format
 *  the string is split in parts and each part is
 *  translated into unicode and from unicode to UTF-8
 *  if one translation step fails or the output buffer
 *  size is too small the return value is FALSE.
 *
 *  Attention:
 *    UTF-8 encodes one character with up to 3 characters
 *    escape sequence for one character may be up to 6 characters
 *
 * @param           lpszStringIn: input string
 * @param           lpszOut: output string (for size see just below)
 * @param           SizeChars: size of lpszOut
 *                             to be shure that buffer is big enough
 *                             SizeChars has to be 6*strlen(lpszOut) + 1
 * @return          successful?
 * @exception       -
 * @see             -
*/
UTILIF BOOL UTIL_String2UTF8(LPCTSTR lpszString, LPTSTR lpszOut, size_t SizeChars);



#ifdef	__cplusplus
}
#endif	//__cplusplus


#endif // __UTILIF_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: utilif.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 4  *****************
 * User: Sis          Date: 1.10.03    Time: 14:55
 * Updated in $/4Control/COM/softing/fc/inc
 * CString not allowed in header file (2)
 * 
 * *****************  Version 3  *****************
 * User: Sis          Date: 1.10.03    Time: 9:48
 * Updated in $/4Control/COM/softing/fc/inc
 * bug fix: may not use CString in header
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 29.09.03   Time: 14:08
 * Updated in $/4Control/COM/softing/fc/inc
 * bug fix d-2109: UTF-8 problems
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 10  *****************
 * User: Sis          Date: 7.08.02    Time: 13:52
 * Updated in $/4Control/COM/softing/fc/inc
 * crypto + fingerprint
 * 
 * *****************  Version 9  *****************
 * User: Sis          Date: 13.05.02   Time: 15:44
 * Updated in $/4Control/COM/softing/fc/inc
 * control string: added parameter pszOPCInstName
 * 
 * *****************  Version 8  *****************
 * User: Sis          Date: 3.05.02    Time: 11:52
 * Updated in $/4Control/COM/softing/fc/inc
 * new parameter in UTIL_BuildControlString(), UTIL_ParseControlString()
 * 
 * *****************  Version 7  *****************
 * User: Sis          Date: 26.04.02   Time: 16:37
 * Updated in $/4Control/COM/softing/fc/inc
 * control string functionality
 * 
 * *****************  Version 6  *****************
 * User: Sis          Date: 15.01.02   Time: 11:11
 * Updated in $/4Control/COM/softing/fc/inc
 * UTIL_TouchFile()
 * 
 * *****************  Version 5  *****************
 * User: Sis          Date: 22.10.01   Time: 17:06
 * Updated in $/4Control/COM/softing/fc/inc
 * UTIL_GetMasterPassword()
 * 
 * *****************  Version 4  *****************
 * User: Sis          Date: 30.07.01   Time: 17:02
 * Updated in $/4Control/COM/softing/fc/Inc
 * implemented UTIL_GetStdProjectPath()
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 10.04.01   Time: 12:35
 * Updated in $/4Control/COM/softing/fc/inc
 * UTIL_FormatMessage(), 
 * 
 * *****************  Version 2  *****************
 * User: Jd           Date: 8.03.01    Time: 11:26
 * Updated in $/4Control/COM/softing/fc/inc
 * Changed registry installation path to V2.0
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 23.01.01   Time: 17:09
 * Created in $/4Control/COM/softing/fc/inc
 * utils and baselib from V1.3
 *==
 *----------------------------------------------------------------------------*
*/

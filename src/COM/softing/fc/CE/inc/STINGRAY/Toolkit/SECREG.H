// Stingray Software Extension Classes
// Copyright (C) 1995 Stingray Software Inc,
// All rights reserved
//   		
// CHANGELOG:
// 7/15/95 : Original Implementation based on wfc by Samuel Blackburn
// 7/20/95 : extended for WIN16 APIs
// 9/30/95 : Code review MSW
// 2/27/98 : Overloaded enumeration methods, enumeration fix

#ifndef __SECREG_H__
#define __SECREG_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif
 
#ifdef WIN32
#ifndef _WINREG_
#include "winreg.h"
#endif
#endif

#ifdef _SECDLL
 #undef AFXAPP_DATA
 #define AFXAPP_DATA AFXAPI_DATA
 #undef AFX_DATA
 #define AFX_DATA SEC_DATAEXT
#endif

// defines for Win16 compatibility and convenience
#define THIS_SUB_KEY FALSE
#define ALL_SUB_KEYS TRUE

#define SIGNAL_EVENT	TRUE
#define WAIT_FOR_CHANGE	FALSE

#ifndef WIN32
#define ERROR_INVALID_HANDLE 100
#endif

#ifndef WIN32
#define SECPBYTE LPTSTR
#else
#define SECPBYTE LPBYTE
#endif


/////////////////////////////
// AutoDuck tag block block for SECRegistry 

//@doc SECRegistry
//@mdata LONG  | SECRegistry | m_lErrorCode | The error code for the last 
// operation.
//@xref <c SECRegistry>

//@doc SECRegistry
//@mdata HKEY  | SECRegistry | m_hKey | The handle to the currently open 
// registry key.
//@xref <c SECRegistry>

//@doc SECRegistry
//@mdata HKEY  | SECRegistry | m_hRegistry | The handle to the current 
// registry.
//@xref <c SECRegistry> <mf SECRegistry::Connect>

//@doc SECRegistry
//@mdata DWORD  | SECRegistry | m_dwLongestSubkeyNameLength | The handle 
// to the currently open registry key.
//@xref <c SECRegistry> <mf SECRegistry::Open>

//@doc SECRegistry
//@mdata DWORD  | SECRegistry | m_dwLongestClassNameLength | The longest 
// subkey name length of last queried key.
//@xref <c SECRegistry> <mf SECRegistry::QueryInfo>

//@doc SECRegistry
//@mdata DWORD  | SECRegistry | m_dwLongestValueNameLength | The longest 
// value name length of last queried key.
//@xref <c SECRegistry> <mf SECRegistry::QueryInfo>

//@doc SECRegistry
//@mdata DWORD  | SECRegistry | m_dwLongestValueDataLength | The longest 
// value data length of last queried key.
//@xref <c SECRegistry> <mf SECRegistry::QueryInfo>

//@doc SECRegistry
//@mdata DWORD  | SECRegistry | m_dwSecurityDescriptorLength | The longest 
// security descriptor length of last queried key.
//@xref <c SECRegistry> <mf SECRegistry::QueryInfo>

//@doc SECRegistry
//@mdata FILETIME  | SECRegistry | m_fileTimeLastWrite | The time the last 
// queried key was written to.
//@xref <c SECRegistry> <mf SECRegistry::QueryInfo>

//@doc SECRegistry
//@mdata CString  | SECRegistry | m_strClassName | The class name of the 
// currently open key.
//@xref <c SECRegistry>

//@doc SECRegistry
//@mdata CString  | SECRegistry | m_strComputerName | The computer name for 
// the currently connected registry.
//@xref <c SECRegistry>

//@doc SECRegistry
//@mdata CString  | SECRegistry | m_strKeyName | The name of the currently 
// open key.
//@xref <c SECRegistry>

//@doc SECRegistry
//@mdata CString  | SECRegistry | m_strRegistryName | The name of the 
// currently connected registry.
//@xref <c SECRegistry>

//@doc SECRegistry
//@mdata DWORD    | SECRegistry | m_dwNumberOfSubkeys | The number of 
// subkeys of the last queried key.
//@xref <c SECRegistry> <mf SECRegistry::QueryInfo>

//@doc SECRegistry
//@mdata DWORD    | SECRegistry | m_dwNumberOfValues | The number of 
// values in the last queried key.
//@xref <c SECRegistry> <mf SECRegistry::QueryInfo>

//@doc SECRegistry
//@mdata CTime    | SECRegistry | m_timeLastWrite | The time the last 
// queried key was written to.
//@xref <c SECRegistry> <mf SECRegistry::QueryInfo>

//@doc SECRegistry
//@mdata BOOL     | SECRegistry | m_bRemote | Indicates whether the registry 
// is local or on a remote system.
//@xref <c SECRegistry> <mf SECRegistry::Connect>

//@doc SECRegistry
//@mdata BOOL     | SECRegistry | m_bCloseKeyOnDisconnect | Incidates 
// whether the registry key should be closed when done.
//@xref <c SECRegistry> <mf SECRegistry::Connect>

// adinsert AutoDuck insertion point for SECRegistry 
//@doc SECRegistry 
//@class SECRegistry encapsulates the APIs used to access and modify the 
// Windows registry in a single class.  The bulk of the SECRegistry 
// member functions pertain to the Win32 API; and a smaller number are 
// used in a Win16 environment.
//
// See the REGDEMO sample in the \SAMPLES\TOOLKIT\STANDARD\UTILITY\REGDEMO directory for a 
// demonstration of this class.
//@base public | CObject
class SECRegistry : public CObject
{
	DECLARE_DYNAMIC( SECRegistry );

// Enumerations for SECRegistry
public:
#ifdef WIN32	


	//@devnote
	// The following static data members were not accessed
	// correctly when compiled as an extension dll and have
	// been removed from this class.  Please use the corresponding 
	// constants defined in WINREG.H:
	//
	//static HKEY hKeyLocalMachine;		//	HKEY_LOCAL_MACHINE<nl>
	//static HKEY hKeyClassesRoot;		//	HKEY_CLASSES_ROOT<nl>
	//static HKEY hKeyUsers;			//	HKEY_USERS<nl>
	//static HKEY hKeyCurrentUser;		//	HKEY_CURRENT_USER<nl>
	
	//@cmember,menum
	/* Creation dispositions*/
	enum CreationDisposition
	{
	dispositionCreatedNewKey	 = REG_CREATED_NEW_KEY,		//@@emem New Registry Key created
	dispositionOpenedExistingKey = REG_OPENED_EXISTING_KEY	//@@emem Existing Key opened
	} ;
	
	//@cmember,menum
	/* Create options*/
	enum CreateOptions
	{
	optionsNonVolatile =	REG_OPTION_NON_VOLATILE,		//@@emem Key is preserved when system is rebooted
	optionsVolatile	=		REG_OPTION_VOLATILE				//@@emem Key is not preserved when system is rebooted
	} ;
	
	//@cmember,menum
	/* Create Permissions*/
	enum CreatePermissions
	{
	permissionAllAccess			= KEY_ALL_ACCESS,			//@@emem All key access permissions.
	permissionCreateLink		= KEY_CREATE_LINK,			//@@emem Permission to create symbolic links.
	permissionCreateSubKey		= KEY_CREATE_SUB_KEY,		//@@emem Permission to create subkeys.
	permissionEnumerateSubKeys	= KEY_ENUMERATE_SUB_KEYS,	//@@emem Permission to enumerate subkeys
	permissionExecute			= KEY_EXECUTE,				//@@emem Permission to execute.
	permissionNotify			= KEY_NOTIFY,				//@@emem Permission to notify.
	permissionQueryValue		= KEY_QUERY_VALUE,			//@@emem Permission to query values.
	permissionRead				= KEY_READ,					//@@emem Permission to read.
	permissionSetValue			= KEY_SET_VALUE,			//@@emem Permission to set values.
	permissionWrite				= KEY_WRITE					//@@emem Permission to write.
	} ;

	//@cmember,menum
	/* Key value types*/
	enum KeyValueTypes
	{
	typeBinary					= REG_BINARY,				//@@emem Free form binary data (32-bit only)
	typeDoubleWord				= REG_DWORD,				//@@emem 32-bit number (32-bit only)
	typeDoubleWordLittleEndian	= REG_DWORD_LITTLE_ENDIAN,	//@@emem 32-bit number (same as REG_DWORD) (32-bit only)
	typeDoubleWordBigEndian		= REG_DWORD_BIG_ENDIAN,		//@@emem 32-bit number (32-bit only)
	typeUnexpandedString		= REG_EXPAND_SZ,			//@@emem Unicode nul terminated string (32-bit only)
	typeSymbolicLink			= REG_LINK,					//@@emem Symbolic Link (unicode) (32-bit only)
	typeMultipleString			= REG_MULTI_SZ,				//@@emem Multiple Unicode strings (32-bit only)
	typeNone					= REG_NONE,					//@@emem No value type (32-bit only)
	typeResourceList			= REG_RESOURCE_LIST,		//@@emem Resource list in the resource map (32-bit only)
	typeString					= REG_SZ					//@@emem Unicode nul terminated string
	};
	
	//@cmember,menum
	/* Notify change filters*/
	enum NotifyChangeFilter
	{
	notifyName					= REG_NOTIFY_CHANGE_NAME,		//@@emem Child created or deleted
	notifyAttributes			= REG_NOTIFY_CHANGE_ATTRIBUTES,	//@@emem Changed attributes
	notifyLastSet				= REG_NOTIFY_CHANGE_LAST_SET,	//@@emem Changed time stamp
	notifySecurity				= REG_NOTIFY_CHANGE_SECURITY	//@@emem Changed security
	};
	
	enum NotifyChangeFlag
	{
	changeKeyAndSubkeys			= TRUE,		//@@emem Notify on key or subkey changes
	changeSpecifiedKeyOnly		= FALSE		//@@emem Notify on specified key change only
	};
	
#else // WIN16 only supports string key type.
	enum KeyValueTypes
	{
	typeString					= REG_SZ
	};
	
#endif
	

	//@access Creation/Initialization

public:

	//@cmember
	/* Constructs a SECRegistry object.*/
	SECRegistry();

protected:

	//@cmember
	/* Initializes data members.*/
	virtual void Initialize( void );

public:

#ifdef WIN32
	//@cmember
	/* Creates key in registry (32-bit only).*/
	virtual BOOL Create( LPCTSTR	lpszSubkeyName,
			 LPCTSTR				name_of_class		= NULL,
			 CreateOptions			options				= optionsNonVolatile,
			 CreatePermissions		permissions			= permissionAllAccess,
			 LPSECURITY_ATTRIBUTES	pSecurityAttributes	= NULL,
			 CreationDisposition *	pDisposition		= NULL 
			 );
#else
	//@cmember
	/* Creates key in registry (16-bit only).*/
	virtual BOOL Create( LPCTSTR	name_of_subkey,
			LPCTSTR					name_of_class		= NULL
			 );
#endif
	
// Attributes and enumerations for SECRegistry
	
	
	//@access Operations
public:

#ifdef WIN32
	//@cmember
	/* Closes the current key (16 and 32-bit).*/
	virtual BOOL Close( void );
#else	
	virtual BOOL Close( void );
#endif //WIN32
	
#ifdef WIN32

	//@cmember
	/* Establishes a connection with the registry (32-bit only).*/

	virtual BOOL Connect(HKEY	hKeyToOpen	= HKEY_CURRENT_USER,
				LPCTSTR	lpszComputerName	= NULL,
				BOOL bCloseKeyOnDisconnect = TRUE);

	
	//@cmember
	/* Deletes the specified key from the registry (16 and 32-bit).*/
	virtual BOOL DeleteKey( LPCTSTR lpszKeyToDelete, BOOL bRecursive = FALSE);
#else
	virtual BOOL DeleteKey( LPCTSTR lpszKeyToDelete);	

#endif //WIN32

#ifdef WIN32

	//@cmember
	/* Deletes the named registry value of the current key (32-bit only).*/
	virtual BOOL DeleteValue( LPCTSTR lpszValueToDelete);
	
	//@cmember
	/* Enumerates subkeys of the currently open key (32-bit only).*/
	virtual BOOL EnumerateKeys( const DWORD dwSubkeyIndex,
				CString&	strSubkeyName);

	//@cmember
	/* Enumerates subkeys of the currently open key (32-bit only).*/
	virtual BOOL EnumerateKeys( const DWORD dwSubkeyIndex,
				CString&	strSubkeyName,
				CString&	strClassName );
	
	//@cmember
	/* Enumerates subkeys of the currently open key (32-bit only).*/
	virtual BOOL EnumerateKeys(const DWORD dwSubkeyIndex,
				LPTSTR	lpszSubkeyName,
				LPDWORD	lpdwSubkeyNameSize,
				LPTSTR	lpszClassName		= NULL,
				LPDWORD	lpdwClassNameSize	= NULL);

#else 

	//@cmember
	/* Enumerates subkeys of the currently open key (16-bit only).*/
	virtual BOOL EnumerateKeys( const DWORD dwIndex,
				LPTSTR	   lpszBuffer,
				DWORD	   dwBufferSize);
#endif //WIN32
	
#ifdef WIN32


	//@cmember
	/* Enumerates values of the currently open key (32-bit only).*/
	virtual BOOL EnumerateValues( const DWORD dwValueIndex,
				CString&		strValueName,
				KeyValueTypes*	pTypeCode			= NULL,
				LPBYTE			lpbDataBuffer		= NULL,
				LPDWORD			lpdwSizeDataBuffer	= NULL);

	//@cmember
	/* Enumerates the values of the currently open key (32-bit only).*/
	virtual BOOL EnumerateValues( const DWORD dwValueIndex,
				CString&		strValueName,
				KeyValueTypes&	type_code );

	//@cmember
	/* Enumerates the values of the currently open key (32-bit only).*/
	virtual BOOL EnumerateValues( const DWORD	dwValueIndex,
				CString&			strValueName,
				KeyValueTypes&	type_code,
				LPBYTE			lpbDataBuffer,
				DWORD&			dwSizeDataBuffer );
	
	//@cmember
	/* Writes the attributes of the currently open key.*/
	virtual BOOL Flush( void );
	
	//@cmember
	/* Retrieves a REG_BINARY value (32-bit only).*/
	virtual BOOL GetBinaryValue( LPCTSTR lpszValueName, CByteArray& return_array );
	
	//@cmember
	/* Retrieves a REG_DWORD value (32-bit only).*/
	virtual BOOL GetDoubleWordValue( LPCTSTR lpszValueName, DWORD& dwReturnValue );
	
	//@cmember
	/* Encapsulates the RegGetSecurity() call (32-bit only).*/
	virtual BOOL GetSecurity( const SECURITY_INFORMATION security_info,
				PSECURITY_DESCRIPTOR	data_buffer,
				DWORD&					dwSizeDataBuffer);
	
	//@cmember
	/* Retrieves a REG_SZ value (32-bit only).*/
	virtual BOOL GetStringValue( LPCTSTR lpszValueName,
				CString& strReturn );
	
	//@cmember
	/* Retrieves a REG_MULTI_SZ value (32-bit only).*/
	virtual BOOL GetStringArrayValue( LPCTSTR name_of_value,
				CStringArray& return_array );
#else

	//@cmember
	/* Determines the number of subkeys in the curently open key.*/
	virtual BOOL GetSubkeys(LPCTSTR lpszBuffer, UINT& nKeys);	
	
#endif //WIN32

#ifdef WIN32

	//@cmember
	/* Retrieves a REG_BINARY value (32-bit only).*/
	virtual BOOL GetValue( LPCTSTR lpszValueName,
				CByteArray& return_array );

	//@cmember
	/* Retrieves a REG_DWORD value (32-bit only).*/
	virtual BOOL GetValue( LPCTSTR lpszValueName,
				DWORD& dwReturnValue );

	//@cmember
	/* Retrieves a REG_MULTI_SZ value (32-bit only).*/
	virtual BOOL GetValue( LPCTSTR lpszValueName,
				CStringArray& return_array );

	//@cmember
	/* Retrieves a REG_SZ value (32-bit only).*/
	virtual BOOL GetValue( LPCTSTR lpszValueName,
				CString& strReturn );
	
	//@cmember
	/* Loads a registry key from a file (32-bit only).*/
	virtual BOOL Load( LPCTSTR lpszSubkeyName,
				LPCTSTR lpszFileName );
	
	//@cmember
	/* Indicates when a registry key has changed (32-bit only).*/

	virtual BOOL NotifyChange( const HANDLE hEvent				= NULL,
				const NotifyChangeFilter changes_to_be_reported	= notifyLastSet,
				const BOOL bAllSubkeys							= changeSpecifiedKeyOnly,
				const BOOL bWaitForChange						= WAIT_FOR_CHANGE );
	
	//@cmember
	/* Opens a registry key (32-bit only).*/
	virtual BOOL Open( LPCTSTR lpszSubkey,
			   const CreatePermissions security_access_mask		= permissionAllAccess );
	
#else 

	//@cmember
	/* Opens the specified key (32-bit only).*/
	virtual BOOL Open( LPCTSTR lpszSubkey);

#endif //WIN32

#ifdef WIN32

	//@cmember
	/* Queries for information about the currently opened key (32-bit only).*/
	virtual BOOL QueryInfo( void );

	//@cmember
	/* Retrieves the type and data for a specified value name (32-bit only).*/
	virtual BOOL QueryValue( LPCTSTR	lpszValueName,
				KeyValueTypes&		value_type,
				LPBYTE				lpbBuffer,
				DWORD&				dwBufferSize);
	
#else 

	//@cmember
	/* Retrieves the type and data for a specified value name (16-bit only).*/
	virtual BOOL QueryValue( LPCTSTR  lpSubKey,
				SECPBYTE	lpbBuffer,
				LONG&		lBufferSize);
#endif //WIN32
	
#ifdef WIN32

	//@cmember
	/* Encapsulates the RegReplaceKey API (32-bit only).*/
	virtual BOOL Replace( LPCTSTR lpszSubkeyName,
				LPCTSTR	lpszNewFile,
				LPCTSTR	lpszBackupFile);
	
	//@cmember
	/* Encapsulates the RegRestoreKey API (32-bit only).*/
	virtual BOOL Restore( LPCTSTR lpszSavedTreeFile,
				const DWORD dwVolatilityFlags = NULL );
	
	//@cmember
	/* Encapsulates the RegSaveKey API (32-bit only).*/
	virtual BOOL Save( LPCTSTR lpszDestFile,
			   LPSECURITY_ATTRIBUTES pSecurityAttributes = NULL );
	
	//@cmember
	/* Sets a value for the key name specified as REG_BINARY (32-bit only).*/
	virtual BOOL SetBinaryValue( LPCTSTR lpszValueName,
				 const CByteArray& bytes_to_write );
	
	//@cmember
	/* Sets a value for the key name specified as REG_DWORD (32-bit only).*/
	virtual BOOL SetDoubleWordValue( LPCTSTR lpszValueName,
				DWORD dwValue );
	
	//@cmember
	/* Encapsulates the RegSetKeySecurity API (32-bit only).*/
	virtual BOOL SetSecurity( const SECURITY_INFORMATION& SecurityInformation,
				const PSECURITY_DESCRIPTOR pSecurityDescriptor );
	
	//@cmember
	/* Sets a value for the key name specified as REG_SZ (32-bit only).*/
	virtual BOOL SetStringValue( LPCTSTR lpszValueName,
				const CString& string_value );
	
	//@cmember
	/* Sets a value for the key name specified as REG_MULTI_SZ (32-bit only).*/
	virtual BOOL SetStringArrayValue( LPCTSTR lpszValueName,
				const CStringArray& string_array );
	
	//@cmember
	/* Sets a value for the key name specified as REG_BINARY (32-bit only).*/
	virtual BOOL SetValue( LPCTSTR lpszValueName,
				const CByteArray& bytes_to_write );

	//@cmember
	/* Sets a value for the key name specified as REG_DWORD (32-bit only).*/
	virtual BOOL SetValue( LPCTSTR lpszValueName, DWORD dwValue );

	//@cmember
	/* Sets a value for the key name specified as REG_MULTI_SZ (32-bit only).*/
	virtual BOOL SetValue( LPCTSTR lpszValueName,
				const CStringArray& strings_to_write );
	
	//@cmember
	/* Sets a value for the key name specified as REG_SZ (32-bit only).*/
	virtual BOOL SetValue( LPCTSTR lpszValueName,
				const CString& strWrite );
	
	//@cmember
	/* Sets a value for the key name specified (32-bit only).*/
	virtual BOOL SetValue( LPCTSTR		lpszValueName,
				const KeyValueTypes		type_of_value_to_set,
				LPBYTE					lpbValueData,
				const DWORD				dwSize );

#else

	//@cmember
	/* Sets a value for the key name specified (16-bit only).*/
	virtual BOOL SetValue( LPCTSTR lpSubKey,
			   const CString& strWrite );

#endif //WIN32

#ifdef WIN32

	//@cmember
	/* Encapsulates the RegUnLoadKey API.*/
	virtual BOOL UnLoad( LPCTSTR lpszSubkey );
	// 
	//@cmember
	/* Converts an HKEY constant to its string equivalent.*/
	static BOOL KeyToStr(HKEY hKey,CString& strKey);
	
	// 
	//@cmember
	/* Converts a string equivalent to an HKEY value.*/
	static HKEY StrToKey(const CString& strKey);
	
#endif


	// Normalizes a keyname
	static void NormalizeKey(CString& strKey, BOOL bSubkey = FALSE);

	// Concatenates a key with a subkey
	static CString ConcatenateKeys (LPCTSTR strKey, LPCTSTR strSubkey);

// Implementation	
public:
	virtual ~SECRegistry();
	
	//@cmember
	/* The error code for the last operation.*/
	LONG m_lErrorCode;

	//@cmember
	/* The handle to the currently open registry key.*/
	HKEY m_hKey;

	//@cmember
	/* The handle to the current registry.*/
	HKEY m_hRegistry;
	
#ifdef WIN32

	//@cmember
	/* The longest subkey name length of last queried key.*/
	DWORD	m_dwLongestSubkeyNameLength;

	//@cmember
	/* The longest class name length of last queried key.*/
	DWORD	m_dwLongestClassNameLength;
	
	//@cmember
	/* The longest value name length of last queried key.*/
	DWORD	m_dwLongestValueNameLength;
	
	//@cmember
	/* The longest value data length of last queried key.*/
	DWORD	m_dwLongestValueDataLength;
	
	//@cmember
	/* The longest security descriptor length of last queried key.*/
	DWORD	m_dwSecurityDescriptorLength;
	
	//@cmember
	/* The time the last queried key was written to.*/
	FILETIME m_fileTimeLastWrite;

#endif

	//@cmember
	/* The class name of the currently open key.*/
	CString m_strClassName;

	//@cmember
	/* The computer name for the currently connected registry.*/
	CString m_strComputerName;

	//@cmember
	/* The name of the currently open key.*/
	CString m_strKeyName;

	//@cmember
	/* The name of the currently connected registry.*/
	CString m_strRegistryName;

	//@cmember
	/* The number of subkeys of the last queried key.*/
	DWORD   m_dwNumberOfSubkeys;

	//@cmember
	/* The number of values in the last queried key.*/
	DWORD   m_dwNumberOfValues;

	//@cmember
	/* The time the last queried key was written to.*/
	CTime   m_timeLastWrite;

	//@cmember
	/* Indicates whether the registry is local or on a remote system.*/
	BOOL    m_bRemote;

	//@cmember
	/* Incidates whether the registry key should be closed when done. */
	BOOL m_bCloseKeyOnDisconnect;

};

//
// SEC Extension DLL
// Reset declaration context
//
#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __SECREG_H__



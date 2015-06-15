/////////////////////////////////////////////////////////////////////////////
// RegistryUtil.h : Registry handling
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  Registry handling utility class
//


#pragma once

#include <winreg.h>
#include <deque>
#include <foundation\string\StringEx.h>


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


namespace stingray {
namespace foundation {



class CRegistry
{

// Enumerations for CRegistry
public:


	//static HKEY hKeyLocalMachine;		//	HKEY_LOCAL_MACHINE<nl>
	//static HKEY hKeyClassesRoot;		//	HKEY_CLASSES_ROOT<nl>
	//static HKEY hKeyUsers;			//	HKEY_USERS<nl>
	//static HKEY hKeyCurrentUser;		//	HKEY_CURRENT_USER<nl>
	
	enum CreationDisposition
	{
	dispositionCreatedNewKey	 = REG_CREATED_NEW_KEY,		//@@emem New Registry Key created
	dispositionOpenedExistingKey = REG_OPENED_EXISTING_KEY	//@@emem Existing Key opened
	} ;
	
	enum CreateOptions
	{
	optionsNonVolatile =	REG_OPTION_NON_VOLATILE,		//@@emem Key is preserved when system is rebooted
	optionsVolatile	=		REG_OPTION_VOLATILE				//@@emem Key is not preserved when system is rebooted
	} ;
	
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
	

public:

	CRegistry();
	CRegistry(HKEY hBase, LPCTSTR lpszSubkey = 0, bool bCreate = false, const CreatePermissions security_access_mask = permissionAllAccess);
	CRegistry(const CRegistry& rhs, LPCTSTR lpszSubkey = 0, bool bCreate = false, const CreatePermissions security_access_mask = permissionAllAccess);
	virtual ~CRegistry();

	const CRegistry& operator= (const CRegistry& rhs);

public:
	// Operations
	
	/* Establishes a connection with the registry */
	bool Connect(HKEY	hKeyToOpen	= HKEY_CURRENT_USER,
				LPCTSTR	lpszComputerName	= NULL );

	/* Creates key in registry */
	bool Create( LPCTSTR	lpszSubkeyName,
			 LPCTSTR				name_of_class		= NULL,
			 CreateOptions			options				= optionsNonVolatile,
			 CreatePermissions		permissions			= permissionAllAccess,
			 LPSECURITY_ATTRIBUTES	pSecurityAttributes	= NULL,
			 CreationDisposition *	pDisposition		= NULL 
			 );
	
	/* Opens a registry key */
	bool Open( LPCTSTR lpszSubkey,
			   const CreatePermissions security_access_mask		= permissionAllAccess );

	/* Closes the current key */
	bool Close();
	
	/* Loads a registry key from a file */
	bool Load( LPCTSTR lpszSubkeyName,
				LPCTSTR lpszFileName );
	
	/* Encapsulates the RegSaveKey API */
	bool Save( LPCTSTR lpszDestFile,
			   LPSECURITY_ATTRIBUTES pSecurityAttributes = NULL );

	/* Encapsulates the RegUnLoadKey API.*/
	bool UnLoad( LPCTSTR lpszSubkey );
	
	/* Deletes the specified key from the registry */
	bool DeleteKey( LPCTSTR lpszKeyToDelete, bool bRecursive = FALSE);

	/* Deletes the named registry value of the current key */
	bool DeleteValue( LPCTSTR lpszValueToDelete);

	/* Deletes all the values and all the subkeys of the current key */
	bool Clean();

	/* Writes the attributes of the currently open key.*/
	bool Flush();
	
	/* Retrieves a REG_BINARY value */
	bool GetBinaryValue( LPCTSTR lpszValueName, void* pBuffer, DWORD dwSize );
	
	/* Retrieves a REG_DWORD value */
	bool GetDoubleWordValue( LPCTSTR lpszValueName, DWORD& dwReturnValue );
	
	/* Retrieves a REG_SZ value */
	bool GetStringValue( LPCTSTR lpszValueName,
				string& strReturn );
	
	/* Retrieves a REG_MULTI_SZ value */
	template <typename return_array_t> 
	bool GetStringArrayValue( LPCTSTR name_of_value,
				return_array_t& return_array );

	/* Retrieves a REG_BINARY value */
	bool GetValue( LPCTSTR lpszValueName, 
				void* pBuffer, DWORD dwSize );

	/* Retrieves a REG_DWORD value */
	bool GetValue( LPCTSTR lpszValueName,
				DWORD& dwReturnValue );

	/* Retrieves a REG_MULTI_SZ value */
	template <typename return_array_t> 
	bool GetValue( LPCTSTR lpszValueName,
				return_array_t& return_array );

	/* Retrieves a REG_SZ value */
	bool GetValue( LPCTSTR lpszValueName,
				string& strReturn );
	
	/* Encapsulates the RegReplaceKey API */
	bool Replace( LPCTSTR lpszSubkeyName,
				LPCTSTR	lpszNewFile,
				LPCTSTR	lpszBackupFile);
	
	/* Encapsulates the RegRestoreKey API */
	bool Restore( LPCTSTR lpszSavedTreeFile,
				const DWORD dwVolatilityFlags = NULL );
	
	/* Sets a value for the key name specified as REG_BINARY */
	bool SetBinaryValue( LPCTSTR lpszValueName, const void* pBuffer, DWORD dwSize );
	
	/* Sets a value for the key name specified as REG_DWORD */
	bool SetDoubleWordValue( LPCTSTR lpszValueName,
				DWORD dwValue );
	
	/* Sets a value for the key name specified as REG_SZ */
	bool SetStringValue( LPCTSTR lpszValueName,
				const string& string_value );
	
	/* Sets a value for the key name specified as REG_MULTI_SZ */
	template <typename return_array_t> 
	bool SetStringArrayValue( LPCTSTR lpszValueName,
				const return_array_t& string_array );
	
	/* Sets a value for the key name specified as REG_BINARY */
	bool SetValue( LPCTSTR lpszValueName, const void* pBuffer, DWORD dwSize );

	/* Sets a value for the key name specified as REG_DWORD */
	bool SetValue( LPCTSTR lpszValueName, DWORD dwValue );

	/* Sets a value for the key name specified as REG_MULTI_SZ */
	template <typename return_array_t> 
	bool SetValue( LPCTSTR lpszValueName,
				const return_array_t& strings_to_write );
	
	/* Sets a value for the key name specified as REG_SZ */
	bool SetValue( LPCTSTR lpszValueName,
				const string& strWrite );
	
	/* Sets a value for the key name specified */
	bool SetValue( LPCTSTR		lpszValueName,
				const KeyValueTypes		type_of_value_to_set,
				const void*				lpbValueData,
				const DWORD				dwSize );

	/* Queries for information about the currently opened key */
	bool QueryInfo();

	/* Retrieves the type and data for a specified value name */
	bool QueryValue( LPCTSTR	lpszValueName,
				KeyValueTypes&		value_type,
				void*				lpbBuffer = NULL,
				DWORD*				pdwBufferSize = NULL);
	
	/* Enumerates subkeys of the currently open key */
	bool EnumerateKeys( const DWORD dwSubkeyIndex,
				string&	strSubkeyName) const;

	/* Enumerates subkeys of the currently open key */
	bool EnumerateKeys( const DWORD dwSubkeyIndex,
				string&	strSubkeyName,
				string&	strClassName ) const;
	
	/* Enumerates subkeys of the currently open key */
	LONG EnumerateKeys(const DWORD dwSubkeyIndex,
				LPTSTR	lpszSubkeyName,
				LPDWORD	lpdwSubkeyNameSize,
				LPTSTR	lpszClassName		= NULL,
				LPDWORD	lpdwClassNameSize	= NULL) const;
	

	/* Enumerates the values of the currently open key */
	bool EnumerateValues( const DWORD dwValueIndex,
				string&		strValueName,
				KeyValueTypes&	type_code ) const;

	/* Enumerates the values of the currently open key */
	bool EnumerateValues( const DWORD	dwValueIndex,
				string&			strValueName,
				KeyValueTypes&	type_code,
				LPBYTE			lpbDataBuffer,
				DWORD*			pdwSizeDataBuffer ) const;
	
	/* Indicates when a registry key has changed */
	bool NotifyChange( const HANDLE hEvent				= NULL,
				const NotifyChangeFilter changes_to_be_reported	= notifyLastSet,
				const bool bAllSubkeys							= changeSpecifiedKeyOnly,
				const bool bWaitForChange						= WAIT_FOR_CHANGE );
	
	/* Encapsulates the RegGetSecurity() call */
	bool GetSecurity( const SECURITY_INFORMATION security_info,
				PSECURITY_DESCRIPTOR	data_buffer,
				DWORD&					dwSizeDataBuffer);
	
	/* Encapsulates the RegSetKeySecurity API */
	bool SetSecurity( const SECURITY_INFORMATION& SecurityInformation,
				const PSECURITY_DESCRIPTOR pSecurityDescriptor );

public:
	// Container access
	template <typename container_t>
	void GetAllSubKeys(container_t& cont) const
	{
		cont.clear();
		DWORD dwIndex = 0;
		foundation::string sKeyName;
		while (EnumerateKeys(dwIndex++, sKeyName)) {
			cont.push_back(sKeyName);
		}
	}

	template <typename container_t>
	void GetAllValues(container_t& cont) const
	{
		cont.clear();
		DWORD dwIndex = 0;
		foundation::string sValueName;
		KeyValueTypes nType;
		while (EnumerateValues(dwIndex++, sValueName, nType)) {
			cont.push_back(sValueName);
		}
	}


protected:
	/* Converts an HKEY constant to its string equivalent.*/
	static bool KeyToStr(HKEY hKey,string& strKey);
	
	/* Converts a string equivalent to an HKEY value.*/
	static HKEY StrToKey(const string& strKey);

	// Normalizes a keyname
	static void NormalizeKey(string& strKey, bool bSubkey = FALSE);

	// Concatenates a key with a subkey
	static string ConcatenateKeys (LPCTSTR strKey, LPCTSTR strSubkey);


// Implementation	
protected:
	void Initialize();

public:
	/* The error code for the last operation.*/
	mutable LONG m_lErrorCode;

	/* The handle to the currently open registry key.*/
	HKEY m_hKey;

	/* The handle to the current registry.*/
	HKEY m_hRegistry;
	
	/* The longest subkey name length of last queried key.*/
	DWORD	m_dwLongestSubkeyNameLength;

	/* The longest class name length of last queried key.*/
	DWORD	m_dwLongestClassNameLength;
	
	/* The longest value name length of last queried key.*/
	DWORD	m_dwLongestValueNameLength;
	
	/* The longest value data length of last queried key.*/
	DWORD	m_dwLongestValueDataLength;
	
	/* The longest security descriptor length of last queried key.*/
	DWORD	m_dwSecurityDescriptorLength;
	
	/* The time the last queried key was written to.*/
	mutable FILETIME m_timeLastWrite;

	/* The class name of the currently open key.*/
	string m_strClassName;

	/* The computer name for the currently connected registry.*/
	string m_strComputerName;

	/* The name of the currently open key.*/
	string m_strKeyName;

	/* The name of the currently connected registry.*/
	string m_strRegistryName;

	/* The number of subkeys of the last queried key.*/
	DWORD   m_dwNumberOfSubkeys;

	/* The number of values in the last queried key.*/
	DWORD   m_dwNumberOfValues;

	//@cmember
	/* Indicates whether the registry is local or on a remote system.*/
	bool    m_bRemote;

};


inline
bool CRegistry::EnumerateValues (
	const DWORD dwValueIndex,
	string& strValueName,
	KeyValueTypes& type_code
) const
{
	return EnumerateValues(	dwValueIndex, strValueName, type_code, NULL, NULL);
}


};  // namespace stingray::foundation
};	// namespace stingray


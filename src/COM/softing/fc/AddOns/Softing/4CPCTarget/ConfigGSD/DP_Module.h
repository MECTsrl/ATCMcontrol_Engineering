// DP_Module.h: interface for the CDP_Module class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DP_MODULE_H__18103483_FEA3_11D1_81D8_00104B43B0E0__INCLUDED_)
#define AFX_DP_MODULE_H__18103483_FEA3_11D1_81D8_00104B43B0E0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ExtParam.h"

class CDP_Module : public CObject
{
public:
	CDP_Module(int id);
			// Constructor
	CDP_Module(CDP_Module const & other);
			// Copy-Constructor
	virtual ~CDP_Module();
			// Destructor
    const CDP_Module&	operator = (const CDP_Module& other);
			// assignment
	CString GetModuleName();
			// returns the Modulename
	void AddExtParamDef(ExtParam* pParam);
			// adding an ExtUserParam to this module
	BOOL RemoveAllUserPrmBytes();
			// Emptys the whole UserPrmArray
	BOOL RemoveAllConfBytes();
			// Emptys the whole ConfigArray
	BOOL SetUserPrmBytes(unsigned char* pucArray, DWORD dwLength);
			// Set all UserprmBytes
	BOOL SetConfigBytes(unsigned char* pucArray, DWORD dwLength);
			// Set all ConfigBytes
	BOOL AddUserPrmByte(unsigned char ucByte);
			// Add one Userprmbyte and queue it at the end of Array
	BOOL AddConfigByte(unsigned char ucByte);
			// Add one Configbyte and queue it at the end of Array
	void SetModuleName(CString szModule);
			// Set the Modulestring
	inline CByteArray* GetConfigArray();
			// returns pointer to array of Configbytes
	inline CByteArray* GetUserPrmArray();
			// returns pointer to array UserParameters
	inline void SetUserPrmLen(DWORD dwLen);
	inline DWORD GetUserPrmLen();
			// set/get uset prmdatalength
	inline CObList* GetExtParams();
			// returns pinter to ExtParamList (with ExtParam* elements)
    inline int GetModuleId() const;
    CString GetState();
    BOOL SetState(LPCTSTR &buffer);

private:
	DWORD		m_dwUserPrmLen;		// number of Userparambytes
	CByteArray	m_UserPrmBytes;		// Contains modulespezific Userparameters
	CByteArray	m_ConfigBytes;		// Contains modulespezific Configdata
	CString		m_szModule;			// Names the Module
	CObList		m_ExtParams;		// List of all ExtParams for this module
    int         m_ModuleId;
};

inline CByteArray* CDP_Module::GetConfigArray()
{
	return &m_ConfigBytes;
}

inline CByteArray* CDP_Module::GetUserPrmArray()
{
	return &m_UserPrmBytes;
}

inline void CDP_Module::SetUserPrmLen(DWORD dwLen)	{
	m_dwUserPrmLen = dwLen;
}

inline DWORD CDP_Module::GetUserPrmLen()	{
	return m_dwUserPrmLen;
}

inline CObList* CDP_Module::GetExtParams()	{
	return &m_ExtParams;
}

inline int CDP_Module::GetModuleId() const
{
    return m_ModuleId;
}

#endif // !defined(AFX_DP_MODULE_H__18103483_FEA3_11D1_81D8_00104B43B0E0__INCLUDED_)

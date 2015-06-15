#define WM_FCNAV_LOADSTATUS (WM_USER + 10)

#define FCNAV_LOADSTATUS_READY   1
#define FCNAV_LOADSTATUS_LOADING 2
#define FCNAV_LOADSTATUS_ABORTED 3

class CCpTextReader;
class CCpObjectList
{
// Construction
public:
	CCpObjectList(CCpObjectList* pParent=NULL);
	virtual ~CCpObjectList();

// Attributes
public:
	inline CCpObjectList*   GetRootObject()		const { return m_pRootObject; }
	inline int				GetObjectCount()	const { return m_pRootObject->m_ObjectCount; }
	inline const CString&	GetComputerName()	const { return m_pRootObject->m_ComputerName; }
	inline BOOL				IsLoading()			const { return m_pRootObject->m_pThread != NULL; }

	inline BOOL				ChildrenLoaded()	const { return m_bChildrenLoaded; }
	inline unsigned			GetLevel()			const { return m_Level; }
	inline unsigned			GetTypeId()			const { return m_TypeId; }
	inline unsigned			GetProperties()		const { return m_Properties; }
	inline const CString&	GetName()			const { return m_Name; }
	inline const CString&	GetErrorText()		const { return m_ErrorText; }
	inline const CString&	GetClassName()		const { return m_ClassName; }
	inline const CString&	GetAspFile()		const { return m_AspFile; }
	inline const CString&	GetConfFile()		const { return m_ConfFile; }
	inline const CString&	GetChildrenFile()	const { return m_ChildrenFile; }
	inline unsigned			GetUserData()		const { return m_UserData; }
	inline void				SetUserData(unsigned data){ m_UserData = data; }

	inline CString	GetChildrenUrl()	const { return CreateUrl(m_ChildrenFile); }
	inline CString	GetAspUrl()			const { return CreateUrl(m_AspFile); }


	const CString& CCpObjectList::GetServer() const;

	inline CCpObjectList* GetParent() const { return m_pParent; }	
	inline CCpObjectList* GetChild(unsigned Index) const { 
		ASSERT(Index < (unsigned)m_ChildrenList.GetSize());
		if (Index < (unsigned)m_ChildrenList.GetSize())
			return m_ChildrenList.GetAt(Index); 
		else 
			return NULL; }	
	inline unsigned GetChildCount() const { return m_ChildrenList.GetSize(); }

	CCpObjectList* FindChildByName(const CString& strName);
	CCpObjectList* FindGrandChildByName(const CString& strName);
	CCpObjectList* FindObjectByPath(const CString& strPath);

	BOOL Filter(unsigned ChildType,LPCSTR pChildName) const;


// Operations
public:
	BOOL Load(LPCTSTR ObjectListURL, CWnd *pMsgWnd=NULL);
	void LoadAsync(LPCTSTR ObjectListURL, CWnd *pMsgWnd);
	void StopLoadAsync();
	
protected:	

public :


private:
	unsigned		m_Level;
	unsigned		m_UserData;
	unsigned		m_TypeId;
	unsigned		m_Properties;
	CString			m_Name;
	CString			m_ErrorText;
	CString			m_ClassName;
	CString			m_AspFile;
	CString			m_ConfFile;
	CString			m_Server;
	CString			m_ChildrenFile;
	BOOL			m_bChildrenLoaded;

	CCpObjectList*	m_pRootObject;
	CCpObjectList*	m_pParent;
    CArray<CCpObjectList*, CCpObjectList*>	m_ChildrenList;

	CString m_ComputerName;
	CWinThread* m_pThread;
	BOOL m_bHaltThread;

	int m_ObjectCount;

#define FCNAV_ITEMTYPE_INVALID			0x00000000
#define FCNAV_ITEMTYPE_PROJECT			0x00000001
#define FCNAV_ITEMTYPE_CONFIGURATION	0x00000002
#define FCNAV_ITEMTYPE_WEBRESSOURCE		0x00000004
#define FCNAV_ITEMTYPE_CONTROLRESSOURCE	0x00000008
#define FCNAV_ITEMTYPE_TASK				0x00000010
#define FCNAV_ITEMTYPE_PROGRAM			0x00000020
#define FCNAV_ITEMTYPE_FUNCTIONBLOCK	0x00000040
#define FCNAV_ITEMTYPE_LIST				0x00000080
#define FCNAV_ITEMTYPE_VARIABLE			0x00000100

private :	
	static inline BOOL IsValidTypeId(unsigned Id) {
		return 
			(Id == FCNAV_ITEMTYPE_INVALID			) ||	
			(Id == FCNAV_ITEMTYPE_PROJECT			) ||
			(Id == FCNAV_ITEMTYPE_CONFIGURATION		) ||
			(Id == FCNAV_ITEMTYPE_WEBRESSOURCE		) ||
			(Id == FCNAV_ITEMTYPE_CONTROLRESSOURCE	) ||
			(Id == FCNAV_ITEMTYPE_TASK				) ||
			(Id == FCNAV_ITEMTYPE_PROGRAM			) ||
			(Id == FCNAV_ITEMTYPE_FUNCTIONBLOCK		) ||
			(Id == FCNAV_ITEMTYPE_VARIABLE		    ) ||
			(Id == FCNAV_ITEMTYPE_LIST			    );
	}

	static BOOL ParseObject(const CString& TextLine, int& Level, unsigned& TypeId, unsigned& Properties, CString& Name, CString& ClassName, CString& Server, CString& AspFile, CString& ConfFile, CString& ChildrenFile);
	static BOOL Token2Integer(const CString& sToken, int& iToken);

	static BOOL Load2(LPCTSTR ObjectListURL, int *pLastLevel, CCpObjectList* pObjectStack[], CWnd *pMsgWnd=NULL);
	static UINT LoadingThread(LPVOID pParam);

	void Clear();
	inline CString CreateUrl(const CString &File) const { 
		if (File.GetLength() > 0)
			return CString("http://") + GetServer() + CString("/4c/") + File; 
		else
			return File;
		}
};

typedef struct
{
	CWnd * pMsgWnd;
	CCpObjectList* pObject;
	CString ObjectListUrl;
}
TCpLoadThreadInfo;

/////////////////////////////////////////////////////////////////////////////

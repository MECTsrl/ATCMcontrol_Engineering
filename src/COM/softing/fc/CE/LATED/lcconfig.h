#ifndef __LCCONFIG_H_
#define __LCCONFIG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "CEIECOperation.h"

//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*
class CLCLink;
class CLCDestLink;
class CLCConfigListener;
class CLCMessageListener;
class CLCConfig;
class CLCGenObj;
struct ICEProjectManager;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
class CTargetInfo
{ 
public:
    /** target name */
    CString m_strName;
    /** target type */
    CString m_strType;
    /** assigned resource if any */
    CString m_strResource;
    /** address string if any */
    CString m_strAddress;
    /** IP Address */
    CString m_strIP;
    /** IP Address as long*/
    ULONG   m_ulIP;
    /** valid target information - only used temporary */
    bool    m_bValid;
public:
    CTargetInfo();
};


//------------------------------------------------------------------*
/**
 * CLCBase
 *
 */
class CLCBase
{
    HTREEITEM m_hTreeItem;
public:
    CLCBase()
    { m_hTreeItem = NULL;}

    virtual ~CLCBase()
    {}

    virtual void GetName(CString &) = 0;
    virtual int GetImageId() = 0;
    virtual bool GetToolTip(CString &str)
    { return false;}

    virtual bool CanLabelEdit()
    { return false;}

    virtual UINT GetContextMenu() = 0;

    void SetTreeItem(HTREEITEM hTreeItem)
    {
        m_hTreeItem = hTreeItem;
    }

    HTREEITEM GetTreeItem()
    {
        return m_hTreeItem;
    }
};

//------------------------------------------------------------------*
/**
 * CLCMember
 *
 */
class CLCMember : public CLCBase, public CXMLObject , public CRefObj
{
    DECLARE_SERIAL(CLCMember);

private:
    BL_IEC_TYP m_Type;
    CString m_strName;
    CString m_strInitVal;
    CString m_strAlias;
    CString m_strComment;
    size_t  m_size;
    bool    m_bSubscript;
public:
    enum Prop
    {   
        name = 0x1,
        type = 0x2,
        alias = 0x04,
        comment = 0x08,
        initval = 0x10,
        all = 0x1f
    };

    CLCMember();
    virtual ~CLCMember()
    {}

    void OnFinalRelease();

    CLCMember(const CLCMember &);

    const CLCMember& operator=(const CLCMember&);
    void SerializeXML(CXMLArchive & ar);
    
    void GetName(CString & strName)
    { strName = m_strName;}

    BL_IEC_TYP GetType(void)
    { return m_Type; }

    size_t GetDataLen();

    int GetImageId()
    { return LATED_IMAGEID_MEMBER; }

    UINT GetContextMenu();

    bool GetToolTip(CString &str);

    void SetProperty(int mask, CString & strName, CString & strType,
                CString & strAlias, CString & strComment, CString & strInitVal);

    void GetProperty(int mask, CString & strName, CString & strType,
                CString & strAlias, CString & strComment, CString & strInitVal);

    bool IsSubscript()
    { return m_bSubscript; }
private:
    void CopyTo(const CLCMember &);

};

typedef CMFCObjStore<CLCMember> MemberList;

//------------------------------------------------------------------*
/**
 * CLCDataType
 *
 */
class CLCDataType : public CLCBase , public CXMLObject , public CRefObj
{
    DECLARE_SERIAL(CLCDataType);

private:
    unsigned short  m_unDataID;
    CString m_strName;
    CString m_strAlias;
    MemberList m_Members;
    bool m_bPCUsed;
    bool m_bECUsed;
	bool m_bCMZUsed;

public:
    CLCDataType();
    CLCDataType(unsigned short);
    CLCDataType(const CLCDataType &);
    virtual ~CLCDataType(); 

    void OnFinalRelease();

    const CLCDataType& operator=(const CLCDataType&);
    void SerializeXML(CXMLArchive & ar);

    void GetKey(CString & strKey);
//methods for visualization

    void GetName(CString & strName)
    { strName = m_strName;}

    LPCTSTR GetName() const
    { return m_strName; }

    void SetName(CString & strName)
    { m_strName = strName; }

    unsigned short GetDataID()
    {
        return m_unDataID;
    }

    size_t GetDataLen();

    LPCTSTR GetAlias() const
    { 
        return m_strAlias; 
    }

    void SetAlias(CString & strAlias)
    { 
        m_strAlias = strAlias;
    }

    int GetImageId()
    { return LATED_IMAGEID_DATATYPE; }

    UINT GetContextMenu();

    bool CanLabelEdit()
    { return true;}

    void SetUsedInfo(bool bPCUsed, bool bECUsed, bool bCMZUsed) {
        m_bPCUsed = bPCUsed;
        m_bECUsed = bECUsed;
		m_bCMZUsed = bCMZUsed;
    }

    bool IsPCUsed(void) {
        return(m_bPCUsed);
    }

    bool IsECUsed(void) {
        return(m_bECUsed);
    }

	bool IsCMZUsed(void) {
		return(m_bCMZUsed);
	}

//methods on member list
    void TraverseMembers(int id, CLCConfigListener*);
    void AddMember(CLCMember*);
    void RemoveMembers();
    bool FindMemberByType(BL_IEC_TYP TypeToFind);
private:
    void CopyTo(const CLCDataType &);
};

typedef CMFCObjMan<CString,CLCDataType> DataTypes;
typedef CMFCObjStore<CLCLink> LinkList;
typedef CMFCObjStore<CLCDataType> DatatypeList;
typedef CMFCObjStore<CLCDestLink> DestLinks;
typedef CMap<BL_IEC_TYP,BL_IEC_TYP&,CString,CString &> IECTypes;
typedef CMap<ULONG,ULONG &,CLCConfigListener*,CLCConfigListener*&> Listeners;
typedef CMap<ULONG,ULONG &,CLCMessageListener*,CLCMessageListener*&>MessageListeners;
typedef CMap<CString,CString &, CTargetInfo, CTargetInfo&>TargetInfo;
typedef CMap<ULONG,ULONG &, CLCBase*,CLCBase*&>SecObjMap;

typedef CList<CString,CString &>NameList;


//------------------------------------------------------------------*
/**
 * CLCLink
 *
 */
class CLCLink : public CLCBase, public CXMLObject , public CRefObj
{
    DECLARE_SERIAL(CLCLink);
private:
    ULONG           m_ulIP;
    CString         m_strTarget;
    CString         m_strResource;
    CString         m_strType;
    CString         m_strIP;
    LATED_CONN_TYPE m_ConnType;
    NameList        m_DatatypeNameList;
    DatatypeList    m_DatatypeList;
    DestLinks       m_DestLinks;
    bool            m_bValid;
public:
    CLCLink();
    CLCLink(const CLCLink &);
    virtual ~CLCLink();

    void OnFinalRelease();
    
    const CLCLink& operator=(const CLCLink&);
    const CLCLink& operator=(const CTargetInfo &);

    void SerializeXML(CXMLArchive & ar);
    void ResolveDatatypes(CLCConfig *);

    void GetName(CString &);
    int GetImageId();
    bool GetToolTip(CString &str);
    UINT GetContextMenu();

    LATED_CONN_TYPE GetConnType()
    { return m_ConnType;}

    void SetConnType(LATED_CONN_TYPE conn)
    { 
        m_ConnType = conn;
    }
    
    const ULONG GetIP() const
    { return m_ulIP; }

    void GetIP(CString &);

    LPCTSTR GetTargetName()
    {
        return m_strTarget;
    }

    LPCTSTR GetTargetType()
    {
        return m_strType;
    }

    LPCTSTR GetResourceName() {
        return m_strResource;
    }

    bool IsValid()
    {   return m_bValid; }

    void SetValid(bool bValid = true)
    {
        m_bValid = bValid;
    }

    void ToTargetInfo(CTargetInfo &);

    bool IsSubscriber(CLCLink* pPublisher);
    void TraverseDestLinks(int id,CLCConfigListener*);

    void AddDestLink(CLCDestLink*);
    void DeleteDestLink(CLCDestLink*);

    //------------------------------------------------------------------*
    /**
     * Returns number of destination link objects.
     * Should be used to iterate through the destination link objects
     * via GetDestLink()
     * @return	int - number of associated destination link objects.
     * @see		GetDestLink(int)	
    */
    int GetDestLinkCount()
    { 
        return m_DestLinks.GetCount();
    }


    //------------------------------------------------------------------*
    /**
     * Returns reference to destination link object at index of argument
     * iIndex if such an object exists within the list of destination 
     * link objects or a NULL reference if no destination link object
     * exists.
     * Call Release() with the returned reference if you give up the
     * reference.
     * @param		int iIndex - index of within list of 
     *              destination link objects.
     * @return		CLCDestLink* - reference of destination link
     *              object or NULL if no reference exists.
     * @see			GetDestLinkCount()
    */
    CLCDestLink* GetDestLink(int i)
    { 
        return m_DestLinks[i]; 
    }

    //------------------------------------------------------------------*
    /**
     * Returns reference to associated data type. Returns NULL reference
     * if no datatype is associated. 
     * Call Release() with the returned reference if you give up the
     * reference.
     * @return		CLCDataType* - reference to associated data type
     *              descriptor or NULL reference if no data type is
     *              associated.
    */
    CLCDataType* GetDataType()
    {
        return m_DatatypeList[0];
    }

    //------------------------------------------------------------------*
    /**
     * Returns true if there any DestLink of type. 
     * @param		szTargetType - TargetType to look for.
     *              LATED_TARGET_TYPE_PC or LATED_TARGET_TYPE_EC
     * @return		true if found, false otherwise 
     *              
    */
    bool ExistsDestOfType(LPCTSTR szTargetType);
    
    void AssignDataType(CLCDataType*);
private:
    void CopyTo(const CLCLink &);
};


//------------------------------------------------------------------*
/**
 * CLCDestLink
 *
 */
class CLCDestLink: public CLCBase, public CXMLObject , public CRefObj
{
    DECLARE_SERIAL(CLCDestLink);
private:
    ULONG           m_ulIP;
    CString         m_strIP;
    CString         m_strTarget;
    CString         m_strType;
    CString         m_strResource;
    LATED_CONN_TYPE m_ConnType;
    bool            m_bValid;
public:
    CLCDestLink();
    CLCDestLink(const CLCDestLink &);
    virtual ~CLCDestLink()
    {}

    void OnFinalRelease();
    
    const CLCDestLink& operator=(const CLCDestLink&);
    const CLCDestLink& operator=(const CTargetInfo & info);

    void SerializeXML(CXMLArchive & ar);

    void GetName(CString &);
    int GetImageId();
    bool GetToolTip(CString &);
    UINT GetContextMenu();

    LATED_CONN_TYPE GetConnType()
    { return m_ConnType;}

    void SetConnType(LATED_CONN_TYPE conn)
    { 
        m_ConnType = conn;
    }

    const ULONG GetIP() const
    { return m_ulIP; }

    void GetIP(CString &);

    LPCTSTR GetTargetName()
    {
        return m_strTarget;
    }

    LPCTSTR GetTargetType()
    {
        return m_strType;
    }

    LPCTSTR GetResourceName() {
        return m_strResource;
    }

    bool IsValid()
    {   return m_bValid; }

    void SetValid(bool bValid = true)
    {
        m_bValid = bValid;
    }

    void ToTargetInfo(CTargetInfo &);

    void SetPublisherAddress(ULONG);
private:
    void CopyTo(const CLCDestLink &);
};




//------------------------------------------------------------------*
/**
 * CLCGenObj
 *
 */
class CLCGenObj : public CXMLObject
{
    DECLARE_SERIAL(CLCGenObj);

private:
    LATED_GENTYPE_TYPE m_eType;   // FILE, LINK_IMP, LINK_PROG, LINK_TASK
    // attributes
    CString            m_strFile;       // FILE: filename, all others: ResourceName
    CString            m_strName;       // LINK_PROG: ProgramInstName,
                                        // LINK_TASK: TaskName,
                                        // LINK_IMP: GVLName
    CString            m_strClass;      // LINK_PROG: Program Class Name
    CString            m_strTask;       // LINK_PROG: Task name for the program instance
    CString            m_strInterval;   // LINK_TASK: task interval
    CString            m_strPriority;   // LINK_TASK: task priority
    LATED_GENPOS_TYPE  m_ePos;          // LINK_TASK, LINK_PROG: first or last obj

public:
    CLCGenObj();
    virtual ~CLCGenObj();

    CLCGenObj(const CLCGenObj &);    
    const CLCGenObj& operator=(const CLCGenObj&);

    void Clear();
    void InitFile(const CString& strFile);
    void InitLinkImp(const CString& strResource, const CString& strGVL);
    void InitLinkProg(const CString& strResource, const CString& strProgInst, 
                      const CString& strTask, const CString& strProgClass, BOOL bPosFirst);
    void InitLinkTask(const CString& strResource, const CString& strTaskName,
                      const CString& strInterval, const CString& strPriority,
                      BOOL bPosFirst);

    virtual void SerializeXML(CXMLArchive & ar);

    void Execute(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp);
    void Remove(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp);
    BOOL Compare(const CLCGenObj& other);

private:
    void CreateLinkFile(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp);
    void RemoveLinkFile(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp);
    void CreateLinkImp (ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp);
    void RemoveLinkImp (ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp);
    void CreateLinkTask(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp);
    void RemoveLinkTask(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp);
    void CreateLinkProg(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp);
    void RemoveLinkProg(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp);

};

typedef CList<CLCGenObj, const CLCGenObj&> GenObjList;



//------------------------------------------------------------------*
/**
 * CLCConfigListener
 *
 */
class CLCConfigListener
{
public:
    virtual void OnDataType(int id, CLCDataType *)
    {}
    virtual void OnMember(int id, CLCDataType *, CLCMember*)
    {}
    virtual void OnLink(int id, CLCLink *)
    {}
    virtual void OnDestLink(int id, CLCLink *, CLCDestLink *)
    {}
    virtual void OnEndDataType(int id, CLCDataType *)
    {}
    virtual void OnEndLink(int id, CLCLink *)
    {}
    virtual void OnIECType(int id, BL_IEC_TYP Type, CString & strType)
    {}
    
    virtual void OnTarget(int id, CTargetInfo & info)
    {}
};




//------------------------------------------------------------------*
/**
 *  class CLCMessageListener - Listener or callback object for 
 *  receiving messages. An instance of this class has to be registered
 *  via CLCConfig::AddMessageListener() in order to receive messages.
 *  In order to revoke message reception call CLCConfig::RemoveMessage-
 *  Listener().
 *
 *  @see    class CLCConfig, CLCConfig::AddMessageListener(),
 *          CLCConfig::RemoveMessageListener().
*/
class CLCMessageListener
{
public:
    virtual void OnMessage(HRESULT hr, LPCTSTR pszMessage) = 0;
};


//------------------------------------------------------------------*
/**
 * CLCConfig
 *
 */
class CLCConfig : public CXMLObject , public CRefObj
{
    DECLARE_SERIAL(CLCConfig);
private:
    /** map of IEC datatypes - key = BL_IEC_TYP */
    IECTypes            m_IECTypes;
    /** map of configured data types key = datatype identifier*/
    DataTypes           m_DataTypes;
    /** list of declared virtual communication links */
    LinkList            m_LinkList;
    /** list of config listeners*/
    Listeners           m_Listeners;
    /** list of message listeners*/
    MessageListeners    m_MessageListeners;
    /** list of generated objects */
    GenObjList          m_GenObjs;
    GenObjList          m_NewGenObjs;
    CString   m_strPath;
    ICEProjectManager* m_pPM;
    TargetInfo m_TargetInfo;
    static SecObjMap s_Objects;
    CComQIPtr<ICEIECResourceOp> m_pResourceOp;

public:
    CLCConfig();
    CLCConfig(LPCTSTR pszFilePath);
    virtual ~CLCConfig();
    void SerializeXML(CXMLArchive & ar);

    static void AddObject(CLCBase*);
    static void RemoveObject(CLCBase*);
    static bool IsObjectValid(CLCBase*);

    static LATED_CONN_TYPE ToConnType(LPCTSTR);
    static LPCTSTR FromConnType(LATED_CONN_TYPE);
    static size_t GetDataLenFromType(BL_IEC_TYP);
    static bool GetSubscript(CString &, CString &, size_t &);

    LPCTSTR GetPath() const
    { 
        return m_strPath;
    }

    void SetPM(ICEProjectManager*);
    HRESULT FillIECTypes();
    void GetIECTypes();
    HRESULT GetTargetList();

//methods with CLCConfigListener
    void AddConfigListener(CLCConfigListener *);
    void RemoveConfigListener(CLCConfigListener *);
    void TraverseDatatypes(int id);
    void TraverseDatatype(int id,CLCDataType*); 
    void TraverseLinks(int id);
    void TraverseIECTypes(int id);
    void TraverseTargetInfo(int id);

//methods with message listener
    void AddMessageListener(CLCMessageListener *);
    void RemoveMessageListener(CLCMessageListener *);
    int  Check();

//methods with links
    int GetLinkCount();
    CLCLink* GetLink(int);

    void AddLink(CLCLink*);
    void AddDestLink(CLCLink*,CLCDestLink*);
    void DeleteLink(CLCLink*);
    void DeleteDestLink(CLCLink*,CLCDestLink*);

    void ActualizeLinks();

//methods with data types
    void ResolveDatatypes();
    unsigned short AllocDataID();
    void AddDatatype(CLCDataType*);
    void DeleteDatatype(CLCDataType*);
    void RenameDatatype(CString & , CLCDataType*);
    CLCDataType * GetDatatype(CString &);
    CLCDataType** GetDatatypeList();
    void FreeDataTypeList(CLCDataType**);

    // methods with generated objects
    void AddGenObj(const CLCGenObj& genObj);
    void GenObjsExecute();

private:
    bool ConvertToIP(CString & strAddr, CString & strIP, ULONG & ulIP);
    void IssueMessage(HRESULT hr, LPCTSTR);
};


#endif // __LCCONFIG_H_

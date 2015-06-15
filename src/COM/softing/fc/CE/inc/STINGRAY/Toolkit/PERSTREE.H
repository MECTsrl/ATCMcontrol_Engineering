/////////////////////////////////////////////////////////////////////////////
// PersTree.h : Declaration of SECPersistentTreeNode
//
// Stingray Software Extension Classes
// Copyright (C) 1998 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Author:       Mark Isham
// Description:  Declaration of SECPersistentTree
//

#ifndef __SECPERSTREE_H__
#define __SECPERSTREE_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

#if _MFC_VER <= 0x0410
#define HKEY_CURRENT_USER           (( HKEY ) 0x80000001 )
#endif

//
// SEC Extension DLL
// Initialize declaration context
//
#ifdef _SECDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL

// Constants

// Macros

// Types

// Forward Declarations
class SECPersistentTreeNode;

/////////////////////////////////////////////////////////////////////////////
// SECPTNStrategy

// This abstract base class provides the interface for the tree node
// persistance strategy. Derived classes may implement persistance
// to profiles, archives, registry, encrypted files, or whatever...
// Note: PTN="Persistent Tree Node"

//@doc SECPTNStrategy 
//@class 
// Abstract base class defining the persistance strategy interface used for 
// load/save operations on the SECPersistentTreeNode objects. 
// Note: the listBranch CStringList parameter below is used to list the
// node names of the particular branch of interest, from child to root.
// For example, "node4","node3","node2","node1" would be a branch in which
// "node1" is the root and "node 4" is the leaf of interest.
//@xref <c SECPTNStrategyProfile> <c SECPersistentTreeNode>
class SECPTNStrategy {
public:	
	SECPTNStrategy() {}
	virtual ~SECPTNStrategy() {}	// virtual destructor for polymorphic dealloc

	// listBranch contains all node names ordered from the
	// node being saved, up its parent list to the root node

	// Description of interfaces

	// SaveKeyValueData will save a key-value pair to a particular
	// branch of the tree as described by the listBranch parameter.
	// The listBranch parameter is a linked list of CString objects
	// naming all tree branch to save this data, starting at the 
	// saving node, and working up the parent branch to the root node.
	// The listChildren is a linked list of the names of all the children
	// (1 level deep) for this particular node.

	//@access Operations

	//@cmember
	/* Blank out all the key-value data at the node passed in.*/
	virtual BOOL ClearSaveData(const CStringList& listBranch)=0;
	
	//@cmember
	/* Save key-value data at the node passed in.*/
	virtual BOOL SaveKeyValueData(const CStringList& listBranch,
								  const CString& strKey,
								  const CString& strValue)=0;

	//@cmember
	/* Save a list of all children of the node passed in.*/
	virtual BOOL SaveChildrenList(const CStringList& listBranch,
								  const CStringList& listChildren)=0;

	//@cmember
	/* Get all key-value data AND all children node names for the node passed in.*/
	virtual BOOL GetNodeKeyValueData(const CStringList& listBranch,
								  CStringList& listChildren,
								  CStringList& listKeys,
								  CStringList& listValues)=0;
};

/////////////////////////////////////////////////////////////////////////////
// SECPTNStrategyProfile

// Extends SECPTNStrategy with behavior conducive to saving inside a 
// private profile using the standard WIN32 GetPrivateProfileString,
// WritePrivateProfileString APIs

#define PTN_PROFILE_BUFFSIZE	512

//@doc SECPTNStrategyProfile 
//@class 
// This SECPTNStrategy derived class implements the SECPTNStrategy interface
// for storing data to a file profile (i.e. via the GetPrivateProfile,
// WritePrivateProfile interface). The tree organization is maintained
// through a unique naming scheme of profile stanza names and an
// iteration of child name links for each stanza.
//@BASE public | SECPTNStrategy
//@xref <c SECPTNStrategy> <c SECPersistentTreeNode> <c SECPTNStrategyRegistry>
class SECPTNStrategyProfile : public SECPTNStrategy {
public:
	//@cmember
	/* Initialize the profile strategy.*/
	SECPTNStrategyProfile(const CString& strProfileName);

	virtual BOOL ClearSaveData(const CStringList& listBranch);
	virtual BOOL SaveKeyValueData(const CStringList& listBranch,
								  const CString& strKey,
								  const CString& strValue);
	virtual BOOL SaveChildrenList(const CStringList& listBranch,
								  const CStringList& listChildren);
	virtual BOOL GetNodeKeyValueData(const CStringList& listBranch,
								  CStringList& listChildren,
								  CStringList& listKeys,
								  CStringList& listValues);


	// Use to reset the max buffer size
	void SetMaxBufSize(int nNewSize) { m_nMaxBufSize=nNewSize; }

protected:																
	CString m_strProfileName;
	virtual BOOL GetAllStanzaEntries(const CString& strStanza,CStringList& listEntries);
	void MakeStanzaNameFromBranchList(const CStringList& listBranch,
									  CString& strStanzaName);

private:
	// Profile utility names
	LPCTSTR m_lpszChildRoot,m_lpcszStanzaCatStr;
	int m_nMaxBufSize;
};

/////////////////////////////////////////////////////////////////////////////
// SECPTNStrategyRegistry

// Extends SECPTNStrategy with behavior conducive to saving/loading
// to the windows system registry.

//@doc SECPTNStrategyRegistry
//@class 
// This SECPTNStrategy derived class implements the SECPTNStrategy interface
// for storing data to the system registry.
//@BASE public | SECPTNStrategy
//@xref <c SECPTNStrategy> <c SECPersistentTreeNode> <c SECPTNStrategyProfile>
class SECPTNStrategyRegistry : public SECPTNStrategy {
public:
	//@cmember
	/* Initialize the registry strategy.*/
	SECPTNStrategyRegistry(const CString& strRootSubKey,HKEY hKey=HKEY_CURRENT_USER);

	virtual BOOL ClearSaveData(const CStringList& listBranch);
	virtual BOOL SaveKeyValueData(const CStringList& listBranch,
								  const CString& strKey,
								  const CString& strValue);
	virtual BOOL SaveChildrenList(const CStringList& listBranch,
								  const CStringList& listChildren);
	virtual BOOL GetNodeKeyValueData(const CStringList& listBranch,
								  CStringList& listChildren,
								  CStringList& listKeys,
								  CStringList& listValues);
protected:
	virtual void BuildStringFromBranchList(CString& strPath,const CStringList& listBranch);

	CString m_strRootSubKey;
	HKEY m_Key;
};


/////////////////////////////////////////////////////////////////////////////
// SECPTNFactory

//@doc SECPTNFactory 
//@class Factory used for easy SECPersistentTreeNode allocation and memory management.
//@xref <c SECPersistentTreeNode>
class SECPTNFactory {
public:
	SECPTNFactory() {}
	virtual ~SECPTNFactory();

	//@cmember
	/* Create a new SECPersistentTreeNode object.*/
	SECPersistentTreeNode* CreateNode();

	//@cmember
	/* Create a new SECPersistentTreeNode object.*/
	SECPersistentTreeNode* CreateNode(CRuntimeClass* pClass);

	//@cmember
	/* Create a new SECPersistentTreeNode object.*/
	SECPersistentTreeNode* CreateNode(const CString& strNodeName,
						SECPersistentTreeNode* pParent);

	//@cmember
	/* Create a new SECPersistentTreeNode object.*/
	SECPersistentTreeNode* CreateNode(const CString& strNodeName,
						SECPersistentTreeNode* pParent,
						CRuntimeClass* pClass);

	//@cmember
	/* Delete a SECPersistentTreeNode previously allocated by this factory.*/
	BOOL DeleteNode(SECPersistentTreeNode* pNode);

protected:
	CTypedPtrList<CPtrList,SECPersistentTreeNode*> m_listAlloc;
};

/////////////////////////////////////////////////////////////////////////////
// SECPersistentTreeNode

// Objects of this type can be arranged in a tree-hierarchy to save
// key-value data. 
//
// Objects derived from SECPTNStrategy can be passed into 
// SaveKeyValues/LoadKeyValues to provide persistance in a state
// generic manner (save to files, registry, etc.)

// SECPersistentTreeNode Return codes
// Return codes document in SECPersistentTreeNode::Create, please
// reflect any updates there.
#define SEC_PERSTREE_SUCCESS				0x0000
#define SEC_PERSTREE_ALREADY_CREATED		0x0001
#define SEC_PERSTREE_NOT_CREATED			0x0002
#define SEC_PERSTREE_NULL_NODE    			0x0004
#define SEC_PERSTREE_NULL_KEYNAME			0x0008
#define SEC_PERSTREE_KEY_ALREADY_EXISTS		0x0010
#define SEC_PERSTREE_NODE_ALREADY_EXISTS	0x0020
#define SEC_PERSTREE_KEY_NOT_FOUND			0x0040
#define SEC_PERSTREE_INVALID_PARAMETER		0x0080
#define SEC_PERSTREE_INVALID_BUFFER			0x0100
#define SEC_PERSTREE_UNKNOWN_ERROR			0xffff

typedef CTypedPtrList<CPtrList,SECPersistentTreeNode*> PTNList;

//@doc SECPersistentTreeNode 
//@class Objects of this type are used to store "Key-Value" data, much
// like is used in the registry or a profile (key describing the data,
// value representing the data). These objects can then be organized
// in a tree structure and stored to persistance storage via a mechanism
// defined by the SECPTNStrategy abstract base class.
//@BASE public | CObject
//@xref <c SECPTNStrategy>  <c SECPTNFactory>
class SECPersistentTreeNode : public CObject
{

	DECLARE_DYNCREATE(SECPersistentTreeNode)

	//---------------------------------------------------------------------//
	// 						   Public Interface							   //
	//---------------------------------------------------------------------//
public:
	
	//@access Construction/Initialization

	//@cmember
	/* Constructor*/
	SECPersistentTreeNode();
	virtual ~SECPersistentTreeNode();

	//@cmember
	/* Initialize this object.*/
	virtual DWORD Create(const CString& strNodeName,SECPersistentTreeNode* pParent);

	// Attributes
	
	// Accessors
	// ...Get and Set functions...	

	//@access Accessors

	//@cmember
	/* Add string data to this object's internal state.*/
	virtual DWORD AddKeyValue(const CString& strKey,const CString& strValue,
							  BOOL bOverwriteExisting=TRUE);

	//@cmember
	/* Add integer data to this object's internal state.*/
	virtual DWORD AddKeyValue(const CString& strKey,int nValue,
							  BOOL bOverwriteExisting=TRUE);

	//@cmember
	/* Add boolean data to this object's internal state.*/
	virtual DWORD AddKeyValueBool(const CString& strKey,BOOL bValue,
							  BOOL bOverwriteExisting=TRUE);

	//@cmember
	/* Add raw binary data to this object's internal state.*/
	virtual DWORD AddKeyValueBinary(const CString& strKey,
							  BYTE* pBuffer,UINT nBufferSize,
							  BOOL bOverwriteExisting=TRUE);

	//@cmember
	/* Deletes a key-value pair given a key name. */
	virtual DWORD Delete(const CString& strKeyName);


	//@cmember
	/* Get string data from this object's internal state*/
	virtual DWORD GetKeyValue(const CString& strKey,CString& strValue,CString strDefault=(CHAR)NULL);

	//@cmember
	/* Get integer data from this object's internal state*/
	virtual DWORD GetKeyValueInt(const CString& strKey,int& nValue,int nDefault=0);

	//@cmember
	/* Get unsigned integer data from this object's internal state*/
	virtual DWORD GetKeyValueUInt(const CString& strKey,UINT& nValue,UINT nDefault=0);

	//@cmember
	/* Get long data from this object's internal state*/
	virtual DWORD GetKeyValueLong(const CString& strKey,long& nValue,long nDefault=0);

	//@cmember
	/* Get unsigned long data from this object's internal state*/
	virtual DWORD GetKeyValueULong(const CString& strKey,ULONG& nValue,ULONG nDefault=0);

	//@cmember
	/* Get boolean data from this object's internal state*/
	virtual DWORD GetKeyValueBool(const CString& strKey,BOOL& bValue,BOOL bDefault=TRUE);

	//@cmember
	/* Get raw binary data from this object's internal state*/
	virtual DWORD GetKeyValueBinary(const CString& strKey,BYTE** pBinaryBuffer,UINT& nSize);

	//@cmember
	/* Get the parent node of this object.*/
	SECPersistentTreeNode* GetParent() const { return m_pParent; }

	//@cmember
	/* Locate child node with a specific name.*/
	SECPersistentTreeNode* FindChildNode(const CString& strNodeName);

	//@cmember
	/* Get list of all children.*/
	PTNList* GetChildList()		{ return &m_listChildren; }

	//@cmember
	/* Get the first position of the key-value pairs*/
	POSITION GetFirstKeyValuePosition() const;

	//@cmember
	/* Enumerate the key-value pairs*/
	BOOL EnumerateKeyValues(POSITION& position, CString& strKey, CString& strValue);
	
	//@cmember
	/* Get the name of this particular node (must be unique among siblings)*/
	CString GetNodeName() { return m_strNodeName; }

	// Operations
	// Implementation of the strategy design pattern, pass in different
	// strategy subclasses to manipulate the data

	//@access Operations

	//@cmember
	/* Save the data associated with this node and all children.*/
	virtual BOOL SaveKeyValues(SECPTNStrategy* pStrategy);

	//@cmember
	/* Load data from the strategy and populate child nodes.*/
	virtual BOOL LoadKeyValues(SECPTNStrategy* pStrategy);

	//---------------------------------------------------------------------//
	// 							 Overridables							   //
	//---------------------------------------------------------------------//
protected:

	//---------------------------------------------------------------------//
	// 						Protected Implementation					   //
	//---------------------------------------------------------------------//
protected:

	// 1 key-value pair. Internal use only
	class SECPersTreeNodeKeyValuePair : public CObject {
	public:
		CString m_strKey;
		CString m_strValue;
	};

	// Implementation Member Variables
	CString m_strNodeName;
	SECPersistentTreeNode* m_pParent;
	SECPTNFactory m_NodeFactory;
	PTNList m_listChildren;
	CTypedPtrList<CPtrList,SECPersTreeNodeKeyValuePair*> m_listKeyValues;

	BOOL m_bDidCreate;

	// Implementation Member Functions
	virtual DWORD AddChildNode(SECPersistentTreeNode* pChild);
	SECPersTreeNodeKeyValuePair* FindKey(const CString& strKeyName);
	void BuildNodeParentList(CStringList& listParents);
	void BuildNodeChildList(CStringList& listChildren);
	BOOL BuildStringFromBuffer(CString& strBuffer,BYTE* pBuffer,UINT nSize);
	BOOL BuildBufferFromString(const CString& strBuffer,BYTE** pBuffer,UINT& nSize);	

	DWORD AddMultipleBinaryChunkKeyValues(const CString& strKey,CString& strValue);
	BOOL GetBinaryStringDataFromChunks(const CString& strKey,CString& strValue);
private:

};


/////////////////////////////////////////////////////////////////////////////

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // WIN32

#endif //__SECPERSTREE_H__


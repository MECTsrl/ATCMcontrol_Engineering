//-----------------------------------------------------------------------------
//                OPC ENGINE              OPC SESSION                         |
//                                                                            |
//  Filename    : GENTREE.H                                                   |   
//  Version     : 2.00.0.00.release       2.00.0.00.release                   |
//  Date        : 29-March-1999           29-March-1999                       |
//                                                                            |
//  Description : Tree handling classes                                       |
//                - GenericElement: tree element                              |
//                - GenericLeaf: leaf object class                            |
//                - GenericBranch: branch object class                        |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  01/05/99  RT        use GenericList for lists in GenericBranch            |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _GENTREE_H_
#define _GENTREE_H_

#include "GenObj.h"

//-----------------------------------------------------------------------------
// GenericElement                                                             |
//-----------------------------------------------------------------------------

class GenericBranch;

class OPCENGINE_EXPORT GenericElement : public GenericObject
{
public:
	GenericElement(IN DWORD objType);

	// element name
	CString getName(void) const;
	virtual HRESULT setName(IN LPCTSTR name);

	// validate name
	virtual HRESULT validateName(IN LPCTSTR name, IN OPTIONAL BOOL checkIfUnique = TRUE);

	// element path
	virtual CString getFullName(void);

	// parent branch
	virtual BOOL setParentBranch(IN GenericBranch *parent);
	GenericBranch *getParentBranch(void);

	// object trace id
	virtual LPCTSTR getObjTraceId(void);

	// object identity
	virtual CString getKey(void);
	virtual BOOL hasKey(LPCTSTR key);

	// path splitting
	static void reducePath(IN LPCTSTR path, OUT LPCTSTR &first, 
			OUT LPCTSTR &second, IN OPTIONAL BOOL fromLeft = TRUE);

protected:
	CString m_name;					// element name
	GenericBranch *m_parent;		// parent branch

	~GenericElement(void);

	// callbacks
	virtual void onSetName(IN LPCTSTR name);
	virtual void onSetParentBranch(IN GenericBranch *parent);

}; // GenericElement


inline CString GenericElement::getName(void) const
{ return m_name; }




//-----------------------------------------------------------------------------
// GenericLeaf                                                                |
//-----------------------------------------------------------------------------

class OPCENGINE_EXPORT GenericLeaf : public GenericElement
{
public:
	GenericLeaf(IN DWORD objType); 
}; // GenericLeaf




//-----------------------------------------------------------------------------
// GenericBranch                                                              |
//-----------------------------------------------------------------------------

typedef CTypedPtrList<CPtrList, GenericLeaf *> LeafList;
typedef CTypedPtrList<CPtrList, GenericBranch *> BranchList;

class OPCENGINE_EXPORT GenericBranch : public GenericElement
{
friend GenericElement;

public:
	GenericBranch(IN DWORD objType, 
			IN OPTIONAL DWORD supportedChildObj = GENOBJ_TYPE_LEAF | GENOBJ_TYPE_BRANCH,
			IN OPTIONAL BYTE whatTagList = GENLIST_CREATE_LIST, IN OPTIONAL BYTE whatNodeList = GENLIST_CREATE_LIST, 
			IN OPTIONAL int blockSizeTagList = 10, IN OPTIONAL int blockSizeNodeList = 10,
			IN OPTIONAL int hashSizeTagList = 17, IN OPTIONAL int hashSizeNodeList = 17); 

	// add child
	virtual BOOL addLeaf(IN GenericLeaf *newLeaf, IN OPTIONAL int index = -1);
	virtual BOOL addBranch(IN GenericBranch *newBranch, 
			IN OPTIONAL int index = -1);

	// remove childs
	virtual BOOL removeLeaf(IN GenericLeaf *leaf, IN OPTIONAL BOOL doLock = TRUE);
	virtual BOOL removeBranch(IN GenericBranch *branch, IN OPTIONAL BOOL doLock = TRUE);
	virtual void removeAllLeafs(IN OPTIONAL BOOL deep = TRUE);
	virtual void removeAllBranches(IN OPTIONAL BOOL deep = TRUE);

	// find child
	virtual GenericLeaf *findLeaf(IN LPCTSTR name, IN OPTIONAL BOOL deep = FALSE);
	virtual GenericBranch *findBranch(IN LPCTSTR name, IN OPTIONAL BOOL deep = FALSE);

	// check if a name is a unique child name
	BOOL isUniqueChildName(IN LPCTSTR name, 
			IN OPTIONAL GenericElement *child = NULL);

	// get child lists
	GenericList<GenericLeaf> *getLeafList(void);
	GenericList<GenericBranch> *getBranchList(void);

	// have to check the uniqueness of the childs?
	void trustUniqueChilds(IN BOOL trust);

	// compatibility methods for V1.10. 
	// Please use GenericList instead
	void lockLeafList(void);
	void lockBranchList(void);
	void unlockLeafList(void);
	void unlockBranchList(void);
	CSyncObject *getLeafListLockObj(void);
	CSyncObject *getBranchListLockObj(void);
#ifndef _AFXDLL
	HANDLE getLeafListLockHandle(void);
	HANDLE getBranchListLockHandle(void);
#endif

protected:
	~GenericBranch(void); 

	GenericList<GenericLeaf> m_leafList;		// list of child leafs
	GenericList<GenericBranch> m_branchList;	// list of child branches
	BYTE m_trustUniqueChilds;					// check uniqueness?

	// change the name of a child object in lists
	HRESULT changeChildName(IN GenericElement *child, IN LPCTSTR name);

	// callbacks
	virtual void onAddLeaf(IN GenericLeaf *leaf);
	virtual void onRemoveLeaf(IN GenericLeaf *leaf);
	virtual void onAddBranch(IN GenericBranch *branch);
	virtual void onRemoveBranch(IN GenericBranch *branch);

}; // GenericBranch


inline GenericList<GenericLeaf> *GenericBranch::getLeafList(void)
{ return &m_leafList; }

inline GenericList<GenericBranch> *GenericBranch::getBranchList(void)
{ return &m_branchList; }

inline void GenericBranch::trustUniqueChilds(IN BOOL trust)
{ m_trustUniqueChilds = trust; }


// compatibility methods for V1.10. Please use GenericList instead

inline CSyncObject *GenericBranch::getLeafListLockObj(void) 
{ return m_leafList.getLockObject(); }

inline CSyncObject *GenericBranch::getBranchListLockObj(void) 
{ return m_branchList.getLockObject(); }

#ifndef _AFXDLL

inline HANDLE GenericBranch::getLeafListLockHandle(void) 
{ return m_leafList.getLockHandle(); }

inline HANDLE GenericBranch::getBranchListLockHandle(void) 
{ return m_branchList.getLockHandle(); }

#endif


#endif

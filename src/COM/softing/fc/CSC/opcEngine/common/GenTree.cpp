//-----------------------------------------------------------------------------
//                OPC ENGINE              OPC SESSION                         |
//                                                                            |
//  Filename    : GENTREE.CPP                                                 |   
//  Version     : 2.00.0.00.release       2.00.0.00.release                   |
//  Date        : 29-March-1999           12-March-1999                       |
//  Author      : ATCM                                                 |
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

#include "stdafx.h"
#include "GenTree.h"
#include "OPCTrace.h"
#include <opcerror.h>
#ifdef SERVER
#include "OPCEngine.h"
#else
#include "OPCSession.h"
#endif

//-----------------------------------------------------------------------------
// GenericElement                                                              |
//-----------------------------------------------------------------------------

GenericElement::GenericElement(
	IN DWORD objType)	// object type
 : GenericObject(objType | GENOBJ_TYPE_ELEMENT)
{
	m_parent = NULL;	
}

GenericElement::~GenericElement(void)
{
	setParentBranch(NULL);	// release parent object
}


//-----------------------------------------------------------------------------
// setName
//
// - set element name
// - check if name is valid
// - add name to parents child name list
//
// returns:
//		S_OK                - set name
//		E_INVALIDARG        - no valid name or parameter
//		OPC_E_DUPLICATENAME - no unique name
//
HRESULT GenericElement::setName(
	IN LPCTSTR name)	// element name
{
	HRESULT res;

    if (!name)
		return E_INVALIDARG;

	_TRACE(TL_DEB, TG_TREE, (_T("> setName %s"), name));
	if (m_name == name) // no change in name
		return S_OK;

	// have a parent, check if name is ok with siblings:
	// check if name is composed of valid characters and if unique:
	if (FAILED(res = validateName(name)))
	{
		// cannot set this name, name is not valid !!
		return res;
	}
    else 
	{
		// name valid
		if (m_parent)
		{
			CString prevName = (LPCTSTR) m_name;

			m_name = name;
			// change map entry in parent object
			res = m_parent->changeChildName(this, prevName);

			if (FAILED(res))
				m_name = prevName;
		}
		else
			m_name = name;

		if (SUCCEEDED(res))
		{
			// set name
			onSetName(name);
			m_objectTraceId.Empty();		
			_TRACE(TL_INF, TG_TREE, (_T("setName=%s"), m_name));
			res = S_OK;
		}

		return res;
    }
} // setName



//-----------------------------------------------------------------------------
// onSetName
//
void GenericElement::onSetName(
	IN LPCTSTR name)	// element name
{} // onSetName


//-----------------------------------------------------------------------------
// onSetParentBranch
//
void GenericElement::onSetParentBranch(
	IN GenericBranch *parent)	// parent branch
{} // onSetName


//-----------------------------------------------------------------------------
// validateName
//
// - decrement reference counter
// - free object if no reference left
//
// returns:
//		S_OK                - valid name
//		E_INVALIDARG        - no valid name
//		OPC_E_DUPLICATENAME - no unique name
//
HRESULT GenericElement::validateName(
	IN LPCTSTR name,				// name
	IN OPTIONAL BOOL checkIfUnique) // check if unique in parent
{
	BOOL ok = TRUE;
	LPCTSTR accept = NULL;
	LPCTSTR bad = NULL;
	HRESULT res = S_OK;

	if (!name)
		return E_INVALIDARG;

	_TRACE(TL_DEB, TG_TREE, (_T("> validateName %s checkUnique:%s"), name, BOOL2STR(checkIfUnique)));

#ifdef SERVER
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	engine->getTreeDelimiter(&accept, &bad);
#else
	TCHAR acceptBuf[2];
	TCHAR badBuf[1];
	GenericPointer<OPCSession> session = ::getOPCSession();
	acceptBuf[0] = session->getTreeDelimiter();
	acceptBuf[1] = _T('\0');
	badBuf[0] = _T('\0');
	accept = acceptBuf;
	bad = badBuf;
#endif

	ok &= (_tcslen(name) != 0);
    ok &= (_tcspbrk(name, accept) == NULL);
    ok &= (_tcspbrk(name, bad) == NULL);

	if (!ok)
		res = E_INVALIDARG;

	// check for uniqueness, too?
    if ((ok) && (checkIfUnique))
	{
        // so check if new name will be unique
        if (m_parent != NULL) 
		{			
            if (!(ok = m_parent->isUniqueChildName(name, (GenericElement *)this)))
				res = OPC_E_DUPLICATENAME;
        }
    }

	if (!ok)
		_TRACE(TL_INF, TG_TREE, (_T("validateName:%s failed 0x%X"), name, res));

    return res;
} // validateName


//-----------------------------------------------------------------------------
// getFullName
//
// - composes the absolut name of the element
//
// returns:
//		path of the element 
//
CString GenericElement::getFullName(void) 
{
    CString name = getName();
    CString path;
    GenericPointer<GenericBranch> parent = m_parent;
	TCHAR pathSeparator;

#ifdef SERVER
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	pathSeparator = engine->getTreeDelimiter(NULL, NULL);
#else
	GenericPointer<OPCSession> session = ::getOPCSession();
	pathSeparator = session->getTreeDelimiter();
#endif

	if (!m_parent)
	{
		_TRACE(TL_INF, TG_TREE, (_T("fullName=%s"), name));
		return name;	// no parent, no path before name
	}

	parent.addRef();
    while ((!!parent) && (_tcslen(parent->getName()) != 0)) // top node is a invisible node with no name
    {
        path = name;
        name = parent->getName();
        name += pathSeparator;
        name += path;
		parent = parent->getParentBranch();
    }

	_TRACE(TL_INF, TG_TREE, (_T("fullName=%s"), name));
    return name;
} // getFullName


//-----------------------------------------------------------------------------
// reducePath
//
// - reduce path into two parts by searching path seperator from left or right
//
void GenericElement::reducePath(
	IN LPCTSTR path,			// path (returned pointers point into this buffer)
	OUT LPCTSTR &first,			// pointer to first path element(s) MULL if top hierarchy object
	OUT LPCTSTR &second,		// pointer to second path element(s) NULL if path at end
	IN OPTIONAL BOOL fromLeft)	// where to start ?
{
	_TRACEC(TL_DEB, TG_TREE, _T("GenericElement"), (_T("> reducePath %s fromLeft:%s"), path, BOOL2STR(fromLeft)));
	LPCTSTR accept = NULL;
	LPTSTR delim;
	LPTSTR next;

#ifdef SERVER
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	engine->getTreeDelimiter(&accept, NULL);
#else
	TCHAR acceptBuf[2];
	GenericPointer<OPCSession> session = ::getOPCSession();
	acceptBuf[0] = session->getTreeDelimiter();
	acceptBuf[1] = _T('\0');
	accept = acceptBuf;
#endif	

	if (fromLeft)
	{ // find first path element from left
		first = path;
		second = NULL;
		delim = (LPTSTR)_tcspbrk(path, accept);
	}
	else
	{ // find first path element from right -> last element
		first = NULL;
		second = path;
		delim = NULL;
		next = (LPTSTR)path;
		while (next = _tcspbrk(next, accept))
		{
			delim = next;
			next++;
		}
		if (delim)
			first = path;
	}

	if (delim)
	{
		*delim = _T('\0');   //split path here
		delim++;
		second = delim;
	}

	_TRACEC(TL_DEB, TG_TREE, _T("GenericElement"), (_T("< reducePath %s %s"), first, second));
} // reducePath


//-----------------------------------------------------------------------------
// getParentBranch
//
// - set parent branch of this object
// - check if the elements name is unique in parent
//
// returns:
//		TRUE  - branch set
//		FALSE - no unique name
//
GenericBranch *GenericElement::getParentBranch(void)
{ 
	if (m_parent)
		m_parent->addRef();

	return m_parent; 
} // getParentBranch


//-----------------------------------------------------------------------------
// setParentBranch
//
// - set parent branch of this object
// - check if the elements name is unique in parent
//
// returns:
//		TRUE  - branch set
//		FALSE - no unique name
//
BOOL GenericElement::setParentBranch(
	IN GenericBranch *parent)	// parent branch
{
	_TRACE(TL_DEB, TG_TREE, (_T("> setParent 0x%X"), parent));
    if (m_parent == parent) 
        return TRUE;    // no change to before, -> no check necessary

    if (parent != NULL) 
	{
		// check if the elements name is unique in parent branch
        if (parent->isUniqueChildName(m_name, this))
		{
			parent->addRef();
			onSetParentBranch(parent);
			if (m_parent)
				m_parent->release();
			m_parent = parent;
			_TRACE(TL_INF, TG_TREE, (_T("parent=%s"), parent->getObjTraceId()));
			return TRUE;
		}
		else
		{
			_TRACE(TL_INF, TG_TREE, (_T("parent=NULL; no unique child name")));
			onSetParentBranch(NULL);
			if (m_parent)
				m_parent->release();
			m_parent = NULL;
			return FALSE;
		}
	}
	else
	{
		_TRACE(TL_INF, TG_TREE, (_T("parent=NULL")));
		onSetParentBranch(NULL);
		if (m_parent)
			m_parent->release();
		m_parent = NULL;
		return TRUE;
	}
} // setParentBranch


//-----------------------------------------------------------------------------
// getObjTraceId
//
// - get objects trace string id
//
// returns:
//		trace id string
//
LPCTSTR GenericElement::getObjTraceId(void)
{ 
	if (m_objectTraceId.IsEmpty())
	{
		m_objectTraceId.Format(_T("[%s|%8.8X| %s]"), getObjTypeString(), this, getName());
		m_objectTraceId.FreeExtra();
	}

	return m_objectTraceId; 
} // getObjTraceId


//-----------------------------------------------------------------------------
// hasKey
//
// - compare objects unique key (name) with given string
//
// returns:
//		TRUE  - object has the key
//      FALSE - object has not the key
//
BOOL GenericElement::hasKey(
	IN LPCTSTR name)	// object key
{
	return (_tcscmp(name, (LPCTSTR)m_name) == 0);
} // hasKey


//-----------------------------------------------------------------------------
// getKey
//
// - get objects unique key (name)
//
// returns:
//		object key
//
CString GenericElement::getKey(void)
{ 
	return m_name; 
} // getKey




//-----------------------------------------------------------------------------
// GenericLeaf                                                                |
//-----------------------------------------------------------------------------

GenericLeaf::GenericLeaf(
	IN DWORD objType)	// object type
 : GenericElement(objType | GENOBJ_TYPE_LEAF)
{}




//-----------------------------------------------------------------------------
// GenericBranch                                                              |
//-----------------------------------------------------------------------------

GenericBranch::GenericBranch(
	IN DWORD objType,						// object type
	IN OPTIONAL DWORD supportedChildObj,	// supported child object types
	IN OPTIONAL BYTE whatLeafList,			// generic list type of leaf list
	IN OPTIONAL BYTE whatBranchList,		// generic list type of branch list
	IN OPTIONAL int blockSizeLeafList,		// block size of leaf list and map
	IN OPTIONAL int blockSizeBranchList,	// block size of branch list and map
	IN OPTIONAL int hashSizeLeafList,		// hash table size of leaf map
	IN OPTIONAL int hashSizeBranchList)		// hash table size of branch map
 : GenericElement(objType | GENOBJ_TYPE_BRANCH)
{
	if ((supportedChildObj & GENOBJ_TYPE_BRANCH) == GENOBJ_TYPE_BRANCH)
	{
		m_branchList.create(whatBranchList, blockSizeBranchList, hashSizeBranchList);
	}
	if ((supportedChildObj & GENOBJ_TYPE_LEAF) == GENOBJ_TYPE_LEAF)
	{
		m_leafList.create(whatLeafList, blockSizeLeafList, hashSizeLeafList);
	}

	m_trustUniqueChilds = FALSE;
}

GenericBranch::~GenericBranch(void)
{
	m_leafList.free();
	m_branchList.free();
}


//-----------------------------------------------------------------------------
// addLeaf
//
// - adds leaf to leaf list
//
// returns:
//		TRUE  - unique name
//		FALSE - no unique name
//
BOOL GenericBranch::addLeaf(
	IN GenericLeaf *newLeaf,	// leaf
	IN OPTIONAL int index)		// index
{
	_TRACE(TL_DEB, TG_TREE, (_T("> addLeaf 0x%X"), newLeaf));
    if (!newLeaf)
		return FALSE;

	if (!m_leafList)
		return FALSE;

	if (!isUniqueChildName(newLeaf->getName(), newLeaf))
		return FALSE;

	GenericList<GenericLeaf> list(m_leafList);
    list.lock();
    // add at end of list?
	if (list.add(newLeaf))
		onAddLeaf(newLeaf);
    list.unlock();

	_TRACE(TL_INF, TG_TREE, (_T("added leaf %s"), newLeaf->getObjTraceId()));
    return newLeaf->setParentBranch(this);
} // addLeaf


//-----------------------------------------------------------------------------
// addBranch
//
// - adds branch to branch list
//
// returns:
//		TRUE  - unique name
//		FALSE - no unique name
//
BOOL GenericBranch::addBranch(
	IN GenericBranch *newBranch,	// branch
	IN OPTIONAL int index)			// index
{
	_TRACE(TL_DEB, TG_TREE, (_T("> addBranch 0x%X"), newBranch));
    if (!newBranch)
		return FALSE;

	if (!m_branchList)
		return FALSE;

	if (!isUniqueChildName(newBranch->getName(), newBranch))
		return FALSE;

	GenericList<GenericBranch> list(m_branchList);
    list.lock();
    // add at end of list?
	if (list.add(newBranch))
		onAddBranch(newBranch);
    list.unlock();

	_TRACE(TL_INF, TG_TREE, (_T("added branch %s"), newBranch->getObjTraceId()));
    return newBranch->setParentBranch(this);
} // addBranch


//-----------------------------------------------------------------------------
// removeLeaf
//
// - removes leaf from leaf list
//
// returns:
//		TRUE  - removed
//		FALSE - not in list
//
BOOL GenericBranch::removeLeaf(
	IN GenericLeaf *leaf,		// leaf
	IN OPTIONAL BOOL doLock)	// lock leaf list?
{
	BOOL ret;

	if (!m_leafList)
		return FALSE;

	GenericList<GenericLeaf> list(m_leafList);
	if (doLock)
		list.lock();
	else
		list.dontLock();

	leaf->addRef();
	if (ret	= list.remove(leaf))
	{
		_TRACE(TL_INF, TG_TREE, (_T("removed leaf %s"), leaf->getObjTraceId()));
		onRemoveLeaf(leaf);		
		leaf->setParentBranch(NULL);
	}
	leaf->release();
    list.unlock();

	return ret;
} // removeLeaf


//-----------------------------------------------------------------------------
// removeBranch
//
// - removes branch from branch list
//
// returns:
//		TRUE  - removed
//		FALSE - not in list
//
BOOL GenericBranch::removeBranch(
	IN GenericBranch *branch,	// branch
	IN OPTIONAL BOOL doLock)	// lock branch list?
{
	BOOL ret;

	if (!m_branchList)
		return FALSE;

	GenericList<GenericBranch> list(m_branchList);
	if (doLock)
		list.lock();
	else
		list.dontLock();

	branch->addRef();
	if (ret	= list.remove(branch))
	{
		_TRACE(TL_INF, TG_TREE, (_T("removed branch %s"), branch->getObjTraceId()));
		onRemoveBranch(branch);		
		branch->setParentBranch(NULL);
	}
	branch->release();
    list.unlock();

	return ret;
} // removeBranch


//-----------------------------------------------------------------------------
// removeAllLeafs
//
// - remove all leafs from the leaf list
//
void GenericBranch::removeAllLeafs(
	IN OPTIONAL BOOL deep)	// remove all sub leafs too
{
	GenericLeaf *leaf;
	POSITION pos;

	_TRACE(TL_INF, TG_TREE, (_T("remove all leafs")));

	if (!(!m_leafList))
	{
		GenericList<GenericLeaf> list(m_leafList);

		list.lock();
		pos = list.getStartPosition();
		while(pos)
		{
			leaf = list.getNext(pos);		
			onRemoveLeaf(leaf);		
			leaf->setParentBranch(NULL);
			list.release(leaf);
		}
		list.removeAll();
		list.unlock();
	}

	if (deep)
	{		
		if (!(!m_branchList))
		{
			GenericList<GenericBranch> list(m_branchList);
			GenericBranch *branch;

			list.lock();
			pos = list.getStartPosition();
			while(pos)
			{
				branch = list.getNext(pos);
				branch->removeAllLeafs(TRUE);
			}
			list.unlock();
		}
	}	
	_TRACE(TL_DEB, TG_TREE, (_T("< removeAllLeafs")));
} // removeAllLeafs


//-----------------------------------------------------------------------------
// removeAllBranches
//
// - remove all branches from the branches list
//
void GenericBranch::removeAllBranches(
	IN OPTIONAL BOOL deep)	// remove all sub branches too
{
	GenericBranch *branch;
	POSITION pos;

	_TRACE(TL_INF, TG_TREE, (_T("remove all branches")));
	if (!(!m_branchList))
	{
		GenericList<GenericBranch> list(m_branchList);

		list.lock();
		pos = list.getStartPosition();
		while(pos)
		{
			branch = list.getNext(pos);		
			if (deep)
				branch->removeAllBranches(TRUE);
			onRemoveBranch(branch);
			branch->setParentBranch(NULL);
			list.release(branch);
		}
		list.removeAll();
		list.unlock();
	}
	_TRACE(TL_DEB, TG_TREE, (_T("< removeAllBranches")));
} // removeAllBranches


//-----------------------------------------------------------------------------
// findLeaf
//
// - find leaf below the branch
//
// returns:
//		pointer to the leaf - leaf found 
//		NULL - no leaf found
//
GenericLeaf *GenericBranch::findLeaf(
	IN LPCTSTR name,		// name of leaf
	IN OPTIONAL BOOL deep)	// deep search or only direct child
{
	GenericLeaf *leaf = NULL;
	CString path;
    LPCTSTR child = NULL;
    LPCTSTR remainingPath = NULL;

	_TRACE(TL_DEB, TG_TREE, (_T("find leaf %s deep:%s"), name, BOOL2STR(deep)));
    if (!name)
		return NULL;

	path = name;
    reducePath(path, child, remainingPath, TRUE);	// slip path

	if ((!deep) && (remainingPath))	// no deep search and remaining path
		return NULL;				// -> name can't be a direct child of this node

	if ((!deep) || (!remainingPath))	// no deep search or no remaining path
	{								// -> lookup name in leaf list
		if (!(!m_leafList))
		{
			GenericList<GenericLeaf> list(m_leafList);

			leaf = list.find(name);
			if (leaf)
			{
				_TRACE(TL_INF, TG_TREE, (_T("found leaf %s"), leaf->getObjTraceId()));
			}
		}
	}
	else							// deep search and remaining path
	{								// -> search for child branch and find inside of this branch
		if (!(!m_branchList))
		{
			GenericList<GenericBranch> list(m_branchList);
			GenericPointer<GenericBranch> branch;

			branch = list.find(child);
			list.unlock();
			if (!(!branch))
			{
				leaf = branch->findLeaf(remainingPath, TRUE);
			}
		}
	}	

	return leaf;
} // findLeaf


//-----------------------------------------------------------------------------
// findBranch
//
// - find branch below the branch
//
// returns:
//		pointer to the branch - branch found 
//		NULL - no branch found
//
GenericBranch *GenericBranch::findBranch(
	IN LPCTSTR name,		// name of branch
	IN OPTIONAL BOOL deep)	// deep search or only direct child
{
	GenericBranch *branch = NULL;
	CString path;
    LPCTSTR child = NULL;
    LPCTSTR remainingPath = NULL;

	_TRACE(TL_DEB, TG_TREE, (_T("find branch %s deep:%s"), name, BOOL2STR(deep)));
    if (!name)
		return NULL;

	path = name;
    reducePath(path, child, remainingPath, TRUE);	// slip path

	if ((!deep) && (remainingPath))	// no deep search and remaining path
		return NULL;				// -> name can't be a direct child of this node

	if (!(!m_branchList))
	{
		GenericList<GenericBranch> list(m_branchList);
		GenericPointer<GenericBranch> search;

		search = list.find(child);
		if (!(!search))
		{
			if ((!deep) || (!remainingPath))	
			{ // no deep search or no remaining path
				branch = search;
				branch->addRef();
				_TRACE(TL_INF, TG_TREE, (_T("found branch %s"), branch->getObjTraceId()));
			}
			else							
			{ // deep search and remaining path
				branch = search->findBranch(remainingPath, TRUE);
			}
		}
	}

	return branch;
} // findBranch


//-----------------------------------------------------------------------------
// isUniqueChildName
//
// - check if the name is unique among all childs
//
// returns:
//		TRUE  - unique name
//		FALSE - no unique name
//
BOOL GenericBranch::isUniqueChildName(
	IN LPCTSTR name,					// name 
	IN OPTIONAL GenericElement *child)	// child to ignore for check
{
	if (!name)
		return FALSE;

	GenericPointer<GenericLeaf> leaf;
	GenericPointer<GenericBranch> branch;	

	_TRACE(TL_DEB, TG_TREE, (_T("isUniqueChildName %s 0x%X"), name, child));
	if (FAILED(validateName(name, FALSE)))
		return FALSE;	// no valid name

	if (m_trustUniqueChilds == TRUE)
		return TRUE;

	// check for leaf with this name
	if ((GenericLeaf *)(leaf = findLeaf(name, FALSE)))
	{
		if ((GenericElement *)leaf == (GenericElement *)child)
		{
			_TRACE(TL_INF, TG_TREE, (_T("unique child %s"), name));
			return TRUE;
		}
		else	
		{
			_TRACE(TL_INF, TG_TREE, (_T("not unique child %s"), name));
			return FALSE;
		}
	}

	// check for branch with this name
	if ((GenericBranch *)(branch = findBranch(name, FALSE)))
	{
		if ((GenericElement *)branch == (GenericElement *)child)
		{
			_TRACE(TL_INF, TG_TREE, (_T("unique child %s"), name));
			return TRUE;
		}
		else	
		{
			_TRACE(TL_INF, TG_TREE, (_T("not unique child %s"), name));
			return FALSE;
		}
	}

	_TRACE(TL_INF, TG_TREE, (_T("unique child %s"), name));
	return TRUE;
} // isUniqueChildName


//-----------------------------------------------------------------------------
// changeChildName
//
// - change the name of a child object in the map
//
// returns:
//		S_OK         - map entry changed
//		E_FAIL       - can't remove object with previous name
//		E_INVALIDARG - invalid argument
//
HRESULT GenericBranch::changeChildName(
	IN GenericElement *child,	// child element
	IN LPCTSTR name)			// previous child name
{
	if ((!child) || (!name))
		return E_INVALIDARG;

	if (child->is(GENOBJ_TYPE_LEAF))
	{
		GenericList<GenericLeaf> list(m_leafList);
		CTypedPtrMap<CMapStringToPtr, CString, GenericLeaf*> *map;
		GenericLeaf *leaf = (GenericLeaf *)child;

		if ((map = list.getMap()) == NULL)
			return S_OK;

		list.lock();
		if (map->Lookup(name, leaf))
		{	
			map->RemoveKey(name);
			map->SetAt(leaf->getKey(), leaf);
		}
		list.unlock();
	}
	else
	{
		GenericList<GenericBranch> list(m_branchList);
		CTypedPtrMap<CMapStringToPtr, CString, GenericBranch*> *map;
		GenericBranch *branch = (GenericBranch *)child;

		if ((map = list.getMap()) == NULL)
			return S_OK;

		list.lock();
		if (map->Lookup(name, branch))
		{	
			map->RemoveKey(name);
			map->SetAt(branch->getKey(), branch);
		}
		list.unlock();
	}

	return S_OK;
} // changeChildName


//-----------------------------------------------------------------------------
// lockLeafList
//
void GenericBranch::lockLeafList(void)
{
	CSyncObject *lock = m_leafList.getLockObject();
	if (lock)
		lock->Lock();
} // lockLeafList


//-----------------------------------------------------------------------------
// lockBranchList
//
void GenericBranch::lockBranchList(void)
{
	CSyncObject *lock = m_branchList.getLockObject();
	if (lock)
		lock->Lock();
} // lockBranchList


//-----------------------------------------------------------------------------
// unlockLeafList
//
void GenericBranch::unlockLeafList(void)
{
	CSyncObject *lock = m_leafList.getLockObject();
	if (lock)
		lock->Unlock();
} // lockLeafList


//-----------------------------------------------------------------------------
// unlockBranchList
//
void GenericBranch::unlockBranchList(void)
{
	CSyncObject *lock = m_branchList.getLockObject();
	if (lock)
		lock->Unlock();
} // lockBranchList


//-----------------------------------------------------------------------------
// onAddLeaf
//
void GenericBranch::onAddLeaf(
	IN GenericLeaf *leaf)	// leaf
{} // onAddLeaf


//-----------------------------------------------------------------------------
// onRemoveLeaf
//
void GenericBranch::onRemoveLeaf(
	IN GenericLeaf *leaf)	// leaf
{} // onRemoveLeaf


//-----------------------------------------------------------------------------
// onAddBranch
//
void GenericBranch::onAddBranch(
	IN GenericBranch *branch)	// Branch
{} // onAddBranch


//-----------------------------------------------------------------------------
// onRemoveBranch
//
void GenericBranch::onRemoveBranch(
	IN GenericBranch *branch)	// Branch
{} // onRemoveBranch

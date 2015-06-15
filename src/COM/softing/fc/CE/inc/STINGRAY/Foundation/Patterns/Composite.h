/////////////////////////////////////////////////////////////////////////////
// Composite.h : SFL Composite design pattern
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  SFL Composite design pattern templated implementation
//


#pragma once


#if !defined(_SFL)
#define _SFL
#endif

#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning(push,3)
	#endif
	#pragma warning(disable:4876)
	#pragma warning(disable:4284) // STL, not a UDT or reference to a UDT. Errors if infix notation used.
#endif

#include <list>
#include <algorithm>
#include <foundation\GuidCast.h>
#include <foundation\Patterns\Iterator.h>


namespace stingray {
namespace foundation {


// Disable the warning that complains about the symbols name length 


//@doc 

//@class CComposite |
/*
	Implements the composite design pattern
*/
//@tcarg typename | _Component | Type of the elements in the composite structure
//@tcarg GUID | guid | GUID that identifies the Composite interface among all the interfaces implemented by an element
template <typename _Component, const GUID* _guid>
class CComposite:
	public IQueryGuid
{
	typedef CComposite<_Component, _guid> _thisClass;
	typedef std::list<_Component* > Children;

	class CompIterator:
		public Iterator<Children>
	{
	public:
		friend class _thisClass;
		explicit CompIterator(Children& acoll): foundation::Iterator<Children>(acoll) {}
	};


public:

	typedef foundation::IteratorBase<_Component*> Iterator;

	// Composite design pattern operations

	//@cmember
	// Returns the number of nodes directly contained in this node
	int GetChildrenCount () const;
	//@cmember
	// Returns the total number of nodes contained by this node and its children
	int GetAllDescendantsCount () const;


	//@cmember
	// Returns this node's parent
	_Component* GetParent() const;
	//@cmember
	// Sets the parent node
	void SetParent(_Component* pComp);
	//@cmember
	// Returns the root of the composite tree
	_Component* GetRoot();

	//@cmember
	// Adds a node as a child of this node
	int AddChild(_Component* pComp);
	//@cmember
	// Removes a child node
	int RemoveChild(_Component* pComp);

	//@cmember
	// Returns an iterator of this node's children
	Iterator* GetChildIterator();

protected:

	CComposite(): m_pParent(NULL) {}
	virtual ~CComposite() {}

protected:
	// Composite design pattern implementation
	Children m_Children;
	_Component* m_pParent;

};



//@mfunc int | CComposite<lt><gt> | GetChildrenCount | 
// Returns the number of nodes directly contained in this node
//@rdesc Count of child nodes
template <typename _Component, const GUID* _guid>
int 
CComposite<_Component, _guid>::GetChildrenCount (
) const
{
	int nCount = 0;
	nCount = (int)m_Children.size();
	return nCount;
}


//@mfunc int | CComposite<lt><gt> | GetAllDescendantsCount | 
// Returns the total number of nodes contained by this node and its children
//@rdesc Count of child nodes
//@comm GetAllDescendantsCount recursively counts the number of nodes in this
// node and its children.
template <typename _Component, const GUID* _guid>
int 
CComposite<_Component, _guid>::GetAllDescendantsCount (
) const
{
	int nCount = GetChildrenCount();
	Children::const_iterator i;
	for (i = m_Children.begin(); i != m_Children.end(); ++i) {
#if _MSC_VER > 1100
		const _thisClass* pComposite = guid_cast<const _thisClass*, _guid>(*i);
		nCount += pComposite->GetAllDescendantsCount();
#else
		const _thisClass* pComposite;
		if ((*i)->QueryGuid(*_guid, (void**) &pComposite))
		{
			nCount += pComposite->GetAllDescendantsCount();
		}
#endif
	}
	return nCount;
}


//@mfunc _Component* | CComposite<lt><gt> | GetParent | 
// Returns this node's parent node
//@rdesc Pointer to parent node
template <typename _Component, const GUID* _guid>
_Component* 
CComposite<_Component, _guid>::GetParent (
) const
{ 
	return m_pParent; 
}


//@mfunc void | CComposite<lt><gt> | SetParent | 
// Sets this node as child of the given node
//@parm _Component* | pComp | Parent node
template <typename _Component, const GUID* _guid>
void 
CComposite<_Component, _guid>::SetParent (
	_Component* pComp
)
{ 
	m_pParent = pComp; 
}


//@mfunc _Component* | CComposite<lt><gt> | GetRoot | 
// Returns the root of tree-like structure the composite defines
//@rdesc Root node of the composite
template <typename _Component, const GUID* _guid>
_Component* 
CComposite<_Component, _guid>::GetRoot (
) 
{
	_thisClass* pRoot = this;
	_Component* pParent = 0;
	do {
		pParent = pRoot->GetParent();
		if (pParent) {
#if _MSC_VER > 1100
			pRoot = guid_cast<_thisClass*, _guid>(pParent);
#else
			pParent->QueryGuid(*_guid, (void**) &pRoot);
#endif
		}
	} while (pParent);

	return guid_cast<_Component*>(pRoot);
}


//@mfunc int | CComposite<lt><gt> | AddChild | 
// Adds the given node to this node's list of children 
//@rdesc The number of children this node contains after inserting the new one
//@parm _Component* | pComp | New child node
template <typename _Component, const GUID* _guid>
int 
CComposite<_Component, _guid>::AddChild (
	_Component* pComp
) 
{
	const UUID& uuid = __uuidof(CComposite);
#if _MSC_VER > 1100
	_thisClass* pNode = guid_cast<_thisClass*, _guid>(pComp);
	_thisClass* pParent = guid_cast<_thisClass*, _guid>(pNode->GetParent());
#else
	_thisClass* pNode;
	_thisClass* pParent;
	pComp->QueryGuid(*_guid, (void**)&pNode);
	pNode->GetParent()->QueryGuid(*_guid, (void**)&pParent);
#endif
	if (pParent) {
		if (pParent == this) {
			return GetChildrenCount();	// already a child!
		}
		else {
			pParent->RemoveChild(pComp);
		}
	}

	// Add the child node
	try {
		m_Children.push_back(pComp);
		pNode->SetParent(guid_cast<_Component*>(this));
	}
	catch (std::exception& ) {
		return -1;
	}

	return GetChildrenCount();
}


//@mfunc int | CComposite<lt><gt> | RemoveChild | 
// Removes the given node from this node's list of children 
//@rdesc The number of children this node contains after removing the one requested
//@parm _Component* | pComp | Node to remove
template <typename _Component, const GUID* _guid>
int 
CComposite<_Component, _guid>::RemoveChild (
	_Component* pComp
)
{
	if (!pComp) {
		return -1;
	}

#if _MSC_VER > 1100
	_thisClass* pNode = guid_cast<_thisClass*, _guid>(pComp);
#else
	_thisClass* pNode;
	pComp->QueryGuid(*_guid, (void**)&pNode);
#endif

	// Remove the child
	try {
		Children::iterator pos = std::find(m_Children.begin(), m_Children.end(), pComp);
		if (pos != m_Children.end()) {
			m_Children.erase(pos);
			pNode->SetParent(NULL);
		}
	}
	catch (std::exception& ) {
		return -1;
	}

	return GetChildrenCount();
}


//@mfunc Iterator* | CComposite<lt><gt> | GetChildIterator | 
// Returns an iterator that allows traversing this node's collection of child nodes
//@rdesc Iterator for collection of children
template <typename _Component, const GUID* _guid>
CComposite<_Component, _guid>::Iterator*
CComposite<_Component, _guid>::GetChildIterator()
{
	return new CompIterator(m_Children);
}



};	// namespace stingray::foundation
};	// namespace stingray



#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning (pop)
	#endif
#endif
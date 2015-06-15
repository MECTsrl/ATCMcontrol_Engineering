/////////////////////////////////////////////////////////////////////////////
// Composite.h : Declaration of SEC Composite Patterns
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
// Authors:      Mark Isham
// Description:  Declaration of SEC Composite Patterns
//

#ifndef __SFL_SECCOMPOSITE_PATTERN_H__
#define __SFL_SECCOMPOSITE_PATTERN_H__

#ifdef WIN32

// Header Includes
#ifndef __SFL_SECCOLLECT_H__
#include "Collect.h"
#endif

//
// CMN Extension DLL
// Initialize declaration context
//
#ifdef /*_CMNDLL*/ _SFLDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SFL_DATAEXT
#endif //_SFLDLL

namespace stingray {
namespace foundation {

// Constants

// Macros

// The Composite pattern makes the assumption that all children/parent links
// established via the SECComposite_T interface are all properly derived
// from SECComposite_T. If this is not done, then type safety will be 
// compromised. If RunTime Type Information (RTTI) support is enabled, then
// the following macro will expand to a debug assertion that will be useful
// in establishing if proper type safety has been established. If no RTTI,
// then we must assume the consumer has properly behaved.
#ifdef _CPPRTTI
#define RTTI_ASSERT(x) ASSERT(x)
#else
#define RTTI_ASSERT(x)
#endif

// Types

// Forward Declarations

/////////////////////////////////////////////////////////////////////////////
// ITreeCollection_T

template <class element_t> 
class ITreeCollection_T : public ICollection_T<element_t> 
{
public:
	virtual BOOL iGetParent(CCIterPos* pbp) const = 0;
};

/////////////////////////////////////////////////////////////////////////////
// IteratorPreOrder_T

//
// PreOrder traversal of a collection that supports ITreeCollection_T 
//

template <class element_t> 
class IteratorPreOrder_T : public Iterator_T<element_t> {
public:
	IteratorPreOrder_T<element_t>(ITreeCollection_T<element_t>* pCollection);

	virtual void First() CMN_ITERATOR_CONST;
	virtual void Last() CMN_ITERATOR_CONST;
	virtual void Next() CMN_ITERATOR_CONST;
	virtual void Prev() CMN_ITERATOR_CONST;
	virtual BOOL EOC() CMN_ITERATOR_CONST;

protected:
	element_t m_SubTreeOverFlow;
	element_t m_SubTreeUnderFlow;
};

template <class element_t>
IteratorPreOrder_T<element_t>::IteratorPreOrder_T(ITreeCollection_T<element_t>* pCollection) :
	Iterator_T<element_t>(pCollection)
{

	// Cache the parent of the root as the terminating overflow condition.
	// This is necessary in case we are iterating a subtree, rather than
	// an entire tree. The parent of the root of the subtree will be the
	// overflow condition (NULL for an entire tree).
	CCIterPos_T<element_t> posTest=(element_t)pCollection;
	pCollection->iGetParent(&posTest);
	m_SubTreeOverFlow=posTest;

	// Also cache the leftmost (firstmost) child, as this will be 
	// the underflow condition for reversed traversal of a subtree.
	posTest=(element_t)pCollection;
	pCollection->iFirst(&posTest);
	CCIterPos_T<element_t> posChildChild=posTest;
	while(posChildChild.m_value) {
		posTest=posChildChild;
		posTest.m_value->iFirst(&posChildChild);
		}
	m_SubTreeUnderFlow=posTest;
}

// Locate the first element in a preorder traversal
template <class element_t>
void IteratorPreOrder_T<element_t>::First() CMN_ITERATOR_CONST {

	// Get position of first child
	CCIterPos_T<element_t> posChild;
	m_pCollection->iFirst(&posChild);

	// For PreOrder, the first entry is the leaf node down the 
	// left-most (first-most) branch
	CCIterPos_T<element_t> posChildChild=posChild;
	while(posChildChild.m_value) {
		posChild=posChildChild;
		posChild.m_value->iFirst(&posChildChild);
		}

	*((CCIterPos_T<element_t>*)m_pPos)=posChild.m_value;
}

// Locate the last element in a preorder traversal
template <class element_t>
void IteratorPreOrder_T<element_t>::Last() CMN_ITERATOR_CONST {
	RTTI_ASSERT(dynamic_cast<element_t>(m_pCollection));	// element should be derived from collection
	element_t pElement=(element_t)(m_pCollection);

	*((CCIterPos_T<element_t>*)m_pPos)=pElement;
}

// Get the next element in a preorder traversal
template <class element_t>
void IteratorPreOrder_T<element_t>::Next() CMN_ITERATOR_CONST {

	// PreOrder will always traverse through leaf nodes first,
	// then iterate back to the parents...
	RTTI_ASSERT(dynamic_cast<CCIterPos_T<element_t>*>(m_pPos));	
	CCIterPos_T<element_t>* pPos=(CCIterPos_T<element_t>*)(m_pPos);

	// If this ASSERT fires, you are attempting to iterate 
	// past the end of the list!
	ASSERT(pPos->m_value);

	// Get a pointer to the parent node.
	CCIterPos_T<element_t> posParent=*pPos;
	pPos->m_value->iGetParent(&posParent);

	// Check bounding condition. If iterating a subtree, if the
	// parent position is equal to the cached overflow value,
	// then we are done with out iteration. If iterating an
	// entire tree, the parent value will be null at the end.
	if((posParent==m_SubTreeOverFlow)||(!posParent.m_value)) {
		// Return a NULL element to signify the end of iteration
		*pPos=(element_t)NULL;
		return;
	}

	// Locate the next child in sequence
	CCIterPos_T<element_t> posNextChild=*pPos;
	posParent.m_value->iNext(&posNextChild);
	if(!posNextChild.m_value) {
		// No more children to process in the parent, return parent itself.
		pPos->m_value=posParent;
		return;
		}

	// Process next child in parent's list. Recurse down to left-most leaf.
	CCIterPos_T<element_t> posLeafChild=posNextChild;
	while(posNextChild.m_value) {
		posLeafChild=posNextChild;
		posLeafChild.m_value->iFirst(&posNextChild);
		}

	// Leaf found, this is the next item in the iteration
	*pPos=posLeafChild.m_value;
}

// Get the previous element in a preorder traversal
template <class element_t>
void IteratorPreOrder_T<element_t>::Prev() CMN_ITERATOR_CONST {

	RTTI_ASSERT(dynamic_cast<CCIterPos_T<element_t>*>(m_pPos));	
	CCIterPos_T<element_t>* pPos=(CCIterPos_T<element_t>*)(m_pPos);

	// If this ASSERT fires, you are attempting to iterate 
	// past the start of the list!
	ASSERT(pPos->m_value);

	// If we are not at a leaf right now, then the last child 
	// (right-most) is the previous in our iteration.
	CCIterPos_T<element_t> posLastChild=*pPos;
	pPos->m_value->iLast(&posLastChild);
	if(posLastChild.m_value) {
		*pPos=posLastChild;
		return;
		}

	// Watch for underflow condition in case we are iterating
	// through a subtree, rather than a complete tree. In this
	// scenario return a NULL to signify the end of the traversal.
	if(*pPos==m_SubTreeUnderFlow) {
		*pPos=(element_t)NULL;
		return;
	}

	// Otherwise we must be at a leaf, move up 1 level, then back
	// down the previous child in the parent list (if there is a
	// previous child)

	// Get parent
	CCIterPos_T<element_t> posPrevChild=*pPos;
	CCIterPos_T<element_t> posParent=*pPos;
	pPos->m_value->iGetParent(&posParent);
	while(posParent.m_value) {
		posParent.m_value->iPrev(&posPrevChild);
		if(posPrevChild.m_value) {
			*pPos=posPrevChild;
			return;
			}
		posPrevChild=posParent;
		posParent.m_value->iGetParent(&posParent);
	}

	// End of iteration. No match, return NULL. (via null entry in posParent)
	*pPos=posParent;
}

template <class element_t>
BOOL IteratorPreOrder_T<element_t>::EOC() CMN_ITERATOR_CONST {
	// PreOrder will always traverse through leaf nodes first,
	// then iterate back to the parents...
	RTTI_ASSERT(dynamic_cast<CCIterPos_T<element_t>*>(m_pPos));	
	CCIterPos_T<element_t>* pPos=(CCIterPos_T<element_t>*)(m_pPos);

	if(!pPos->m_value) return TRUE;
	if((pPos->m_value==m_SubTreeUnderFlow)||(pPos->m_value==m_SubTreeOverFlow))
		return TRUE;

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// SECComposite_T

// Composite implementation based on the CObject POSITION based
// collection classes. This implementation also supports the
// ICollection_T interface such that it can be iterated via
// Iterator_T
template <class element_t>
class SECComposite_T : public IComposite_T<element_t,POSITION>,
					   public ITreeCollection_T<element_t>					   
{

public:
	//
	// SECComposite_T interface
	//
	SECComposite_T() { m_pParent=NULL; }
	virtual ~SECComposite_T() {}

public:
	// 
	// IComposite_T interface
	//

	//@cmember
	/* Return number of children in composite*/
	virtual int GetChildCount(BOOL bRecursive=FALSE) const;

	//@cmember
	/* Return a pointer to a child given the collection index*/
	virtual element_t GetChild(const POSITION pos) const;

	//@cmember
	/* Add a child to the end of the collection*/
	virtual BOOL AddChild(element_t pChild);

	//@cmember
	/* Insert a child into the composite at a given position*/
	virtual BOOL InsertChildAt(element_t pChild, const POSITION pos);

	//@cmember
	/* Remove specfied child*/
	virtual BOOL RemoveChild(const element_t pChild);

	//@cmember
	/* Remove a child at the specified position from the composite*/
	virtual BOOL RemoveChildAt(const POSITION pos);

	//@cmember
	/* Return the index of a given child in the composite*/
	virtual BOOL FindChild(element_t pChild, POSITION& pos) const;

	//@cmember
	/* Return head position*/
	virtual POSITION GetHeadChildPos() const { return m_listChildren.GetHeadPosition(); }

	//@cmember
	/* Return tail position*/
	virtual POSITION GetTailChildPos() const { return m_listChildren.GetTailPosition(); }

	//@cmember
	/* Return the next position in sequence*/
	virtual POSITION GetNextChildPos(POSITION posOld) const;

	//@cmember
	/* Return the previous position in sequence*/
	virtual POSITION GetPrevChildPos(POSITION posOld) const;

	//@cmember
	/* Set (or reset) this node's parent*/
	virtual void SetParent(element_t parent,BOOL bRemoveFromOldParent=TRUE);

	//@cmember
	/* Fetch this node's parent*/
	virtual element_t GetParent() const { return m_pParent; }

public:
	//
	// ITreeCollection_T interface:
	//

	// ICollection_T base support:
	virtual CCIterPos* iCreateIteratorPosition() const ;
	virtual void iFirst(CCIterPos* pbp) const;
	virtual void iNext(CCIterPos* pbp) const;
	virtual void iPrev(CCIterPos* pbp) const;
	virtual void iLast(CCIterPos* pbp) const;
	virtual element_t iGet(CCIterPos* pbp) const;
	virtual element_t* iGetPtr(CCIterPos* pbp) const;
	virtual element_t& iGetRef(CCIterPos* pbp) const;
	virtual BOOL iEOC(CCIterPos*) const;

	// ITreeCollection_T derived support
	virtual BOOL iGetParent(CCIterPos* pbp) const;

	// NOT IMPLEMENTED:
	virtual int  GetSize() const 						{ ASSERT(FALSE); return -1; }
	virtual void iRemoveAt(CCIterPos*) 					{ ASSERT(FALSE); }
	virtual void iSet(CCIterPos*,element_t) 			{ ASSERT(FALSE); }
	virtual element_t iInsert(CCIterPos*,element_t elem) { ASSERT(FALSE); return elem; }
	virtual void RemoveAll() 							{ ASSERT(FALSE); }
	virtual BOOL iFindNext(CCIterPos*,element_t) const	{ ASSERT(FALSE); return FALSE; }
	virtual BOOL iFindPrev(CCIterPos*,element_t) const	{ ASSERT(FALSE); return FALSE; }

protected:
	//
	// SECComposite_T implementation
	//
	CTypedPtrList<CPtrList,element_t> m_listChildren;
	element_t m_pParent;
};

//
// Implementation
//

// Return number of children in composite
template <class element_t>
int SECComposite_T<element_t>::GetChildCount(BOOL bRecursive) const {
	// get count of immediate children
	int nChildCount=m_listChildren.GetCount();

	if(bRecursive) {
		// and recurse if specified
		element_t pObject;
		POSITION pos=m_listChildren.GetHeadPosition();
		while(pos) {
			pObject=m_listChildren.GetNext(pos);
			nChildCount+=pObject->GetChildCount(bRecursive);
			}
		}
	return nChildCount;
}

// Return a pointer to a child given the collection index
template <class element_t>
element_t SECComposite_T<element_t>::GetChild(const POSITION pos) const {
	if(!pos) return (element_t)NULL;
	else	 return m_listChildren.GetAt(pos);
}

// Add a child to the end of the collection
template <class element_t>
BOOL SECComposite_T<element_t>::AddChild(element_t pChild) {
	if(!pChild) return FALSE;
	
	// Do not insert the same element twice!
	ASSERT(!m_listChildren.Find(pChild));

	// Add the new child
	m_listChildren.AddTail(pChild);

	// SECComposite_T is provided as a base type, thus "this"
	// must also be of type element_t. Verify this
	RTTI_ASSERT(dynamic_cast<element_t>(this));
	element_t pThis=(element_t)(this);	// use static for speed
	pChild->m_pParent=pThis;	// calling SetParent would loop infinitely

	return TRUE;
}

// Insert a child into the composite at a given position
template <class element_t>
BOOL SECComposite_T<element_t>::InsertChildAt(element_t pChild, const POSITION pos) {
	if(!pChild) return FALSE;

	// Do not insert the same element twice!
	ASSERT(!m_listChildren.Find(pChild));

	// Add the new child
	if(!pos)
		 m_listChildren.AddHead(pChild);
	else m_listChildren.InsertAfter(pos,pChild);

	// SECComposite_T is provided as a base type, thus "this"
	// must also be of type element_t. Verify this
	RTTI_ASSERT(dynamic_cast<element_t>(this));
	element_t pThis=(element_t)(this);	// use static for speed
	pChild->m_pParent=pThis;	// calling SetParent would loop infinitely

	return TRUE;
}

//@cmember
/* Remove specified child*/
template <class element_t>
BOOL SECComposite_T<element_t>::RemoveChild(const element_t pChild) {
	if(!pChild) return FALSE;

	// locate the child
	POSITION posFind=m_listChildren.Find(pChild);
	if(!posFind) return FALSE;

	// And remove
	VERIFY(RemoveChildAt(posFind));
	ASSERT(!m_listChildren.Find(pChild));	// child should only be listed once!

	return TRUE;
}

// Remove a child at the specified position from the composite
template <class element_t>
BOOL SECComposite_T<element_t>::RemoveChildAt(const POSITION pos) {
	if(!pos) return FALSE;

	// locate the child and nullify the parent link
	element_t pChild=m_listChildren.GetAt(pos);
	m_listChildren.RemoveAt(pos);	// Remove before SetParent to prevent possible infinite loop
	pChild->SetParent(NULL,FALSE);

	return TRUE;
}

// Return the index of a given child in the composite
template <class element_t>
BOOL SECComposite_T<element_t>::FindChild(element_t pChild, POSITION& pos) const {
	if(!pChild) return FALSE;
	
	pos=m_listChildren.Find(pChild);
	if(pos) 
		 return TRUE;
	else return FALSE;
}

template <class element_t>
POSITION SECComposite_T<element_t>::GetNextChildPos(POSITION posOld) const {
	// Make sure we are not already at the last position!
	ASSERT(posOld);

	m_listChildren.GetNext(posOld);
	return posOld;
}

template <class element_t>
POSITION SECComposite_T<element_t>::GetPrevChildPos(POSITION posOld) const {
	// Make sure we are not already at the first position!
	ASSERT(posOld);

	m_listChildren.GetPrev(posOld);
	return posOld;
}

//@cmember
/* Set (or reset) this node's parent*/
template <class element_t>
void SECComposite_T<element_t>::SetParent(element_t parent,BOOL bRemoveFromOldParent) {

	// If remove from old parent flag set, also perform a removal on
	// top of the parent link reset.
	if(bRemoveFromOldParent) {
		RTTI_ASSERT(dynamic_cast<element_t>(this));
		element_t pThis=(element_t)(this);

		// Remove from old parent
		if(m_pParent)
			m_pParent->RemoveChild(pThis);

		// Add to new parent
		if(parent)
			parent->AddChild(pThis);
		}

	// And set the internal link
	m_pParent=parent;
}

//
// Iteration support
// (ICollection interface)

template <class element_t>
CCIterPos* SECComposite_T<element_t>::iCreateIteratorPosition() const {
	CCIterPos_T<element_t>* pIterPos=new CCIterPos_T<element_t>;
	
	// Initialize in the first position
	iFirst(pIterPos);
	return pIterPos;
}

// Get the first child of this composite node.
template <class element_t>
void SECComposite_T<element_t>::iFirst(CCIterPos* pbp) const {
	ASSERT(pbp);

	RTTI_ASSERT(dynamic_cast<CCIterPos_T<element_t>*>(pbp));	
	CCIterPos_T<element_t>* pp=(CCIterPos_T<element_t>*)(pbp);	

	POSITION pos=GetHeadChildPos();
	if(pos) 
		 *pp=GetChild(pos);
	else *pp=(element_t)NULL;
}

template <class element_t>
void SECComposite_T<element_t>::iLast(CCIterPos* pbp) const {
	ASSERT(pbp);

	RTTI_ASSERT(dynamic_cast<CCIterPos_T<element_t>*>(pbp));	
	CCIterPos_T<element_t>* pp=(CCIterPos_T<element_t>*)(pbp);

	POSITION pos=GetTailChildPos();
	if(pos) 
		 *pp=GetChild(pos);
	else *pp=(element_t)NULL;
}

// Locate next child element 
template <class element_t>
void SECComposite_T<element_t>::iNext(CCIterPos* pbp) const {
	ASSERT(pbp);

	RTTI_ASSERT(dynamic_cast<CCIterPos_T<element_t>*>(pbp));	
	CCIterPos_T<element_t>* pp=(CCIterPos_T<element_t>*)(pbp);
	
	POSITION posOld,posNew;
	FindChild(*pp,posOld);
	if(posOld) {
		posNew=GetNextChildPos(posOld);
		*pp=GetChild(posNew);
		}
	else *pp=(element_t)NULL;
}

// Locate prev child element 
template <class element_t>
void SECComposite_T<element_t>::iPrev(CCIterPos* pbp) const {
	ASSERT(pbp);

	RTTI_ASSERT(dynamic_cast<CCIterPos_T<element_t>*>(pbp));	
	CCIterPos_T<element_t>* pp=(CCIterPos_T<element_t>*)(pbp);
	
	POSITION posOld,posNew;
	FindChild(*pp,posOld);
	if(posOld) {
		posNew=GetPrevChildPos(posOld);
		*pp=GetChild(posNew);
		}
	else *pp=(element_t)NULL;
}

// Fetch the element at the current iterator position
template <class element_t>
element_t SECComposite_T<element_t>::iGet(CCIterPos* pbp) const {
	ASSERT(pbp);
	RTTI_ASSERT(dynamic_cast<CCIterPos_T<element_t>*>(pbp));	
	CCIterPos_T<element_t>* pp=(CCIterPos_T<element_t>*)(pbp);

	return *pp;
}

// Fetch the element at the current iterator position and return a pointer to it
template <class element_t>
element_t* SECComposite_T<element_t>::iGetPtr(CCIterPos* pbp) const {
	ASSERT(pbp);
	RTTI_ASSERT(dynamic_cast<CCIterPos_T<element_t>*>(pbp));	
	CCIterPos_T<element_t>* pp=(CCIterPos_T<element_t>*)(pbp);

	return &(pp->m_value);
}

template <class element_t>
element_t& SECComposite_T<element_t>::iGetRef(CCIterPos* pbp) const {
	ASSERT(pbp);
	RTTI_ASSERT(dynamic_cast<CCIterPos_T<element_t>*>(pbp));	
	CCIterPos_T<element_t>* pp=(CCIterPos_T<element_t>*)(pbp);

	return (pp->m_value);
}

template <class element_t>
BOOL SECComposite_T<element_t>::iEOC(CCIterPos* pbp) const {
	ASSERT(pbp);
	RTTI_ASSERT(dynamic_cast<CCIterPos_T<element_t>*>(pbp));	
	CCIterPos_T<element_t>* pp=(CCIterPos_T<element_t>*)(pbp);

	if(!(pp->m_value))
		 return TRUE;
	else return FALSE;
}

// Set the iterator position to that of its parent. Return FALSE
// if iterator is already at root, or if iterator was null.
template <class element_t>
BOOL SECComposite_T<element_t>::iGetParent(CCIterPos* pbp) const {
	if(!pbp) return FALSE;				// error, no iterator

	RTTI_ASSERT(dynamic_cast<CCIterPos_T<element_t>*>(pbp));	
	CCIterPos_T<element_t>* pp=(CCIterPos_T<element_t>*)(pbp);

	if(!(*pp)) return FALSE;		// error, already at root

	*pp=pp->m_value->GetParent();
	return TRUE;
}

};  // namespace stingray::foundation
};  // namespace stingray

/////////////////////////////////////////////////////////////////////////////

//
// CMN Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif	// WIN32

#endif	//__SFL_SECCOMPOSITE_PATTERN_H__


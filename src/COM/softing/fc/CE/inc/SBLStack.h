#ifndef __SBLSTACK_H_
#define __SBLSTACK_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//------------------------------------------------------------------*
/**
 *  class CSBLStack
 *
 *   simple stack class
 *
 *   - used in CSBLLexTree for traversing
 *
 *  @see    CSBLLexTree
 */
template<class T>
class CSBLStack
{
// public methods
public:

    // standard constructor
    CSBLStack();
    ~CSBLStack();

    // push on stack
    void push(const T& crtData);
    // pop from stack
    BOOL pop(T& rtData);
    
    // is stack empty ?
    bool isEmpty();

    // empty stack
    void empty();

// protected methods
protected:

    // initialization
    void init();

// protected members
protected:

    // internal node class
    class CSBLStackNode
    {
    public:
        // inline constructor
        CSBLStackNode() {};
        ~CSBLStackNode() {};

    public:
        T              m_tData;
        CSBLStackNode*  m_pNext;
    };

    // link to first node
    CSBLStackNode*      m_pHead;
    // link to stack end
    //CSBLStackNode*      m_pTail;

};

//------------------------------------------------------------------*
/**
 * constructor.
 *
 *  Calls init() to initialize stack.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             init()
*/
template <class T> 
CSBLStack<T>::CSBLStack()
{
    init();
}

//------------------------------------------------------------------*
/**
 * destructor.
 *
 *  Calls empty() and deletes head node.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             empty()
*/
template <class T>
CSBLStack<T>::~CSBLStack()
{
    empty();
    delete m_pHead; m_pHead = 0;
    //delete m_pTail; m_pTail = 0;
}

//------------------------------------------------------------------*
/**
 * init.
 *
 *  Creates head node and initializes it.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
template <class T> 
void CSBLStack<T>::init()
{
    m_pHead = new CSBLStackNode;
    //m_pTail = new CSBLStackNode;
    m_pHead->m_pNext = 0;
    //m_pTail->m_pNext = m_pTail;
}


//------------------------------------------------------------------*
/**
 * is stack empty?.
 *
 * @param           -
 * @return          is stack empty?
 * @exception       -
 * @see             -
*/
template <class T> 
bool CSBLStack<T>::isEmpty()
{
    if(m_pHead->m_pNext == 0)
        return true;
    return false;
}

//------------------------------------------------------------------*
/**
 * empty stack.
 *
 *  Deletes all nodes.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
template <class T> 
void CSBLStack<T>::empty()
{
    T   dummy;
    while(pop(dummy));  // pop until FALSE is returned
}


//------------------------------------------------------------------*
/**
 * push.
 *
 *  push data on stack.
 *  data can be retrieved using pop().
 *
 * @param           crtData: data to be pushed on the stack.
 * @return          -
 * @exception       -
 * @see             pop()
*/
template <class T> 
void CSBLStack<T>::push(const T& crtData)
{
    CSBLStackNode* pNewNode;                 // new node

    pNewNode = new CSBLStackNode;            // create new node
    pNewNode->m_tData = crtData;             // store new data
    
    pNewNode->m_pNext = m_pHead->m_pNext;    // insert new node directly after head
    m_pHead->m_pNext = pNewNode;
}

//------------------------------------------------------------------*
/**
 * pop.
 *
 *  get data from last push() call.
 *  returns FALSE if stack is empty().
 *
 * @param           rtData: returned data
 * @return          successful?
 * @exception       -
 * @see             push()
*/
template <class T> 
BOOL CSBLStack<T>::pop(T& rtData)
{
    // if empty return 0
    if(isEmpty())
    {
        return FALSE;                       // not OK
    }

    CSBLStackNode*  pNode;                   // helper variable for node

    pNode = m_pHead->m_pNext;               // point to first node
    rtData = pNode->m_tData;                // get data from first node
    m_pHead->m_pNext = pNode->m_pNext;      // remove link
    
    delete pNode; pNode = 0;                // delete node

    return TRUE;                            // OK
}

#endif // __SBLSTACK_H_


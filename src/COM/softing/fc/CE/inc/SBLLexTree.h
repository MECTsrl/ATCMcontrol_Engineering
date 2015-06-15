#ifndef __SBLLEXTREE_H_
#define __SBLLEXTREE_H_


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "stdlib.h"
#include "string.h"
#include "SBLStack.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//#define MAX_LEX_TREE_STRLEN 256

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

 

//------------------------------------------------------------------*
/**
 *  class CSBLLexTree<T>
 *
 *   - class for fast access to data associated with a string (key)
 *     (T is the data object class)
 *   - stores data pointers associated with strings in a tree
 *     (data pointers are of type T*)
 *   - each node represents a character of a string
 *   - the string is the full path to the node containing the data
 *   - all strings must be unique
 *   - by default the association is case sensitive. The original string
 *     can be retrieved using getOriginalKey(). Case sensitivity can be
 *     enabled using setModusCaseSensitive()
 *   - by default CSBLLexicalTree is not owner of the data, i. e.
 *     data pointers are not deleted on deletion of the tree node 
 *     containing the data. This behaviour can be changed using 
 *     setModusOwner()
 *
 *  remarks:
 *   - if size of class T is only 4 bytes and owner modus is active.
 *     it is a good idea to use the data pointer directly. This saves
 *     memory and is faster. A more detailled description is given in the
 *     methods insertData() and getData().
 *   - to get all stored data first get all strings using getAllStringsFromSubstring()
 *     with empty string as parameter and then run over all strings of the
 *     returned string array and get the data using getData(). (If required,
 *     an iterator may be implemented easily)
 *
 */

template <class T>
class CSBLLexTree
{
protected:
    class CSBLLexTreeNode;

// public methods
public:
    // constructor
    CSBLLexTree();
    // destructor
    ~CSBLLexTree();

    // get maximum string length
    int getMaximumStringLength();

    // get number of strings stored in tree
    long getNumStrings();

    // insert data
    bool insertData(const CString& strKey, T* pData, bool b_overwrite = false);

    // delete data
    bool deleteData(const CString& strKey);

    // check if substring is present (if so node is marked)
    bool isSubStringPresent(const CString& strKey);

    // retrieve data for string (or marked node) (0 if not found)
    T* getData(const CString& strKey = "");

    // get original key string
    CString getOriginalKey(const CString& crstrKey);

    // retrieve data for next found string and corresponding string (0 if no one found)
    T* getNextData(CString& strKey);

    // complete string as far as unambigous
    void completeStringUnambigous(CString& strKey);

    // get all strings up to l_max_strings starting with given substring (returns number of strings)
    //long getAllStringsFromSubstring(CString& strSubstring, CString&* caa_strings, long l_max_strings);

    // same as above but with CString
    long getAllStringsFromSubstring(const CString& rcs_substring, CStringArray& rcsa_strings, long lMaxCount = LONG_MAX);

    // reset tree (deletes all strings)
    void reset();

    // set modus owner
    void setModusOwner(bool bOwner = true);

    // set modus case sensitive
    void setModusCaseSensitive(bool bCaseSensitive = false);

// protected methods
protected:
    // deletes all children recursively
    void deleteAllChildren(CSBLLexTreeNode* pNode);

    // delete node if no children and no siblings exist and node not root node
    bool deleteNode(CSBLLexTreeNode* pNode);

    // find child node
    CSBLLexTreeNode* findChild(CSBLLexTreeNode* pFatherNode, char cToFind, bool b_exact = true);

    // find node
    CSBLLexTreeNode* findNode(const CString& strKey);

    // insert child node for character cToInsert if not yet present and return this child node
    CSBLLexTreeNode* insertChild(CSBLLexTreeNode* pFatherNode, char cToInsert);

    // returns first node with data (0 if none)
    CSBLLexTreeNode* getFirstString(CSBLLexTreeNode* pNodeStart, CString& strKey);

    // find next node containing data starting from node (does not find starting node)
    //CSBLLexTreeNode* findData(CSBLLexTreeNode* pNodeStart);

// protected members
protected:
    // internal class for nodes
    class CSBLLexTreeNode
    {
    // public methods
    public:
        /////////////////////////////// LEX_TREE_NODE ///////////////////////////////////////////////
        // constructor with default arguments
        CSBLLexTreeNode
        (
            char cChar = 0, 
            CSBLLexTreeNode* pNextSibling = 0,
            CSBLLexTreeNode* pFirstChild = 0
        )
        {
            m_cChar = cChar;
            m_pNextSibling = pNextSibling;
            m_pFirstChild = pFirstChild;
            m_pData = 0;
        };
        // destructor
        ~CSBLLexTreeNode() {};
        /////////////////////////////////////////////////////////////////////////////////////////////

    // public members
    public:
        char                m_cChar;        // char represented by node
        T*                  m_pData;        // data for node

        CSBLLexTreeNode*    m_pNextSibling; // pointer to next sibling node
        CSBLLexTreeNode*    m_pFirstChild;  // pointer to first child node
        CString             m_strString;    // original string 
    };

    // internal class for tree string (used for stack)
    class CSBLLexTreeString
    {
    // public methods
    public:
        // constructor
        CSBLLexTreeString(CSBLLexTreeNode* pNode = 0, const CString& strKey = "") {m_pNode = pNode; m_strKey = strKey; };
        // destructor
        ~CSBLLexTreeString() {};

    // public members
    public:
        CSBLLexTreeNode*    m_pNode;    // tree node
        CString             m_strKey;   // key string
    };


    // maximum string length (is not updated on deletion)
    int                     m_i_max_strlen;

    // number of strings stored in tree
    long                    m_l_num_strings;
    
    // link to first node
    CSBLLexTreeNode*        m_pRoot;

    // link to marked node
    CSBLLexTreeNode*        m_pMarked;

    // owner of data?
    bool                    m_bOwner;

    // case sensitive?
    bool                    m_bCaseSensitive;

    // stack of tree nodes to marked node
    //CSBStack<CSBLLexTreeNode>   m_tMarkedStack;

};


/////////////////////////////// LEX_TREE ////////////////////////////////////////////////////
// 
//------------------------------------------------------------------*
/**
 * constructor.
 *
 *  objects of class T are managed and retrieved using pointers (T*)
 *
 *  e. g. given an array of CTmp objects you use CSBLLexTree<CTmp>.
 *  You add and retrieve objects by pointers (CTmp*).
 *
*/
template <class T>
CSBLLexTree<T>::CSBLLexTree()
{
    m_i_max_strlen      = 0;
    m_l_num_strings     = 0;
    m_pRoot             = new CSBLLexTreeNode;
    m_pMarked           = 0;
    m_bOwner            = false;
    m_bCaseSensitive    = false;
}

//------------------------------------------------------------------*
/**
 * destructor.
 *
 * @see             reset()
*/
template <class T>
CSBLLexTree<T>::~CSBLLexTree()
{
    reset();
    delete m_pRoot; m_pRoot = 0;
}


//------------------------------------------------------------------*
/**
 * set owner modus.
 *
 *  On default CSBLLexTree is not owner of data but only references
 *  to data belonging to an other class.
 *  If the lexical tree should be owner, use this function with bOwner = true.
 *  On deletion of a node the corresponding data is deleted also.
 *
 * @param           [in] bOwner: lexical tree is owner? default is true
 * @return          -
 * @exception       -
 * @see             -
*/
template <class T>
void CSBLLexTree<T>::setModusOwner(bool bOwner /*=true*/)
{
    m_bOwner = bOwner;
}


//------------------------------------------------------------------*
/**
 * set modus case sensitive.
 *
 *  Normally the association of string to data is not case sensitive.
 *  In this case the associated key is made lower case and the string
 *  member of the node is used to store the original string (key).
 *  To get the original key use getOriginalKey().
 *
 * @param           [in] bCaseSensitive: case sensitivity enabled? default is false
 * @return          -
 * @exception       -
 * @see             getOriginalKey()
*/
template <class T>
void CSBLLexTree<T>::setModusCaseSensitive(bool bCaseSensitive /*=false*/)
{
    m_bCaseSensitive = bCaseSensitive;
}

//------------------------------------------------------------------*
/**
 * reset tree.
 *
 *  Delete all strings.
 *  If owner all stored data objects are deleted also.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
template <class T>
void CSBLLexTree<T>::reset()
{
    deleteAllChildren(m_pRoot);
    m_pRoot->m_pFirstChild = 0;
    m_pMarked = 0;
    m_i_max_strlen  = 0;
    m_l_num_strings = 0;
}

//------------------------------------------------------------------*
/**
 * delete all children.
 *
 *   Delete all children nodes recursively.
 *
 * @param           [in] pNode: pointer to parent node
 * @return          -
 * @exception       -
 * @see             -
*/
template <class T>
void CSBLLexTree<T>::deleteAllChildren(CSBLLexTreeNode* pNode)
{
    // run over all subnodes and collect them in a stack
    CSBLStack<CSBLLexTreeNode*>  tStackCollect;  // for collecting nodes
    CSBLStack<CSBLLexTreeNode*>  tStack;         // for traversing

    CSBLLexTreeNode*    pNodeTmp;
    CSBLLexTreeNode*    pNodeNext;

    if(!pNode->m_pFirstChild)
        return;

    pNodeNext = pNode->m_pFirstChild;
    tStack.push(pNodeNext);
    do                                          // run over all other children
    {
        pNodeNext = pNodeNext->m_pNextSibling;
        if(pNodeNext)
        {
            tStack.push(pNodeNext);
        }
    } while(pNodeNext);

    // run over all children
    while(!tStack.isEmpty())  
    {
        tStack.pop(pNodeTmp);
        tStackCollect.push(pNodeTmp);

        pNodeNext = pNodeTmp->m_pFirstChild;
        if(pNodeNext)
        {
            tStack.push(pNodeNext);
            do                                          // run over all other children
            {
                pNodeNext = pNodeNext->m_pNextSibling;
                if(pNodeNext)
                    tStack.push(pNodeNext);
            } while(pNodeNext);
        }
    }                     // until stack is empty
    // delete all subnodes from the collect stack
    while(tStackCollect.pop(pNodeTmp))
    {
        if(pNodeTmp)
        {
            if(m_bOwner && pNodeTmp->m_pData)
            {
                delete (pNodeTmp->m_pData);
            }
            delete pNodeTmp;
        }
    }
}


//------------------------------------------------------------------*
/**
 * delete node.
 *
 *  Node is only deleted if it has no child and no sibling and it is not
 *  the root node.
 *  If owner modus is active, associated data is also deleted.
 *
 * @param           [in] pNode: node to be deleted.
 * @return          -
 * @exception       -
 * @see             -
*/
template <class T>
bool CSBLLexTree<T>::deleteNode(CSBLLexTreeNode* pNode)
{
    // do not delete node if it has children or siblings or is the root node
    if(pNode->m_pFirstChild || pNode->m_pNextSibling || pNode == m_pRoot)
    {
        return false;
    }

    if(pNode->m_pData)
    {
        --m_l_num_strings;
        if(m_bOwner)
        {
            delete pNode->m_pData;
        }
    }

    delete pNode;

    return true;
}


//------------------------------------------------------------------*
/**
 * get maximum string length.
 *
 *  get the number of characters of the longest stored string
 *  caution: this is not updated on deletion of single strings but 
 *  only on reset.
 *
 * @param           -
 * @return          maximum string length
 * @exception       -
 * @see             -
*/
template <class T>
int CSBLLexTree<T>::getMaximumStringLength()
{
    return m_i_max_strlen;
}

//------------------------------------------------------------------*
/**
 * get number of strings.
 *
 *  Get number of strings associated with data items.
 *
 * @param           -
 * @return          number of stored data items
 * @exception       -
 * @see             -
*/
template <class T>
long CSBLLexTree<T>::getNumStrings()
{
    return m_l_num_strings;
}

//------------------------------------------------------------------*
/**
 * insert data.
 *
 *  Insert new data into the tree.
 *  New nodes are created, if necessary and the data element is stored.
 *
 *  Here a little trick:
 *  If sizeof(T) == 4 and owner modus is active, T* can be used instead of T.
 *  E. g. You want to associate strings to long integers you do not need to
 *  create the data but use type casts like in the following example:
 *
 *  CSBLLexTree<long>   tTree;
 *
 *  instead of:
 *  tTree.setModusOwner();
 *  long*   pLong = new long;
 *  *pLong = 5;
 *  tTree.insert("fuenf", pLong);
 *  ...
 *  pLong = tTree.getData("fuenf");
 *  long    lLong = *pLong;
 *
 *  use:
 *  pLong = (long*) 5;
 *  tTree.insert("fuenf", pLong);
 *  ...
 *  pLong = tTree.getData("fuenf");
 *  long    lLong = (long) pLong;
 *  
 *  Do not call setModusOwner() in the latter case.
 *
 * @param           [in] strKey: key string
 * @param           [in] pData: data associated with the key
 * @param           [in] b_overwrite: overwrite existing data? default is false
 * @return          successful?
 * @exception       -
 * @see             -
*/
template <class T>
bool CSBLLexTree<T>::insertData(const CString& strKey, T* pData, bool b_overwrite /*=false*/)
{
    CSBLLexTreeNode* pNodeNew;
    CSBLLexTreeNode* pNodeTmp = m_pRoot;
    CString strKeyTmp = strKey;
    if(!m_bCaseSensitive)
    {
        strKeyTmp.MakeLower();
    }

    int i_char_count = 0;
    int i_strlen = strKeyTmp.GetLength();

    // store max string length
    if(i_strlen > m_i_max_strlen)
        m_i_max_strlen = i_strlen;

    // run over all characters
    for(int i=0; i<i_strlen; ++i)
    {
        pNodeNew = insertChild(pNodeTmp, strKeyTmp[i]);
        pNodeTmp = pNodeNew;
    }

    // if data is already present and should not be overwritten -> return false
    if(pNodeNew->m_pData)
    {
        if(b_overwrite)
        {
            if(m_bOwner)
            {
                delete (pNodeNew->m_pData);
            }
            --m_l_num_strings;
        }
        else
            return false;
    }

    pNodeNew->m_pData = pData;
    pNodeNew->m_strString = strKey;
    ++m_l_num_strings;
    m_pMarked = pNodeNew;

    return true;
}

//------------------------------------------------------------------*
/**
 * find child node.
 *
 *  used to find an existing child node or to find
 *  the insert location for a new one.
 *
 * @param           [in] pFatherNode: start node to search
 * @param           [in] cToFind: character to find
 * @param           [in] b_exact: find exact? false means find insert location.
 * @return          pointer to found tree node or NULL if not found
 * @exception       -
 * @see             -
*/
template <class T>
CSBLLexTree<T>::CSBLLexTreeNode* CSBLLexTree<T>::findChild(CSBLLexTreeNode* pFatherNode, char cToFind, bool b_exact /*=true*/)
{
    CSBLLexTreeNode* pNodeFirst = pFatherNode->m_pFirstChild;
    CSBLLexTreeNode* pNodeSecond;

    // if father node has no children -> return 0
    if(!pNodeFirst)
    {
        return(0);
    }

    // if cToFind < first char -> not found
    if(cToFind > pNodeFirst->m_cChar)
    {
        if(b_exact)             // if exact find required return 0
            return(0);  
        return(pFatherNode);    // else return father node
    }

    do
    {
        // get next sibling
        pNodeSecond = pNodeFirst;
        pNodeFirst = pNodeSecond->m_pNextSibling;

        if(cToFind == pNodeSecond->m_cChar)
            return(pNodeSecond);             // return node

        if(pNodeFirst)
        {
            if(cToFind > pNodeFirst->m_cChar)
            {
                break;
            }
        }
    } while(pNodeFirst);

    if(!b_exact)                    // if not exact find required
    {
        return(pNodeSecond);        // return node
    }
    return(0);                      // else return 0
}


//------------------------------------------------------------------*
/**
 * insert child node.
 *
 *  insert child node with character cToInsert, if not yet present
 *
 * @param           [in] pFatherNode: father node where new child is to be inserted
 * @return          new tree node.
 * @exception       -
 * @see             -
*/
template <class T>
CSBLLexTree<T>::CSBLLexTreeNode* CSBLLexTree<T>::insertChild(CSBLLexTreeNode* pFatherNode, char cToInsert)
{
    CSBLLexTreeNode* pNodeNew = 0;

    // if no children -> insert as first child
    if(!pFatherNode->m_pFirstChild)
    {
        pNodeNew = new CSBLLexTreeNode(cToInsert);
        pFatherNode->m_pFirstChild = pNodeNew;
        return(pNodeNew);
    }

    // Search for child with character cToInsert in insert mode
    CSBLLexTreeNode* pFindChild = findChild(pFatherNode, cToInsert, false);

    // insert before first child
    if(pFindChild == pFatherNode)
    {
        pNodeNew = new CSBLLexTreeNode(cToInsert, pFatherNode->m_pFirstChild);
        pFatherNode->m_pFirstChild = pNodeNew;
        return(pNodeNew);
    }

    // character is already present
    if(pFindChild->m_cChar == cToInsert)
        return(pFindChild);

    // insert after pFindChild
    pNodeNew = new CSBLLexTreeNode(cToInsert, pFindChild->m_pNextSibling);
    pFindChild->m_pNextSibling = pNodeNew;

    return(pNodeNew);
}


//------------------------------------------------------------------*
/**
 * delete data.
 *
 *  Deletes a key string.
 *  If owner mode, the associated data is deleted also.
 *  Nodes that are not used any more are deleted.
 *
 * @param           [in] strKey: key string to be deleted.
 * @return          successful?
 * @exception       -
 * @see             -
*/
template <class T>
bool CSBLLexTree<T>::deleteData(const CString& strKey)
{
    CSBLLexTreeNode* pNodeTmp1 = m_pRoot;
    CSBLLexTreeNode* pNodeTmp2;
    CSBLStack<CSBLLexTreeNode*>  tStack;
    bool bFound;
    char c_toFind;
    CString strKeyTmp = strKey;
    if(!m_bCaseSensitive)
    {
        strKeyTmp.MakeLower();
    }

    int i_char_count = 0;
    int i_strlen = strKeyTmp.GetLength();
    
    if(i_strlen == 0)
        return false;

    // run over all characters
    for(int i=0; i<i_strlen; ++i)
    {
        c_toFind = strKeyTmp[i];
        if(!pNodeTmp1->m_pFirstChild)
            return false;

        // run over children and find correct character
        bFound = false;
        pNodeTmp2 = pNodeTmp1->m_pFirstChild;
        do
        {
            pNodeTmp1 = pNodeTmp2;
            tStack.push(pNodeTmp2);
            if(pNodeTmp2->m_cChar == c_toFind)
            {
                bFound = true;
                break;
            }
            pNodeTmp2 = pNodeTmp1->m_pNextSibling;
        } while(pNodeTmp2);

        if(!bFound)
            return false;
    }

    if(pNodeTmp1->m_pData)
    {
        if(m_bOwner)
            delete pNodeTmp1->m_pData;
        pNodeTmp1->m_pData = 0;
        --m_l_num_strings;
        if(m_pMarked == pNodeTmp1)
            m_pMarked = 0;
    }
    else
    {
        bFound = false;
    }

    // do some cleaning work
    CSBLLexTreeNode* pNodeData = 0;

    tStack.pop(pNodeTmp2);  // at least one stack entry

    while(tStack.pop(pNodeTmp1))
    {
        // if no links and no data exist -> remove node
        if(!pNodeTmp2->m_pFirstChild && !pNodeTmp2->m_pData)
        {
            // delete link in previous node
            // is previous node father or last sibling?
            if(pNodeTmp1->m_pNextSibling == pNodeTmp2)
            {
                pNodeTmp1->m_pNextSibling = pNodeTmp2->m_pNextSibling;
            }
            else
            {
                pNodeTmp1->m_pFirstChild = pNodeTmp2->m_pNextSibling;
            }
            delete pNodeTmp2;
        }
        else
        {
            break;
        }
        pNodeTmp2 = pNodeTmp1;
    }

    return bFound;
}

//------------------------------------------------------------------*
/**
 * check if substring is present.
 *
 *  Search node which node path is identical with substring.
 *  Uses findNode()
 *
 * @param           [in] strKey: substring to be searched
 * @return          substring is present?
 * @exception       -
 * @see             findNode()
*/
template <class T>
bool CSBLLexTree<T>::isSubStringPresent(const CString& strKey)
{
    CSBLLexTreeNode* pNodeNew;
    CString strKeyTmp = strKey;
    if(!m_bCaseSensitive)
    {
        strKeyTmp.MakeLower();
    }

    pNodeNew = findNode(strKeyTmp);

    if(pNodeNew)
    {
        m_pMarked = pNodeNew;
        return true;
    }
    return false;
}


//------------------------------------------------------------------*
/**
 * find node.
 *
 *  search for node representing a certain given key.
 *
 * @param           [in] strKey: key string to find
 * @return          found node pointer or NULL, if not found.
 * @exception       -
 * @see             -
*/
template <class T>
CSBLLexTree<T>::CSBLLexTreeNode* CSBLLexTree<T>::findNode(const CString& strKey)
{
    CSBLLexTreeNode* pNodeTmp = m_pRoot;
    CSBLLexTreeNode* pNodeNew = pNodeTmp;

    int i_char_count = 0;
    int i_strlen = strKey.GetLength();

    // never seen such a large string
    if(i_strlen > m_i_max_strlen)
        return false;

    // run over all characters
    for(int i=0; i<i_strlen; ++i)
    {
        pNodeNew = findChild(pNodeTmp, strKey[i]);
        if(!pNodeNew)
            return(0);
        pNodeTmp = pNodeNew;
    }

    return(pNodeNew);
}

//------------------------------------------------------------------*
/**
 * get data.
 *
 *   retrieve associated data for string or marked node.
 *
 * @param           [in] strKey: key string. default is empty
 * @return          pointer to associated data or NULL if not found.
 * @exception       -
 * @see             -
*/
template <class T>
T* CSBLLexTree<T>::getData(const CString& strKey /*= ""*/)
{
    if(strKey == "")
    {
        if(m_pMarked)
            return(m_pMarked->m_pData);
        return 0;
    }

    CString strKeyTmp = strKey;
    if(!m_bCaseSensitive)
    {
        strKeyTmp.MakeLower();
    }

    CSBLLexTreeNode* pNodeFind = findNode(strKeyTmp);
    if(pNodeFind && pNodeFind->m_pData)
        return(pNodeFind->m_pData);
    return 0;
}

//------------------------------------------------------------------*
/**
 * get original key.
 *
 *  get original key string for key
 *  this function is only meaningful in "no case sensitivity modus"
 *
 * @param           [in] crstrKey: key string
 * @return          original key string
 * @exception       -
 * @see             
*/
template <class T>
CString CSBLLexTree<T>::getOriginalKey(const CString& crstrKey)
{
    CString strKeyTmp = crstrKey;
    CString str;

    if(!m_bCaseSensitive)
    {
        strKeyTmp.MakeLower();
    }

    CSBLLexTreeNode* pNodeFind = findNode(strKeyTmp);
    if(pNodeFind)
    {
        str = pNodeFind->m_strString;
    }
    return str;
}

//------------------------------------------------------------------*
/**
 * get next data.
 *
 *  Get data of next node associated with data and corresponding key string.
 *  Uses getFirstString().
 *
 * @param           [in/out] strKey: key string to start with.
 *                                   if successful, key string to found data
 * @return          pointer to data.
 * @exception       -
 * @see             getFirstString()
*/
template <class T>
T* CSBLLexTree<T>::getNextData(CString& strKey)
{
    T* pData = 0;

    CString strKeyTmp = strKey;
    if(!m_bCaseSensitive)
    {
        strKeyTmp.MakeLower();
    }

    CSBLLexTreeNode* pNodeTmp = findNode(strKeyTmp);
    if(pNodeTmp)
    {
        pNodeTmp = getFirstString(pNodeTmp, strKeyTmp);
        if(pNodeTmp)
        {
            pData = pNodeTmp->m_pData;
        }
    }
    return(pData);
}


//------------------------------------------------------------------*
/**
 * get first string.
 *
 *   Get first node with data starting from pStartNode.
 *
 * @param           [in] pNodeStart: start node.
 * @return          [in/out] rstrKey: key to start node.
 *                                    if successful, key to found node
 * @exception       -
 * @see             getNextData()
*/
template <class T>
CSBLLexTree<T>::CSBLLexTreeNode* CSBLLexTree<T>::getFirstString(CSBLLexTreeNode* pNodeStart, CString& rstrKey)
{
    CSBLStack<CSBLLexTreeString*>  tStack;
    CSBLLexTreeNode*    pNodeNext;
    CSBLLexTreeNode*    pNodeFound = 0;
    CSBLLexTreeString*  pStringTmp;
    CSBLLexTreeString*  pStringTmp2;
    if(!m_bCaseSensitive)
    {
        rstrKey.MakeLower();
    }

    int i_new_char = rstrKey.GetLength();  // points to trailing \0 of rstrKey

    pStringTmp = new CSBLLexTreeString(pNodeStart, rstrKey);
    tStack.push(pStringTmp);

    // run over all children in turn
    while(!tStack.isEmpty())
    {
        tStack.pop(pStringTmp);
        rstrKey = pStringTmp->m_strKey;
        // if node has data -> return node
        if(pStringTmp->m_pNode->m_pData)
        {
            pNodeFound = pStringTmp->m_pNode;
            delete pStringTmp;
            return(pNodeFound);
        }
        pNodeNext = pStringTmp->m_pNode->m_pFirstChild;
        if(pNodeNext)                  // if child exists create tree string and push
        {
            rstrKey += pNodeNext->m_cChar;
            if(pNodeNext->m_pData)
            {
                delete pStringTmp;
                pNodeFound = pNodeNext;
                break;
            }
            pStringTmp2 = new CSBLLexTreeString(pNodeNext, rstrKey);
            tStack.push(pStringTmp2);
        }
        i_new_char = rstrKey.GetLength();
        while(pNodeNext)                                     // run over all other children
        {
            pNodeNext = pNodeNext->m_pNextSibling;
            if(pNodeNext)
            {
                rstrKey.SetAt(i_new_char-1, pNodeNext->m_cChar);
                pStringTmp2 = new CSBLLexTreeString(pNodeNext, rstrKey);
                tStack.push(pStringTmp2);
            }
        }
        // delete tree string
        delete pStringTmp;
    }

    // delete remaining TreeStrings
    while(tStack.pop(pStringTmp))
    {
        delete pStringTmp;
    }

    // if pNodeFound exists search for last sibling with data
    if(pNodeFound)
    {
        pNodeNext = pNodeFound->m_pNextSibling;
        while(pNodeNext)
        {
            if(pNodeNext->m_pData)
                pNodeFound = pNodeNext;
            pNodeNext = pNodeNext->m_pNextSibling;
        }
        rstrKey.SetAt(i_new_char, pNodeFound->m_cChar);
    }

    return(pNodeFound);
}


//------------------------------------------------------------------*
/**
 * complete string unambiguous.
 *
 *  Complete string as far as unambigous.
 *  e. g.: the tree contains: "hallo" and "halleluja"
 *          completeStringUnambigous("h") will yield "hall"
 *
 * @param           [in/out] rstrKey: start key / found key
 * @return          -
 * @exception       -
 * @see             -
*/
template <class T>
void CSBLLexTree<T>::completeStringUnambigous(CString& rstrKey)
{
    CSBLLexTreeNode* pNodeTmp;
    if(!m_bCaseSensitive)
    {
        rstrKey.MakeLower();
    }

    pNodeTmp = findNode(rstrKey);

    if(!pNodeTmp)
        return;

    pNodeTmp = pNodeTmp->m_pFirstChild;

    // collect characters for all nodes not having next sibling
    while(pNodeTmp && !pNodeTmp->m_pNextSibling)
    {
        // append character of child
        rstrKey += pNodeTmp->m_cChar;
        pNodeTmp = pNodeTmp->m_pFirstChild;
    }
}

//------------------------------------------------------------------*
/**
 * get all strings from substring.
 *
 * @param           [in] rcs_substring: substring that is included in all strings
 *                                      to get. To get all strings pass empty string.
 * @param           [out] rcsa_strings: array of found strings
 * @param           [in] lMaxCount: maximum number of strings to find. default is LONG_MAX
 * @return          number of returned strings.
 * @exception       -
 * @see             -
*/
template <class T>
long CSBLLexTree<T>::getAllStringsFromSubstring(const CString& rcs_substring, CStringArray& rcsa_strings, long lMaxCount /*=LONG_MAX*/)
{
    CSBLStack<CSBLLexTreeString*>  tStack;
    CSBLLexTreeNode*    pNodeNext;
    CSBLLexTreeString*  pStringTmp;
    CSBLLexTreeString*  pStringTmp2;
    CString strSubstring = rcs_substring;
    if(!m_bCaseSensitive)
    {
        strSubstring.MakeLower();
    }

    int i_new_char = strSubstring.GetLength();  // points to trailing \0 of strSubstring
    long l_count = 0;

    pNodeNext = findNode(strSubstring);
    if(!pNodeNext || m_l_num_strings == 0)
        return(0);

    pStringTmp = new CSBLLexTreeString(pNodeNext, strSubstring);
    tStack.push(pStringTmp);

    // run over all children in turn
    while(!tStack.isEmpty())
    {
        tStack.pop(pStringTmp);
        strSubstring = pStringTmp->m_strKey;
        // if node has data -> return node
        if(pStringTmp->m_pNode->m_pData)
        {
            // add string
            rcsa_strings.Add(pStringTmp->m_pNode->m_strString);
            ++l_count;
            if(l_count >= lMaxCount)
            {
                delete pStringTmp;
                break;
            }
        }
        pNodeNext = pStringTmp->m_pNode->m_pFirstChild;
        if(pNodeNext)                  // if child exists create tree string and push
        {
            strSubstring += pNodeNext->m_cChar;
            pStringTmp2 = new CSBLLexTreeString(pNodeNext, strSubstring);
            tStack.push(pStringTmp2);
        }
        i_new_char = strSubstring.GetLength();
        while(pNodeNext)                                       // run over all other children
        {
            pNodeNext = pNodeNext->m_pNextSibling;
            if(pNodeNext)
            {
                strSubstring.SetAt(i_new_char-1, pNodeNext->m_cChar);
                pStringTmp2 = new CSBLLexTreeString(pNodeNext, strSubstring);
                tStack.push(pStringTmp2);
            }
        }
        // delete tree string
        delete pStringTmp;
    }


    // delete remaining TreeStrings
    while(tStack.pop(pStringTmp))
    {
        delete pStringTmp;
    }
    
    return(l_count);
}


/*
void CSBLLexTree<T>::traverseInOrder(CSBLLexTreeNode* pNode)
{
    CSBLStack<CSBLLexTreeNode*>  tStack;         // for traversing

    CSBLLexTreeNode*    pNodeTmp;
    CSBLLexTreeNode*    pNodeNext;

    tStack.push(pNode);

    // run over all children
    while(tStack.pop(pNodeTmp))  
    {
        // visit(pNodeTmp);

        pNodeNext = pNodeTmp->m_pFirstChild;
        if(pNodeNext)
        {
            tStack.push(pNodeNext);
            do                                          // run over all other children
            {
                pNodeNext = pNodeNext->m_pNextSibling;
                if(pNodeNext)
                    tStack.push(pNodeNext);
            } while(pNodeNext);
        }
    }
 
}*/


#endif // __SBLLEXTREE_H_


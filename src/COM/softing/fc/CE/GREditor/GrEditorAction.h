#if !defined(AFX_ACTION_H__9BCBA58F_01C7_11D2_8E7B_444553540000__INCLUDED_)
#define AFX_ACTION_H__9BCBA58F_01C7_11D2_8E7B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CAction : public CObject  
{
public:
          	CAction();
	virtual ~CAction();
// void test(char *s);

   void DeleteAll();

	void StoreState     (CElemList *pList);     //  save after edit actions, increment stack
	void LoadState      (CElemList *pList);     // undo                       
	void ReloadState    (CElemList *pList);     // redo
	void ReloadLastState(CElemList *pList);     // abort
	void RemoveLastState();                     //  for temporary saved state, decrement stack

   bool StateChangedQ(bool bReset=false);      // false if the load-state is displayed
   BOOL IsLoadAvail();
	BOOL IsReloadAvail();

private:
	int       m_iIdx;
	CPtrArray m_Hist;
   int m_iChangeLog;
};

#endif // !defined(AFX_ACTION_H__9BCBA58F_01C7_11D2_8E7B_444553540000__INCLUDED_)

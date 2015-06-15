// SECEditHint.h
#ifndef __SECEDITHINT_H_
#define __SECEDITHINT_H_

// OE Extension DLL
// Initialize declaration context
#ifdef _OEDLL
	#undef AFX_DATA
	#define AFX_DATA OE_DATAEXT
#endif // _OEDLL

class CGlobalOleInit
{
public:
	HRESULT m_hr;
	CGlobalOleInit();
	~CGlobalOleInit();
};

// class SECEditHint
// used to pass a LINEDESC to OnUpdate()
//@doc SECEditView
//@class SECEditView derives from CView
class SECEditHint : public CObject
{
public:

protected:
	DECLARE_DYNAMIC( SECEditHint )

// Attributes
protected:

public:
		//@cmember Internal line hint.
	PLINEDESC m_pLine;

// Operations
public:

};  // class SECEditHint

// OE Extension DLL
// Reset declaration context
#undef AFX_DATA
#define AFX_DATA


#endif // __SECEDITHINT_H_
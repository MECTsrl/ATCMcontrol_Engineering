
#if !defined(_SFCELEM_H)
#define _SFCELEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSFCElem  
{
public:
	CSFCElem();
	CSFCElem(const CString& strName, unsigned long lNumber, unsigned long lX, unsigned long lY)
		:Name(strName),Number(lNumber),X(lX),Y(lY)
	{
	};
	virtual ~CSFCElem();

public:
	CString Name;
	unsigned long Number;
	unsigned long X;
	unsigned long Y;
};

#endif // !defined(_SFCELEM_H)

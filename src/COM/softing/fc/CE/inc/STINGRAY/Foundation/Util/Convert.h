#ifndef __CONVERT_H__
#define __CONVERT_H__

namespace stingray {
namespace foundation {

class Convert
{
public:
	static BOOL LPtoLP(HDC hDstDC, const int nSrcMM, SIZE& size);
	static BSTR Variant2BSTR(VARIANT* pVar);
};

};	// namespace stingray::foundation
};	// namespace stingray

#endif  // __CONVERT_H__

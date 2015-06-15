#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <limits.h>
#include "fc_todebug\fc_mem.h"
#include "fc_tools\fc_defines.h"
#include "baselib.h"


static int NtotalErrors;



static void DumpTestResult(bool bOk, int nTest)
{
    if(bOk)
        _tprintf(_T("test %d: OK\n"),nTest);
    else
    {
        _tprintf(_T("test %d: FAILED\n"),nTest);
        NtotalErrors++;
    }
}
static bool CheckTimeVariant(const VARIANT* pVt, __int64 i64MStot)
{
    if(pVt->vt!=VT_DECIMAL || pVt->decVal.scale!=0 || pVt->decVal.Hi32!=0)
        return false;

    if(i64MStot<0)
        return pVt->decVal.sign==DECIMAL_NEG && pVt->decVal.Lo64==(unsigned __int64) (-i64MStot);
    else
        return pVt->decVal.sign==0 && pVt->decVal.Lo64==(unsigned __int64) i64MStot;
}


int main()
{
    VARIANT out;
    HRESULT hr;
    bool    bOk;
    int     nTest=0;
    TCHAR   buffer[256];

    NtotalErrors=0;





    //###############################################################
    //test string boolean to variant:
    hr = BL_StringToVariant(_T(" 0 "), BL_BOOL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.boolVal==VARIANT_FALSE;
    DumpTestResult(bOk, ++nTest);
    
    hr = BL_StringToVariant(_T("\tbOOl#0\r"), BL_BOOL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.boolVal==VARIANT_FALSE;
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T("FaLse\n"), BL_BOOL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.boolVal==VARIANT_FALSE;
    DumpTestResult(bOk, ++nTest);
    
    hr = BL_StringToVariant(_T("   \r\n\tBool#FaLse   \r\n\t"), BL_BOOL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.boolVal==VARIANT_FALSE;
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T("1"), BL_BOOL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.boolVal==VARIANT_TRUE;
    DumpTestResult(bOk, ++nTest);
    
    hr = BL_StringToVariant(_T("bOOl#1"), BL_BOOL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.boolVal==VARIANT_TRUE;
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T("TrUe                        \t\t\t\n\r"), BL_BOOL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.boolVal==VARIANT_TRUE;
    DumpTestResult(bOk, ++nTest);
    
    hr = BL_StringToVariant(_T("Bool#TrUE"), BL_BOOL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.boolVal==VARIANT_TRUE;
    DumpTestResult(bOk, ++nTest);
    //###############################################################






    //###############################################################
    //test string double to variant:
    hr = BL_StringToVariant(_T("13.45"), BL_LREAL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.dblVal==13.45;
    DumpTestResult(bOk, ++nTest);
    
    hr = BL_StringToVariant(_T("+13.45"), BL_LREAL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.dblVal==13.45;
    DumpTestResult(bOk, ++nTest);
    
    hr = BL_StringToVariant(_T("-13.45"), BL_LREAL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.dblVal==-13.45;
    DumpTestResult(bOk, ++nTest);
    
    hr = BL_StringToVariant(_T(" -13.45\t"), BL_LREAL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.dblVal==-13.45;
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T(" -13.45e0\t"), BL_LREAL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.dblVal==-13.45;
    DumpTestResult(bOk, ++nTest);
    
    hr = BL_StringToVariant(_T(" -13.45e+0\t"), BL_LREAL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.dblVal==-13.45;
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T(" -1_3.4_5e-0_0_0\t"), BL_LREAL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.dblVal==-13.45;
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T(" -1_3.4_5e-0_0_0\t"), BL_REAL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.dblVal==-13.45;
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T("Real#-1_3.4_5e-0_0_0\t"), BL_REAL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.dblVal==-13.45;
    DumpTestResult(bOk, ++nTest);

    hr = BL_VariantToString(&out, BL_REAL, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = SUCCEEDED(hr) && !lstrcmp(L"-13.45", buffer);
    DumpTestResult(bOk, ++nTest);



    //max / min values
    hr = BL_StringToVariant(_T("1.7976931348623158e+308"), BL_LREAL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.dblVal==1.7976931348623158e+308;
    DumpTestResult(bOk, ++nTest);
   
    hr = BL_StringToVariant(_T("-1.7976931348623158e+308"), BL_LREAL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.dblVal==-1.7976931348623158e+308;
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T("3.402823466e+38"), BL_REAL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.dblVal==3.402823466e+38;
    DumpTestResult(bOk, ++nTest);
    hr = BL_StringToVariant(_T("-3.402823466e+38"), BL_REAL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.dblVal==-3.40282346600000000e+38;
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T("1.175494351e-38"), BL_REAL, NULL, 0, &out);
    DumpTestResult(hr==S_OK && out.dblVal==+1.175494351e-38, ++nTest);
    hr = BL_StringToVariant(_T("-1.175494351e-38"), BL_REAL, NULL, 0, &out);
    DumpTestResult(hr==S_OK && out.dblVal==-1.175494351e-38, ++nTest);


    hr = BL_StringToVariant(_T("2.2250738585072014e-308"), BL_LREAL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.dblVal==2.2250738585072014e-308;
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T("-2.2250738585072014e-308"), BL_LREAL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.dblVal==-2.2250738585072014e-308;
    DumpTestResult(bOk, ++nTest);


    //max/min value +- eps 
    hr = BL_StringToVariant(_T("1.7976931348623159e+308"), BL_LREAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);
    hr = BL_StringToVariant(_T("-1.7976931348623159e+308"), BL_LREAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);

    hr = BL_StringToVariant(_T("3.402823467e+38"), BL_REAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);
    hr = BL_StringToVariant(_T("-3.402823467e+38"), BL_REAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);


    //underflow:
    hr = BL_StringToVariant(_T("2.2250738585072013e-508"), BL_LREAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_LOSS_OF_PRECIS, ++nTest);
    hr = BL_StringToVariant(_T("-2.2250738585072013e-508"), BL_LREAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_LOSS_OF_PRECIS, ++nTest);

    hr = BL_StringToVariant(_T("1.175494350e-38"), BL_REAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_LOSS_OF_PRECIS, ++nTest);
    hr = BL_StringToVariant(_T("-1.175494350e-38"), BL_REAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_LOSS_OF_PRECIS, ++nTest);

    

    
    //huge fraction
    hr = BL_StringToVariant(_T("0065859339586_1.797693134862376329653630953266702957690257629587_875897089586276295627596587282975493875285728957296274529752835787528352382357352857_285237238559e+1_2_3"), BL_LREAL, NULL, 0, &out);
    bOk = SUCCEEDED(hr) && out.dblVal==00658593395861.79769313486237632965363095326670295769025762958787589708958627629562759658728297549387528572e+123;
    DumpTestResult(bOk, ++nTest);

    //misplaces '_'
    hr = BL_StringToVariant(_T("_1.797"), BL_LREAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_SYNTAX, ++nTest);
    hr = BL_StringToVariant(_T("-_1.797"), BL_LREAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_SYNTAX, ++nTest);
    hr = BL_StringToVariant(_T("-1_.797"), BL_LREAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_SYNTAX, ++nTest);
    hr = BL_StringToVariant(_T("-1__0.797"), BL_LREAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_SYNTAX, ++nTest);
    hr = BL_StringToVariant(_T("-1_0._797"), BL_LREAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_SYNTAX, ++nTest);
    hr = BL_StringToVariant(_T("-1_0.7__97"), BL_LREAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_SYNTAX, ++nTest);
    hr = BL_StringToVariant(_T("-1_0.797_"), BL_LREAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_SYNTAX, ++nTest);
    hr = BL_StringToVariant(_T("-1_0.797_0e_9"), BL_LREAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_SYNTAX, ++nTest);
    hr = BL_StringToVariant(_T("-1_0.797_0e+_9"), BL_LREAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_SYNTAX, ++nTest);
    hr = BL_StringToVariant(_T("-1_0.797_0e+9_"), BL_LREAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_SYNTAX, ++nTest);
    
    //bad trailing / leading chars
    hr = BL_StringToVariant(_T("d-1_0.797"), BL_LREAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_SYNTAX, ++nTest);
    hr = BL_StringToVariant(_T("-1_0.797\t\n6"), BL_LREAL, NULL, 0, &out);
    DumpTestResult(hr==E_BL_ERR_SYNTAX, ++nTest);
    //###############################################################




    //###############################################################
    //test string time to variant:
    __int64 l64MStot, l64Tmp;
    const long lD = 24*3600*1000;
    const long lH = 3600*1000;
    const long lM = 60*1000;
    const long lS = 1000;

    //test: some simple syntax errors:
    DumpTestResult(E_BL_ERR_SYNTAX==BL_StringToVariant(_T(""), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out), ++nTest);
    DumpTestResult(E_BL_ERR_SYNTAX==BL_StringToVariant(_T("time#"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out), ++nTest);
    DumpTestResult(E_BL_ERR_SYNTAX==BL_StringToVariant(_T(" t# "), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out), ++nTest);
    DumpTestResult(E_BL_ERR_SYNTAX==BL_StringToVariant(_T("t#d4"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out), ++nTest);
    DumpTestResult(E_BL_ERR_SYNTAX==BL_StringToVariant(_T("t#4ms \n\t\007"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out), ++nTest);

    //test the most complex syntax:
    hr = BL_StringToVariant(_T("\t \r+t#-1d_2h_3m_4S_5mS\t\n\r\r  "), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    l64MStot = - (1*lD + 2*lH + 3*lM + 4*lS + 5);
    bOk = SUCCEEDED(hr) && CheckTimeVariant(&out, l64MStot);
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T("t#1d_2h_3m_4S_5mS"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    l64MStot = + (1*lD + 2*lH + 3*lM + 4*lS + 5);
    bOk = SUCCEEDED(hr) && CheckTimeVariant(&out, l64MStot);
    DumpTestResult(bOk, ++nTest);

    //very simple
    hr = BL_StringToVariant(_T("t#-4ms"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = SUCCEEDED(hr) && CheckTimeVariant(&out, -4);
    DumpTestResult(bOk, ++nTest);
    


    hr = BL_StringToVariant(_T(" t#1.45d "), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    l64MStot = 1*lD + (lD/10/10)*45;
    bOk = SUCCEEDED(hr) && CheckTimeVariant(&out, l64MStot);
    DumpTestResult(bOk, ++nTest);
    
    hr = BL_StringToVariant(_T("t#2345.450d"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    l64MStot = ((__int64)2345)*lD + (lD/10/10/10)*450;
    bOk = SUCCEEDED(hr) && CheckTimeVariant(&out, l64MStot);
    DumpTestResult(bOk, ++nTest);

    //max value
    hr = BL_StringToVariant(_T("t#9223372036854775807ms"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = SUCCEEDED(hr) && CheckTimeVariant(&out, _I64_MAX);
    DumpTestResult(bOk, ++nTest);

    //min value
    hr = BL_StringToVariant(_T("t#-9223372036854775808ms"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = SUCCEEDED(hr) && CheckTimeVariant(&out, _I64_MIN);
    DumpTestResult(bOk, ++nTest);

    //max value + 1
    hr = BL_StringToVariant(_T("t#9223372036854775808ms"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);

    //min value - 1
    hr = BL_StringToVariant(_T("\n \tt#-9223372036854775809ms  \t"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);

    //max value + 'some huge'
    hr = BL_StringToVariant(_T("t#9223372036854775808498674986965964768679867886586768647ms"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);

    //min value - 'some huge'
    hr = BL_StringToVariant(_T("t#-92233720368547758097947676060470946906986769457676967ms"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);

    //overflow using more units                  //9223372036854775808 <-max ms
    hr = BL_StringToVariant(_T("t#100d_111h_23m_4S_9223372036854770000mS"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);

    //underflow using more units                  //9223372036854775808 <-max ms
    hr = BL_StringToVariant(_T("t#-100d_111h_23m_4S_9223372036854770000mS"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);

    //underflow using day*huge                  
    hr = BL_StringToVariant(_T("t#-9223372036854d"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);

    //overflow using day*huge                  
    hr = BL_StringToVariant(_T("t#9223372036854d"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);

    //overflow using unary muinus on min val                  
    hr = BL_StringToVariant(_T("-t#-9223372036854775808ms"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);

    
    //no overflow using very long fraction:
    hr = BL_StringToVariant(_T("t#2.2345678d"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    l64Tmp = lD;
    l64MStot = ((__int64)2)*lD + (l64Tmp/=10)*2+
                                   (l64Tmp/=10)*3+
                                   (l64Tmp/=10)*4+
                                   (l64Tmp/=10)*5+
                                   (l64Tmp/=10)*6+
                                   (l64Tmp/=10)*7+ //0.2345678
                                   (l64Tmp/=10)*8;

    bOk = SUCCEEDED(hr) && CheckTimeVariant(&out, l64MStot);
    DumpTestResult(bOk, ++nTest);


    //test the 'most complex' syntax with unary -+:
    hr = BL_StringToVariant(_T(" -t#-1_0d_2h_3m_4S_5mS "), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    l64MStot = + (10*lD + 2*lH + 3*lM + 4*lS + 5);
    bOk = SUCCEEDED(hr) && CheckTimeVariant(&out, l64MStot);
    DumpTestResult(bOk, ++nTest);

    //test the 'most complex' syntax with unary -+:
    hr = BL_StringToVariant(_T("+t#-1d_2h_3m_4S_5mS "), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    l64MStot = - (1*lD + 2*lH + 3*lM + 4*lS + 5);
    bOk = SUCCEEDED(hr) && CheckTimeVariant(&out, l64MStot);
    DumpTestResult(bOk, ++nTest);

    //test the 'most complex' syntax with unary -+:
    hr = BL_StringToVariant(_T("\t+t#1d_2h_3m_4S_5mS "), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    l64MStot = + (1*lD + 2*lH + 3*lM + 4*lS + 5);
    bOk = SUCCEEDED(hr) && CheckTimeVariant(&out, l64MStot);
    DumpTestResult(bOk, ++nTest);

    //test the 'most complex' syntax with unary -+:
    hr = BL_StringToVariant(_T("-t#1d_2h_3m_4S_5mS\t\n\r"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    l64MStot = - (1*lD + 2*lH + 3*lM + 4*lS + 5);
    bOk = SUCCEEDED(hr) && CheckTimeVariant(&out, l64MStot);
    DumpTestResult(bOk, ++nTest);

    //test the 'most complex' syntax with unary -+:
    hr = BL_StringToVariant(_T("-t#1d_2h_3m_4S_5mS \t"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    l64MStot = - (1*lD + 2*lH + 3*lM + 4*lS + 5);
    bOk = SUCCEEDED(hr) && CheckTimeVariant(&out, l64MStot);
    DumpTestResult(bOk, ++nTest);
    
    //test usage of fraction (first int before fraction is zero!!)
    hr = BL_StringToVariant(_T("t#-0.125d"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    l64MStot = - ((lD / 1000) * 125);
    bOk = SUCCEEDED(hr) && CheckTimeVariant(&out, l64MStot);
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T("t#0.125d"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    l64MStot = + ((lD / 1000) * 125);
    bOk = SUCCEEDED(hr) && CheckTimeVariant(&out, l64MStot);
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T("t#24.125d"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    l64MStot = + (24*lD  +  (lD/1000)*125);
    bOk = SUCCEEDED(hr) && CheckTimeVariant(&out, l64MStot);
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T("t#-24.125d"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    l64MStot = - (24*lD  +  (lD/1000)*125);
    bOk = SUCCEEDED(hr) && CheckTimeVariant(&out, l64MStot);
    DumpTestResult(bOk, ++nTest);



    //test syntax error using '_'
    hr = BL_StringToVariant(_T("-t#1__0d"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = hr==E_BL_ERR_SYNTAX;
    DumpTestResult(bOk, ++nTest);
    
    //test syntax error using '_'
    hr = BL_StringToVariant(_T("-t#_1d"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = hr==E_BL_ERR_SYNTAX;
    DumpTestResult(bOk, ++nTest);
    
    //test syntax error using '_'
    hr = BL_StringToVariant(_T("T#1_d"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = hr==E_BL_ERR_SYNTAX;
    DumpTestResult(bOk, ++nTest);
    
    //test syntax error using '_'
    hr = BL_StringToVariant(_T("T#1d_"), BL_TIME, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = hr==E_BL_ERR_SYNTAX;
    DumpTestResult(bOk, ++nTest);
    //###############################################################
    




    //###############################################################
    //test string date_and_time to variant & vice versa:
    //###############################################################
    //syntax errors:
    hr = BL_StringToVariant(_T(""), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_SYNTAX, ++nTest);
    hr = BL_StringToVariant(_T(" \t\r\n"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_SYNTAX, ++nTest);
    hr = BL_StringToVariant(_T("-dt#1999-1-1-23:59:59"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_SYNTAX, ++nTest);
    hr = BL_StringToVariant(_T("dt#19_99-1-1-23:59"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_SYNTAX, ++nTest);

    //out of valid range in year:
    hr = BL_StringToVariant(_T(" dt#1601-1-1-02:00:00.0000000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);
    hr = BL_StringToVariant(_T("dt#1970-1-1-00:00:00.0000000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);
    hr = BL_StringToVariant(_T("dt#9999-12-31-23:59:59.9990000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest); // now OK.
    hr = BL_VariantToString(&out, BL_DT, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("dt#9999-12-31-23:59:59.999"));
    DumpTestResult(bOk, ++nTest);

    //out of valid range in month:
    hr = BL_StringToVariant(_T("dt#1998-13-1-00:00:00.0000000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);

    //out of valid range in day:
    hr = BL_StringToVariant(_T("dt#1998-1-32-00:00:00.0000000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);

    //out of valid range in hour:
    hr = BL_StringToVariant(_T("dt#1998-1-1-24:00:00.0000000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);

    //out of valid range in minute:
    hr = BL_StringToVariant(_T("dt#1998-1-1-00:60:00.0000000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);

    //out of valid range in sec:
    hr = BL_StringToVariant(_T("dt#1998-1-1-00:00:6_0.00_00000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);

    //non existing dates:
    hr = BL_StringToVariant(_T("dt#2001-2-29-00:00:00"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);
    //year 2000 is a leap year !
    hr = BL_StringToVariant(_T("dt#2000-2-29-00:00:0"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);

    //special case 'invalid date and time'
    hr = BL_StringToVariant(_T("dt#1-1-1-0:0:0"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.decVal.Lo64==0, ++nTest);
    hr = BL_VariantToString(&out, BL_DT, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("dt#0001-01-01-00:00:00.000"));
    DumpTestResult(bOk, ++nTest);
    
    
    //difference of one day including silvester must be 24*3600*1000 ms:
    hr = BL_StringToVariant(_T("\tdt#1998-12-31-23:59:59.99999999999999 "), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    l64MStot = out.decVal.Lo64;
    hr = BL_StringToVariant(_T("dt#1999-1-1-23:59:59.99999999999999"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = hr==S_OK && (out.decVal.Lo64 - l64MStot)==24*3600*1000;
    DumpTestResult(bOk, ++nTest);

    //difference of one hour including silvester must be 3600*1000 ms:
    hr = BL_StringToVariant(_T("\tdt#1998-12-31-23:59:59.99999999999999 "), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    l64MStot = out.decVal.Lo64;
    hr = BL_StringToVariant(_T("dt#1999-1-1-00:59:59.99999999999999"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = hr==S_OK && (out.decVal.Lo64 - l64MStot)==3600*1000;
    DumpTestResult(bOk, ++nTest);
    
    //difference of one min including silvester must be 60*1000 ms:
    hr = BL_StringToVariant(_T("\tdt#1998-12-31-23:59:59.99999999999999 "), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    l64MStot = out.decVal.Lo64;
    hr = BL_StringToVariant(_T("dt#1999-1-1-00:00:59.99999999999999"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = hr==S_OK && (out.decVal.Lo64 - l64MStot)==60*1000;
    DumpTestResult(bOk, ++nTest);

    //difference of one sec including silvester must be 1000 ms:
    hr = BL_StringToVariant(_T("\tdt#1998-12-31-23:59:59.99999999999999 "), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    l64MStot = out.decVal.Lo64;
    hr = BL_StringToVariant(_T("dt#1999-1-1-00:00:00.99999999999999"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = hr==S_OK && (out.decVal.Lo64 - l64MStot)==1000;
    DumpTestResult(bOk, ++nTest);

    //difference of 100 msec including silvester must be 100 ms:
    hr = BL_StringToVariant(_T("\tdt#1998-12-31-23:59:59.99999999999999 "), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    l64MStot = out.decVal.Lo64;
    hr = BL_StringToVariant(_T("dt#1999-1-1-00:00:00.099"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = hr==S_OK && (out.decVal.Lo64 - l64MStot)==100;
    DumpTestResult(bOk, ++nTest);

    //difference of 10 msec including silvester must be 10 ms:
    hr = BL_StringToVariant(_T("\tdt#1998-12-31-23:59:59.99999999999999 "), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    l64MStot = out.decVal.Lo64;
    hr = BL_StringToVariant(_T("dt#1999-1-1-00:00:00.009"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = hr==S_OK && (out.decVal.Lo64 - l64MStot)==10;
    DumpTestResult(bOk, ++nTest);

    //difference of 1 msec including silvester must be 1 ms:
    hr = BL_StringToVariant(_T("\tdt#1998-12-31-23:59:59.99999999999999 "), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    l64MStot = out.decVal.Lo64;
    hr = BL_StringToVariant(_T("dt#1999-1-1-00:00:00.0009"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = hr==S_OK && (out.decVal.Lo64 - l64MStot)==1;
    DumpTestResult(bOk, ++nTest);



    //test winter/summer time
    hr = BL_StringToVariant(_T("dt#1998-03-29-01:59:59.000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    l64MStot = out.decVal.Lo64;

    hr = BL_StringToVariant(_T("dt#1998-03-29-03:00:00.000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = hr==S_OK && (out.decVal.Lo64 - l64MStot)==1000;
    DumpTestResult(bOk, ++nTest);
    l64MStot = out.decVal.Lo64;


    hr = BL_StringToVariant(_T("dt#1998-10-25-01:59:59.000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    l64MStot = out.decVal.Lo64;

    hr = BL_StringToVariant(_T("dt#1998-10-25-03:00:00.000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = hr==S_OK && (out.decVal.Lo64 - l64MStot)==(2*3600*1000 + 1000);
    DumpTestResult(bOk, ++nTest);
    l64MStot = out.decVal.Lo64;


    hr = BL_StringToVariant(_T("dt#1999-10-31-01:59:59.000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    l64MStot = out.decVal.Lo64;

    hr = BL_StringToVariant(_T("dt#1999-10-31-03:00:00.000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = hr==S_OK && (out.decVal.Lo64 - l64MStot)==(2*3600*1000 + 1000);
    DumpTestResult(bOk, ++nTest);
    l64MStot = out.decVal.Lo64;



    hr = BL_StringToVariant(_T("dt#1998-10-25-02:59:59.000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    l64MStot = out.decVal.Lo64;

    hr = BL_StringToVariant(_T("dt#1998-10-25-03:00:00.000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    bOk = hr==S_OK && (out.decVal.Lo64 - l64MStot)==(1*3600*1000 + 1000);
    DumpTestResult(bOk, ++nTest);
    l64MStot = out.decVal.Lo64;



    //test variant to date_and_time    
    hr = BL_StringToVariant(_T("dt#1998-10-25-02:59:59.000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    hr = BL_VariantToString(&out, BL_DT, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("dt#1998-10-25-02:59:59.000"));
    DumpTestResult(bOk, ++nTest);


    //start test sequence 'pervers' : summer to winter transition
    hr = BL_StringToVariant(_T("dt#1998-10-25-01:00:00.000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    out.decVal.Lo64 += 1*3600*1000;    
    hr = BL_VariantToString(&out, BL_DT, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("dt#1998-10-25-02:00:00.000"));
    DumpTestResult(bOk, ++nTest);

    out.decVal.Lo64 += 1*3600*1000;    
    hr = BL_VariantToString(&out, BL_DT, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("dt#1998-10-25-02:00:00.000"));
    DumpTestResult(bOk, ++nTest);

    out.decVal.Lo64 += 1*3600*1000;    
    hr = BL_VariantToString(&out, BL_DT, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("dt#1998-10-25-03:00:00.000"));
    DumpTestResult(bOk, ++nTest);
    //end test sequence 'pervers'




    hr = BL_StringToVariant(_T("dt#1998-10-25-03:00:00.000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    hr = BL_VariantToString(&out, BL_DT, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("dt#1998-10-25-03:00:00.000"));
    DumpTestResult(bOk, ++nTest);

    
    hr = BL_StringToVariant(_T("dt#1998-03-29-01:59:59.000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    hr = BL_VariantToString(&out, BL_DT, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("dt#1998-03-29-01:59:59.000"));
    DumpTestResult(bOk, ++nTest);
    
    hr = BL_StringToVariant(_T("dt#1998-03-29-03:00:00.000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    hr = BL_VariantToString(&out, BL_DT, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("dt#1998-03-29-03:00:00.000"));
    DumpTestResult(bOk, ++nTest);
    
#if 0 
    //this is the case of a DT where conversion is not symmetric
    hr = BL_StringToVariant(_T("dt#1998-03-29-02:00:00.000"), BL_DT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    hr = BL_VariantToString(&out, BL_DT, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("dt#1998-03-29-02:00:00.000"));
    DumpTestResult(bOk, ++nTest);
#endif    
    
    

    //###############################################################
    //test string tod to variant & vice versa
    //###############################################################
    //syntax errors:
    DumpTestResult(BL_StringToVariant(_T(""), BL_TOD, NULL, BL_USE_IEC_FORMAT, &out)==E_BL_ERR_SYNTAX, ++nTest);
    DumpTestResult(BL_StringToVariant(_T("\n"), BL_TOD, NULL, BL_USE_IEC_FORMAT, &out)==E_BL_ERR_SYNTAX, ++nTest);
    DumpTestResult(BL_StringToVariant(_T("-tod#0:0:0"), BL_TOD, NULL, BL_USE_IEC_FORMAT, &out)==E_BL_ERR_SYNTAX, ++nTest);
    DumpTestResult(BL_StringToVariant(_T("+tod#0:0:0"), BL_TOD, NULL, BL_USE_IEC_FORMAT, &out)==E_BL_ERR_SYNTAX, ++nTest);
    DumpTestResult(BL_StringToVariant(_T("tod#-0:0:0"), BL_TOD, NULL, BL_USE_IEC_FORMAT, &out)==E_BL_ERR_SYNTAX, ++nTest);

    hr = BL_StringToVariant(_T("tod#0:0:0.000000000000000000000000000000000000000"), BL_TOD, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0 , ++nTest);
    hr = BL_VariantToString(&out, BL_TOD, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("tod#00:00:00.000"));
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T("tod#05:13:19.345"), BL_TOD, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==1000*(5*3600+13*60+19)+345 , ++nTest);
    hr = BL_VariantToString(&out, BL_TOD, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("tod#05:13:19.345"));
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T("TOD#05:13:19.345"), BL_TOD, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==1000*(5*3600+13*60+19)+345 , ++nTest);

    hr = BL_StringToVariant(_T("ToD#05:13:19.34_59999999999999999"), BL_TOD, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==1000*(5*3600+13*60+19)+345 , ++nTest);

    hr = BL_StringToVariant(_T("TIME_oF_DaY#05:1_3:19.345"), BL_TOD, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==1000*(5*3600+13*60+19)+345 , ++nTest);
    
    //###############################################################
    //test string date to variant & vice versa
    //###############################################################
    //syntax errors:
    DumpTestResult(BL_StringToVariant(_T(""), BL_DATE, NULL, BL_USE_IEC_FORMAT, &out)==E_BL_ERR_SYNTAX, ++nTest);
    DumpTestResult(BL_StringToVariant(_T("\n"), BL_DATE, NULL, BL_USE_IEC_FORMAT, &out)==E_BL_ERR_SYNTAX, ++nTest);
    DumpTestResult(BL_StringToVariant(_T("-DATE#1998-11-05"), BL_DATE, NULL, BL_USE_IEC_FORMAT, &out)==E_BL_ERR_SYNTAX, ++nTest);
    DumpTestResult(BL_StringToVariant(_T("+DATE#1998-11-05"), BL_DATE, NULL, BL_USE_IEC_FORMAT, &out)==E_BL_ERR_SYNTAX, ++nTest);
    DumpTestResult(BL_StringToVariant(_T("DATE#-1998-11-05"), BL_DATE, NULL, BL_USE_IEC_FORMAT, &out)==E_BL_ERR_SYNTAX, ++nTest);
    DumpTestResult(BL_StringToVariant(_T("DATE#19__98-11-05"), BL_DATE, NULL, BL_USE_IEC_FORMAT, &out)==E_BL_ERR_SYNTAX, ++nTest);

    //special case 'invalid date'
    hr = BL_StringToVariant(_T( "date#1-1-1"), BL_DATE, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0, ++nTest);
    hr = BL_VariantToString(&out, BL_DATE, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("d#0001-01-01"));
    DumpTestResult(bOk, ++nTest);


    hr = BL_StringToVariant(_T( "date#19_98-1_1-0_5\r"), BL_DATE, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    hr = BL_VariantToString(&out, BL_DATE, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("d#1998-11-05"));
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T( "DATE#1998-11-05\t"), BL_DATE, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    hr = BL_VariantToString(&out, BL_DATE, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("d#1998-11-05"));
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T( "DaTE#1998-11-05\r"), BL_DATE, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    hr = BL_VariantToString(&out, BL_DATE, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("d#1998-11-05"));
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T( "D#1998-11-05\r"), BL_DATE, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    hr = BL_VariantToString(&out, BL_DATE, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("d#1998-11-05"));
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T( "d#1998-11-05\r"), BL_DATE, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK, ++nTest);
    hr = BL_VariantToString(&out, BL_DATE, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("d#1998-11-05"));
    DumpTestResult(bOk, ++nTest);

    hr = BL_StringToVariant(_T("date#1-1-1"), BL_DATE, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0 , ++nTest);
    hr = BL_VariantToString(&out, BL_DATE, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    bOk = hr==S_OK && !_tcscmp(buffer, _T("d#0001-01-01"));
    DumpTestResult(bOk, ++nTest);


    
    //###############################################################
    //test string BYTE,WORD,DWORD,DINT,INT to variant & vice versa
    //###############################################################

    hr = BL_StringToVariant(_T("2#0"), BL_BYTE, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0 , ++nTest);
    hr = BL_StringToVariant(_T("2#0"), BL_WORD, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0 , ++nTest);
    hr = BL_StringToVariant(_T("2#0"), BL_DWORD, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0 , ++nTest);
    hr = BL_StringToVariant(_T("2#0"), BL_DINT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0 , ++nTest);
    hr = BL_StringToVariant(_T("2#0"), BL_INT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0 , ++nTest);
    hr = BL_StringToVariant(_T("-0"), BL_INT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0 , ++nTest);
    hr = BL_StringToVariant(_T("-23450"), BL_INT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==-23450 , ++nTest);
    hr = BL_StringToVariant(_T("+23450"), BL_INT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==+23450 , ++nTest);
    hr = BL_StringToVariant(_T("int#16#ffff"), BL_INT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==USHRT_MAX , ++nTest);

    hr = BL_VariantToString(&out, BL_INT, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
    DumpTestResult(hr==S_OK && !lstrcmp(buffer, L"65535"), ++nTest);

    //under/flow:
    hr = BL_StringToVariant(_T("53450"), BL_INT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);
    hr = BL_StringToVariant(_T("-53450"), BL_INT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_RANGE, ++nTest);
    //syntax error:
    hr = BL_StringToVariant(_T("-53__450"), BL_INT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==E_BL_ERR_SYNTAX, ++nTest);




    hr = BL_StringToVariant(_T("bYte#2#0"), BL_BYTE, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0 , ++nTest);
    hr = BL_StringToVariant(_T("Word#2#0"), BL_WORD, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0 , ++nTest);
    hr = BL_StringToVariant(_T("dWord#2#0"), BL_DWORD, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0 , ++nTest);
    hr = BL_StringToVariant(_T("dinT#2#0"), BL_DINT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0 , ++nTest);
    hr = BL_StringToVariant(_T("inT#2#0"), BL_INT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0 , ++nTest);


    hr = BL_StringToVariant(_T("16#ff"), BL_BYTE, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0xff , ++nTest);
    hr = BL_StringToVariant(_T("16#ffff"), BL_WORD, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0xffff , ++nTest);
    hr = BL_StringToVariant(_T("16#ffffffff"), BL_DWORD, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0xffffffff , ++nTest);
    hr = BL_StringToVariant(_T("2147483647"), BL_DINT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==2147483647 , ++nTest);
    hr = BL_StringToVariant(_T("   -2147483648"), BL_DINT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==LONG_MIN , ++nTest);
    hr = BL_StringToVariant(_T("   32767"), BL_INT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==SHRT_MAX , ++nTest);
    hr = BL_StringToVariant(_T("   -32768"), BL_INT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==SHRT_MIN , ++nTest);


    hr = BL_StringToVariant(_T("2#11111111"), BL_BYTE, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0xff , ++nTest);
    hr = BL_StringToVariant(_T("2#1111111111111111"), BL_WORD, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0xffff , ++nTest);
    hr = BL_StringToVariant(_T("2#11111111111111111111111111111111"), BL_DWORD, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0xffffffff , ++nTest);
                                  
    hr = BL_StringToVariant(_T("2#10000000000000000000000000000000"), BL_DWORD, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0x80000000 , ++nTest);
                                  
    hr = BL_StringToVariant(_T("2#11111111111111111111111111111111"), BL_DWORD, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==-1 , ++nTest);
    
    hr = BL_StringToVariant(_T("2#10000000000000000000000000000000"), BL_DINT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0x80000000 , ++nTest);
                                  
    hr = BL_StringToVariant(_T("2#11111111111111111111111111111111"), BL_DINT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==-1 , ++nTest);
    
    hr = BL_StringToVariant(_T("2#1000000000000000"), BL_INT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0x8000 , ++nTest);
                                  
    hr = BL_StringToVariant(_T("2#1111111111111111"), BL_INT, NULL, BL_USE_IEC_FORMAT, &out);
    DumpTestResult(hr==S_OK && out.lVal==0xffff , ++nTest);

    

    _tprintf(_T("number of tests %d errors=%d\n"), nTest,NtotalErrors); 
    return NtotalErrors;
}











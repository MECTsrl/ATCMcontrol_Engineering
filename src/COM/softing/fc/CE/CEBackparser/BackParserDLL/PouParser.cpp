
#include "stdafx.h"

#include "PouParser.h"
#include "Global.h"

//------------------------------------------------------------------*
/**
 * Function name    PouParser::yyparse
 * Description      parses the pou read by scanner and creates a POU object
 * @param           yy_scan* scanner
 * @param           POU** pou
 * @return          int
 * @exception       -
 * @see             -
*/
//================================================================================
int PouParser::yyparse(yy_scan* scanner, POU** pou)
//================================================================================
{
    int res = yy_parse::yyparse(scanner);
    if ( res != 0 )
    {
        clear();
    }

    (*pou) = GetPou();
    return res;
}

//------------------------------------------------------------------*
/**
 * Function name    PouParser::yyparse
 * Description      parses the pou read by scanner and creates a FunctionBlock object
 * @param           yy_scan* scanner
 * @param           FunctionBlock** fb
 * @return          int
 * @exception       -
 * @see             -
*/
//================================================================================
int PouParser::yyparse(yy_scan* scanner, FunctionBlock** fb)
//================================================================================
{
    POU *pou = 0;
    int res = yyparse(scanner, &pou);
    if ( res == 0 )
    {
        if ( pou->getPOUType() != _FUNCTION_BLOCK )
        {
            delete pou;
            pou = NULL;
            (*fb) = NULL;
            throw string("not a function block");
        }
    }
    
    (*fb) = (FunctionBlock*)pou;
    return res;
}

//------------------------------------------------------------------*
/**
 * Function name    PouParser::yyparse
 * Description      parses the pou read by scanner and creates a Function object
 * @param           yy_scan* scanner
 * @param           Function** f
 * @return          int
 * @exception       -
 * @see             -
*/
//================================================================================
int PouParser::yyparse(yy_scan* scanner, Function** f)
//================================================================================
{
    POU *pou = 0;
    int res = yyparse(scanner, &pou);
    if ( res == 0 )
    {
        if ( pou->getPOUType() != _FUNCTION )
        {
            delete pou;
            pou = NULL;
            (*f) = NULL;
            throw string("not a function");
        }
    }
    
    (*f) = (Function*)pou;
    return res;
}

//------------------------------------------------------------------*
/**
 * Function name    PouParser::yyparse
 * Description      parses the pou read by scanner and creates a Resource object
 * @param           yy_scan* scanner
 * @param           Resource** resource
 * @return          int
 * @exception       -
 * @see             -
*/
//================================================================================
int PouParser::yyparse(yy_scan* scanner, Resource** resource)
//================================================================================
{
    POU *pou = 0;
    int res = yyparse(scanner, &pou);
    if ( res == 0 )
    {
        if ( pou->getPOUType() != _RESOURCE )
        {
            delete pou;
            pou = NULL;
            (*resource) = NULL;
            throw string("not a resource");
        }
    }
    
    (*resource) = (Resource*)pou;
    return res;
}

//------------------------------------------------------------------*
/**
 * Function name    PouParser::yyparse
 * Description      parses the pou read by scanner and creates a Configuration object
 * @param           yy_scan* scanner
 * @param           Configuration** configuration
 * @return          int
 * @exception       -
 * @see             -
*/
//================================================================================
int PouParser::yyparse(yy_scan* scanner, Configuration** configuration)
//================================================================================
{
    POU *pou = 0;
    int res = yyparse(scanner, &pou);
    if ( res == 0 )
    {
        if ( pou->getPOUType() != _CONFIGURATION )
        {
            delete pou;
            pou = NULL;
            (*configuration) = NULL;
            throw string("not a configuration");
        }
    }
    
    (*configuration) = (Configuration*)pou;
    return res;
}

//------------------------------------------------------------------*
/**
 * Function name    PouParser::yyparse
 * Description      parses the pou read by scanner and creates a GlobVarConstList object
 * @param           yy_scan* scanner
 * @param           GlobVarConstList** gvcl
 * @return          int
 * @exception       -
 * @see             -
*/
//================================================================================
int PouParser::yyparse(yy_scan* scanner, GlobVarConstList** gvcl)
//================================================================================
{
    POU *pou = 0;
    int res = yyparse(scanner, &pou);

/*  Constant and global variable files may be empty. So the type can not be determined.
    So the type can not be checked.
    if ( res == 0 )
    {
        if (( pou->getPOUType() != _CST_FILE ) &&
            ( pou->getPOUType() != _GVL_FILE ))
        {
            throw string("not a cst file or gvl file");
        }
    }
*/    
    (*gvcl) = (GlobVarConstList*)pou;
    return res;
}

//------------------------------------------------------------------*
/**
 * Function name    PouParser::yyparse
 * Description      parses the pou read by scanner and creates a StStruct object
 * @param           yy_scan* scanner
 * @param           StStruct** st
 * @return          int
 * @exception       -
 * @see             -
*/
//================================================================================
int PouParser::yyparse(yy_scan* scanner, StStruct** st)
//================================================================================
{
    POU *pou = 0;
    int res = yyparse(scanner, &pou);
    if ( res == 0 )
    {
        if ( pou->getPOUType() != _STRUCT )
        {
            delete pou;
            pou = NULL;
            (*st) = NULL;
            throw string("not a structure");
        }
    }
    
    (*st) = (StStruct*)pou;
    return res;
}

//------------------------------------------------------------------*
/**
 * Function name    PouParser::getStreamScanner
 * Description      returns the scanner used by the parser
 * @return          stream_scan*
 * @exception       -
 * @see             -
*/
//================================================================================
stream_scan* PouParser::getStreamScanner()
//================================================================================
{
    return (stream_scan*)scan;
}


#ifndef __DEFINES_H_
#define __DEFINES_H_


#define MODULE_USER_NAME _T("4cg")     //used for error messages that have no edipos
#define MODULE_KAD_NAME _T("4cg.kad")  //the KAD file name (path is this module's path)


//compiler limits
const int ST_MAX_IDENTIFIER_LENGTH = 32;
const int ST_MAX_FUNCTION_PARAMS = 32; //including function return
const int ST_MAX_JAVA_STACK_VARS = 65000;
const int ST_MAX_NUMBER_ERRORS = 50;
const int ST_MAX_FILE_SIZE = 0x10000000;

//TODO via initFeatures:
const int MAX_ARRAY_ELEMENTS = 1024*64;
const int MAX_STRING_LENGTH  = 1024*64;
const int MAX_INSTPATH_NESTING = 20;   




#define ST_IS_NAME_EQUAL(x,y) (!lstrcmpi(x,y))



#endif // __DEFINES_H_


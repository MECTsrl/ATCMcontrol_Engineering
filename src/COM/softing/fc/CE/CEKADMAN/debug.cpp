



#include "stdafx.h"

#include "debug.h"

#ifdef DEBUG_ACTIVE

int CAllocCheck::allocCounter = 0;

CAllocCheck::CAllocCheck()
{
    FILE *f;
    f = fopen("alloc.log","ab");
    if (f != NULL) {
        allocCounter++;
        fprintf(f,"alloc counter increased to %d\r\n",allocCounter);
        fclose(f);
    }
}

CAllocCheck::~CAllocCheck()
{
    FILE *f;
    f = fopen("alloc.log","ab");
    if (f != NULL) {
        allocCounter--
            ;
        fprintf(f,"alloc counter decreased to %d\r\n",allocCounter);
        fclose(f);
    }
}

#endif


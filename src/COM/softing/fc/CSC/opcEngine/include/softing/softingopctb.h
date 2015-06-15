#ifndef _SOFTINGOPCTB_H_
#define _SOFTINGOPCTB_H_

#define TOOLKIT_MAGIC 4308743
extern DWORD g_toolkitMagic;

OPCENGINE_EXPORT void setOPCToolkitMagic(DWORD magic);

#define SOFTING_MAGIC_CHECK (g_toolkitMagic == TOOLKIT_MAGIC)
#define SOFTING_INIT ::setOPCToolkitMagic(TOOLKIT_MAGIC);

#endif
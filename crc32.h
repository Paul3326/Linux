#ifndef __CRC32__INC__
#define __CRC32__INC__

#ifdef __cplusplus
extern "C" {
#endif

void InitCrc32();
int UpdCrc32 (void *pStart, unsigned int uSize)  ;
unsigned int FinalCrc32();

#ifdef __cplusplus
}
#endif

#endif
